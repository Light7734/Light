#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <utility>
#include <vector>

class GLFWwindow;

namespace Light {


	/** @todo Docs */
	struct WindowModuleConfig
	{
		std::vector<std::pair<int, int>> hints = {};

		std::string title = "Light Engine";

		uint32_t width  = 0u;
		uint32_t height = 0u;

		bool sizeRelativeToMonitor   = true;
		float monitorWidthMultiplier = 0.75;
		float aspectRatio            = 16.0 / 9.0;

		bool fullscreen  = false;
		bool vsyncEnable = false;
	};

	/**
    * @brief Module for creating and handling the main application's Window
    * @todo Multi-monitor support
    * @todo Multi-window suuport [ ? ]
    * @todo Fullscreen support
    * @todo Window hint mapping
    */
	class WindowModule final : public Module
	{
	public:
		WindowModule();
		virtual ~WindowModule() override;

		////////////////////////////////////////////////////////////////
		/// Module Interface
		virtual bool HasRequestedAppTermination() const override;

		virtual void OnConfig() override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void OnDeinit() override;

		////////////////////////////////////////////////////////////////
		/// Facade Functions
		void MakeCentered();

		void SetVisibility(bool visible, bool toggle = false);

		std::pair<int32_t, int32_t> GetMonitorPosition() const;
		std::pair<int32_t, int32_t> GetMonitorSize() const;

		std::pair<int32_t, int32_t> GetWindowSize() const;
		std::pair<int32_t, int32_t> GetWindowPosition() const;

		inline bool IsVisible() const { return m_Visible; }

	private:
		void BindGlfwCallbacks();

	private:
		WindowModuleConfig m_Config = {};

		bool m_Visible;

		GLFWwindow* m_Handle = {};
	};

	/** @brief Facade of the WindowModule */
	class Window
	{
	public:
		Window()  = delete;
		~Window() = delete;

		/** @brief Initialize the facade with the actual module 
         * @note Do not manually call this */
		static void Init(WindowModule* module)
		{
			ASSERT(!s_Module, "Window::Init was called more than once");
			s_Module = module;
		}

		/** @brief Centers the window */
		static void MakeCentered() { s_Module->MakeCentered(); }

		/** @brief Set/Toggle visiblity of the window
        * @param visible Wether or not to make the window visible
        * @param toggle  Ignore the @a visible param and toggle the current state of visibility */
		static void SetVisibility(bool visible, bool toggle = false) { s_Module->SetVisibility(visible, toggle); }

		/** @return Position of the monitor on the virtual desktop in screen coordinates */
		static inline std::pair<int32_t, int32_t> GetMonitorPosition() { return s_Module->GetMonitorPosition(); }

		/** @return Size of the video mode in screen coordinates */
		static inline std::pair<int32_t, int32_t> GetMonitorSize() { return s_Module->GetMonitorSize(); }

		/** @return Size of the content area(not framebuffer) of the window in screen coordinates */
		static inline std::pair<int32_t, int32_t> GetWindowSize() { return s_Module->GetWindowSize(); }

		/** @return Position of the upper-left corner of the content area in screen coordinates */
		static inline std::pair<int32_t, int32_t> GetWindowPosition() { return s_Module->GetWindowPosition(); }

	private:
		static WindowModule* s_Module;
	};

} // namespace Light
