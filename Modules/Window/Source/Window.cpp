#include "Window.hpp"

#include <Exception/Module.hpp>
#include <GLFW/glfw3.h>

namespace Light {

	WindowModule* Window::s_Module = nullptr;

	WindowModule::WindowModule()
	    : Module(true)
	{
	}

	WindowModule::~WindowModule()
	{
	}

	bool WindowModule::HasRequestedAppTermination() const
	{
		return glfwWindowShouldClose(m_Handle);
	}

	void WindowModule::OnConfig()
	{
	}

	void WindowModule::OnInit()
	{
		LoggerCategoryCreateInfo categoryInfo = {
			"Window",                 // name
			LOGGER_DEFAULT_PATTERN,   // pattern
			LoggerType::eStdoutColor, // type
			NULL,                     // outputFile
		};

		Logger::CreateCategory(categoryInfo);

		ASSERT(glfwInit(), "Failed to initialize glfw");

		// #TODO: Make mapping of glfw window hints and expose them as an enum in Window .hpp
		// so you can write the defaults on the struct and not here...
		m_Config.hints.insert(m_Config.hints.end(), { { GLFW_CLIENT_API, GLFW_NO_API },
		                                              { GLFW_DECORATED, GLFW_FALSE },
		                                              { GLFW_VISIBLE, GLFW_FALSE } });

		auto [monitorX, monitorY]          = GetMonitorPosition();
		auto [monitorWidth, monitorHeight] = GetMonitorSize();

		if (m_Config.sizeRelativeToMonitor)
		{
			m_Config.width  = monitorWidth * m_Config.monitorWidthMultiplier;
			m_Config.height = m_Config.width / m_Config.aspectRatio;
		}
		for (auto hint : m_Config.hints)
		{
			glfwWindowHint(hint.first, hint.second);
			LOG("Window", LogLvl::eTrace, "Window hint -> {} = {}", hint.first, hint.second);
		}

		// #TODO: Add fullscreen support
		m_Handle = glfwCreateWindow(m_Config.width, m_Config.height, m_Config.title.c_str(), NULL, NULL);
		ASSERT(m_Handle, "Failed to create glfw window");

		BindGlfwCallbacks();

		glfwSetWindowPos(m_Handle,
		                 monitorX + (monitorWidth - m_Config.width) / 2,
		                 monitorY + (monitorHeight - m_Config.height) / 2);

		glfwSetWindowUserPointer(m_Handle, &m_Notifiers);

		SetVisibility(true);

		Bind_MouseMove(&WindowModule::Test, this);
	}

	bool WindowModule::Test(std::pair<double, double> pos)
	{
		auto [x, y] = pos;
		LOG("Window", LogLvl::eTrace, "Mouse moved {} - {}", x, y);

		return false;
	}

	void WindowModule::OnUpdate()
	{
		glfwSwapBuffers(m_Handle);
		glfwPollEvents();
	}

	void WindowModule::OnDeinit()
	{
		glfwDestroyWindow(m_Handle);
	}

	void WindowModule::BindGlfwCallbacks()
	{
		////////////////////////////////////////////////////////////////
		/// Mouse events
		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* window, double xpos, double ypos) {
			((NotifierList*)glfwGetWindowUserPointer(window))->mouseMove.Invoke({ xpos, ypos });
		});

		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
			((NotifierList*)glfwGetWindowUserPointer(window))->mouseButton.Invoke(std::forward<int32_t>(button), std::forward<int32_t>(action), std::forward<int32_t>(mods));
		});

		glfwSetCursorEnterCallback(m_Handle, [](GLFWwindow* window, int entered) {
			((NotifierList*)glfwGetWindowUserPointer(window))->mouseEnter.Invoke(std::forward<int32_t>(entered));
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xoffset, double yoffset) {
			((NotifierList*)glfwGetWindowUserPointer(window))->mouseScroll.Invoke({ xoffset, yoffset });
		});

		////////////////////////////////////////////////////////////////
		/// Keyboard events
		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int mods) {
			((NotifierList*)glfwGetWindowUserPointer(window))->key.Invoke(std::forward<int32_t>(key), std::forward<int32_t>(scancode), std::forward<int32_t>(action), std::forward<int32_t>(mods));

			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});

		glfwSetCharCallback(m_Handle, [](GLFWwindow* window, uint32_t character) {
			((NotifierList*)glfwGetWindowUserPointer(window))->character.Invoke(std::forward<uint32_t>(character));
		});

		////////////////////////////////////////////////////////////////
		/// Window events
		glfwSetWindowPosCallback(m_Handle, [](GLFWwindow* window, int32_t xpos, int32_t ypos) {
			((NotifierList*)glfwGetWindowUserPointer(window))->windowMove.Invoke({ xpos, ypos });
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int32_t width, int32_t height) {
			((NotifierList*)glfwGetWindowUserPointer(window))->windowResize.Invoke({ width, height });
		});

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window) {
			((NotifierList*)glfwGetWindowUserPointer(window))->windowClose.Invoke();
		});

		glfwSetWindowFocusCallback(m_Handle, [](GLFWwindow* window, int32_t focus) {
			((NotifierList*)glfwGetWindowUserPointer(window))->windowFocus.Invoke(std::forward<int32_t>(focus));
		});

		////////////////////////////////////////////////////////////////
		/// File drop
		glfwSetDropCallback(m_Handle, [](GLFWwindow* window, int32_t count, const char** paths) {
			((NotifierList*)glfwGetWindowUserPointer(window))->fileDrop.Invoke(std::forward<int32_t>(count), std::forward<const char**>(paths));
		});
	}

	void WindowModule::MakeCentered()
	{
		auto [monitorX, monitorY]          = GetMonitorPosition();
		auto [monitorWidth, monitorHeight] = GetMonitorSize();
		auto [windowWidth, windowHeight]   = GetWindowSize();

		glfwSetWindowPos(m_Handle,
		                 monitorX + (monitorWidth - windowWidth) / 2,
		                 monitorY + (monitorHeight - windowHeight) / 2);
	}

	void WindowModule::SetVisibility(bool visible, bool toggle)
	{
		visible = toggle ? !m_Visible : visible;

		if (visible)
		{
			glfwShowWindow(m_Handle);
		}
		else
		{
			glfwHideWindow(m_Handle);
		}
	}

	std::pair<int32_t, int32_t> WindowModule::GetMonitorPosition() const
	{
		int32_t count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		std::pair<int32_t, int32_t> position;
		glfwGetMonitorPos(monitors[0], &position.first, &position.second);

		return position;
	}

	std::pair<int32_t, int32_t> WindowModule::GetMonitorSize() const
	{
		int32_t count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		const GLFWvidmode* vidmode = glfwGetVideoMode(monitors[0]);
		return { vidmode->width, vidmode->height };
	}


	std::pair<int32_t, int32_t> WindowModule::GetWindowSize() const
	{
		std::pair<int32_t, int32_t> size;
		glfwGetWindowSize(m_Handle, &size.first, &size.second);
		return size;
	}

	std::pair<int32_t, int32_t> WindowModule::GetWindowPosition() const
	{
		std::pair<int32_t, int32_t> position;
		glfwGetWindowPos(m_Handle, &position.first, &position.second);
		return position;
	}

} // namespace Light
