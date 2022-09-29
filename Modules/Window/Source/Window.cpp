#include "Window.hpp"

#include <Exception/Module.hpp>
#include <GLFW/glfw3.h>

namespace Light {

	WindowModule* Window::self = nullptr;

	WindowModule::WindowModule()
	    : Module(Module::TickType::eNone, true)
	{
		WindowModule::Facade::self = this;

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

		auto [monitorX, monitorY]          = Window::GetMonitorPosition();
		auto [monitorWidth, monitorHeight] = Window::GetMonitorSize();

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

		glfwSetWindowUserPointer(m_Handle, &m_Events);

		Window::SetVisibility(true);
	}

	WindowModule::~WindowModule()
	{
		WindowModule::Facade::self = nullptr;

		glfwDestroyWindow(m_Handle);
	}

	bool WindowModule::HasRequestedAppTermination() const
	{
		return glfwWindowShouldClose(m_Handle);
	}

	void WindowModule::OnTick()
	{
	}

	void WindowModule::OnSync()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Handle); // @todo remove when vulkan renderer is added
	}

	void WindowModule::BindGlfwCallbacks()
	{
		////////////////////////////////////////////////////////////////
		/// Mouse events
		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* window, double xpos, double ypos) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->mouseMove.push_back({ xpos, ypos });
		});

		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->mouseButton.push_back({ (button), (action), (mods) });
		});

		glfwSetCursorEnterCallback(m_Handle, [](GLFWwindow* window, int entered) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->mouseEnter.push_back({ (entered) });
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xoffset, double yoffset) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->mouseScroll.push_back({ xoffset, yoffset });
		});

		////////////////////////////////////////////////////////////////
		/// Keyboard events
		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int mods) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->key.push_back({ (key), (scancode), (action), (mods) });

			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});

		glfwSetCharCallback(m_Handle, [](GLFWwindow* window, uint32_t character) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->character.push_back({ character });
		});

		////////////////////////////////////////////////////////////////
		/// Window events
		glfwSetWindowPosCallback(m_Handle, [](GLFWwindow* window, int32_t xpos, int32_t ypos) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->windowMove.push_back({ xpos, ypos });
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int32_t width, int32_t height) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->windowResize.push_back({ width, height });
		});

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->windowClose.push_back({});
		});

		glfwSetWindowFocusCallback(m_Handle, [](GLFWwindow* window, int32_t focus) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->windowFocus.push_back({ (focus) });
		});

		////////////////////////////////////////////////////////////////
		/// File drop
		glfwSetDropCallback(m_Handle, [](GLFWwindow* window, int32_t count, const char** paths) {
			((WindowEventList*)glfwGetWindowUserPointer(window))->fileDrop.push_back({ (count), std::forward<const char**>(paths) });
		});
	}

	void WindowModule::Facade::MakeCentered()
	{
		auto [monitorX, monitorY]          = GetMonitorPosition();
		auto [monitorWidth, monitorHeight] = GetMonitorSize();
		auto [windowWidth, windowHeight]   = GetWindowSize();

		glfwSetWindowPos(self->m_Handle,
		                 monitorX + (monitorWidth - windowWidth) / 2,
		                 monitorY + (monitorHeight - windowHeight) / 2);
	}

	void WindowModule::Facade::SetVisibility(bool visible, bool toggle)
	{
		visible = toggle ? !self->m_Visible : visible;

		if (visible)
		{
			glfwShowWindow(self->m_Handle);
		}
		else
		{
			glfwHideWindow(self->m_Handle);
		}
	}

	std::pair<int32_t, int32_t> WindowModule::Facade::GetMonitorPosition()
	{
		int32_t count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		std::pair<int32_t, int32_t> position;
		glfwGetMonitorPos(monitors[0], &position.first, &position.second);

		return position;
	}

	std::pair<int32_t, int32_t> WindowModule::Facade::GetMonitorSize()
	{
		int32_t count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		const GLFWvidmode* vidmode = glfwGetVideoMode(monitors[0]);
		return { vidmode->width, vidmode->height };
	}


	std::pair<int32_t, int32_t> WindowModule::Facade::GetWindowSize()
	{
		std::pair<int32_t, int32_t> size;
		glfwGetWindowSize(self->m_Handle, &size.first, &size.second);
		return size;
	}

	std::pair<int32_t, int32_t> WindowModule::Facade::GetWindowPosition()
	{
		std::pair<int32_t, int32_t> position;
		glfwGetWindowPos(self->m_Handle, &position.first, &position.second);
		return position;
	}

	std::vector<std::tuple<double, double>> WindowModule::Facade::GetMouseMoveEvents()
	{
		return self->m_Events.mouseMove;
	}

	std::vector<std::tuple<int32_t, int32_t, int32_t>> WindowModule::Facade::GetMouseButtonEvents()
	{
		return self->m_Events.mouseButton;
	}

	std::vector<std::tuple<double, double>> WindowModule::Facade::GetMouseScrollEvents()
	{
		return self->m_Events.mouseScroll;
	}

	std::vector<std::tuple<int>> WindowModule::Facade::GetMouseEnterEvents()
	{
		return self->m_Events.mouseEnter;
	}

	std::vector<std::tuple<int, int, int, int>> WindowModule::Facade::GetKeyEvents()
	{
		return self->m_Events.key;
	}

	std::vector<std::tuple<uint32_t>> WindowModule::Facade::GetCharacterEvents()
	{
		return self->m_Events.character;
	}

	std::vector<std::tuple<int32_t, int32_t>> WindowModule::Facade::GetWindowMoveEvents()
	{
		return self->m_Events.windowMove;
	}

	std::vector<std::tuple<int32_t, int32_t>> WindowModule::Facade::GetWindowResizeEvents()
	{
		return self->m_Events.windowResize;
	}

	std::vector<std::tuple<int32_t>> WindowModule::Facade::GetWindowFocusEvents()
	{
		return self->m_Events.windowFocus;
	}

	std::vector<std::tuple<>> WindowModule::Facade::GetWindowCloseEvents()
	{
		return self->m_Events.windowClose;
	}

	std::vector<std::tuple<int, const char**>> WindowModule::Facade::GetFileDropEvents()
	{
		return self->m_Events.fileDrop;
	}


} // namespace Light
