#include "Window.hpp"

#include <GLFW/glfw3.h>

namespace Light { namespace Window {

	Module::Module()
	    : Mojula::Module("Window", 420ull, { 69ull }, true)
	{
	}

	Module::~Module()
	{
	}

	bool Module::HasRequestedAppTermination() const
	{
		return glfwWindowShouldClose(m_Handle);
	}

	void Module::StoreAPI(uint64_t module_uuid, class Mojula::ModuleAPI* api)
	{
		if (module_uuid == 69ull)
		{
			m_LoggerAPI = static_cast<Logger::ModuleAPI*>(api);
		}
	}

	void Module::OnInit()
	{
		m_LoggerAPI->CreateCategory("Window");

		glfwInit();

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
			LOG(Window, trace, "Window hint, {} = {}", hint.first, hint.second);
		}


		// #TODO: Add fullscreen support
		m_Handle = glfwCreateWindow(m_Config.width, m_Config.height, m_Config.title.c_str(), NULL, NULL);
		BindEvents();

		glfwSetWindowPos(m_Handle,
		                 monitorX + (monitorWidth - m_Config.width) / 2,
		                 monitorY + (monitorHeight - m_Config.height) / 2);

		SetVisibility(true);
	}

	void Module::OnUpdate()
	{
		glfwPollEvents();
	}

	void Module::BindEvents()
	{
		////////////////////////////////////////////////////////////////
		/// Mouse events
		glfwSetCursorPosCallback(m_Handle, [](GLFWwindow* window, double xpos, double ypos) {
		});

		glfwSetMouseButtonCallback(m_Handle, [](GLFWwindow* window, int button, int action, int mods) {
		});

		glfwSetScrollCallback(m_Handle, [](GLFWwindow* window, double xoffset, double yoffset) {
		});

		////////////////////////////////////////////////////////////////
		/// Keyboard events
		glfwSetKeyCallback(m_Handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE)
				glfwSetWindowShouldClose(window, true);
		});

		glfwSetCharCallback(m_Handle, [](GLFWwindow* window, unsigned int character) {
		});

		////////////////////////////////////////////////////////////////
		/// Window events
		glfwSetWindowPosCallback(m_Handle, [](GLFWwindow* window, int xpos, int ypos) {
		});

		glfwSetWindowSizeCallback(m_Handle, [](GLFWwindow* window, int width, int height) {
		});

		glfwSetWindowCloseCallback(m_Handle, [](GLFWwindow* window) {
		});

		glfwSetWindowFocusCallback(m_Handle, [](GLFWwindow* window, int focus) {
		});
	}


	void Module::MakeCentered()
	{
		auto [monitorX, monitorY]          = GetMonitorPosition();
		auto [monitorWidth, monitorHeight] = GetMonitorSize();
		auto [windowWidth, windowHeight]   = GetWindowSize();

		glfwSetWindowPos(m_Handle,
		                 monitorX + (monitorWidth - windowWidth) / 2,
		                 monitorY + (monitorHeight - windowHeight) / 2);
	}

	void Module::SetVisibility(bool visible, bool toggle)
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

	std::pair<int32_t, int32_t> Module::GetMonitorPosition() const
	{
		int32_t count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		const GLFWvidmode* vidmode = glfwGetVideoMode(monitors[0]);

		std::pair<int32_t, int32_t> position;
		glfwGetMonitorPos(monitors[0], &position.first, &position.second);

		return position;
	}

	std::pair<int32_t, int32_t> Module::GetMonitorSize() const
	{
		int32_t count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		const GLFWvidmode* vidmode = glfwGetVideoMode(monitors[0]);

		return { vidmode->width, vidmode->height };
	}


	std::pair<int32_t, int32_t> Module::GetWindowSize() const
	{
		std::pair<int32_t, int32_t> size;
		glfwGetWindowSize(m_Handle, &size.first, &size.second);

		return size;
	}

	std::pair<int32_t, int32_t> Module::GetWindowPosition() const
	{
		std::pair<int32_t, int32_t> position;
		glfwGetWindowPos(m_Handle, &position.first, &position.second);
		return position;
	}

}} // namespace Light::Window
