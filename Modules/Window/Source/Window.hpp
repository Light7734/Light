#include "Events/Module.hpp"
#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <utility>
#include <variant>
#include <vector>

class GLFWwindow;

namespace Light {

	using namespace std::placeholders;

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
    * @todo Multi-window support [ ? ]
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
		/// Facade: Event Bindings
		template<typename T>
		inline void Bind_MouseMove(bool (T::*func)(std::pair<double, double>), T* instance)
		{
			m_Notifiers.mouseMove.AddListener(std::bind(func, instance, std::placeholders::_1));
		}

		template<typename T>
		inline void Bind_MouseButton(bool (T::*func)(int32_t, int32_t, int32_t), T* instance)
		{
			m_Notifiers.mouseButton.AddListener(std::bind(func, instance, _1, _2, _3));
		}

		template<typename T>
		inline void Bind_MouseScroll(bool (T::*func)(std::pair<double, double>), T* instance)
		{
			m_Notifiers.mouseScroll.AddListener(std::bind(func, instance, _1));
		}

		template<typename T>
		inline void Bind_MouseEnter(bool (T::*func)(int32_t), T* instance)
		{
			m_Notifiers.mouseEnter.AddListener(std::bind(func, instance, _1));
		}

		template<typename T>
		inline void Bind_Key(bool (T::*func)(int32_t, int32_t, int32_t, int32_t), T* instance)
		{
			m_Notifiers.key.AddListener(std::bind(func, instance, _1, _2, _3, _4));
		}

		template<typename T>
		inline void Bind_Char(bool (T::*func)(uint32_t), T* instance)
		{
			m_Notifiers.character.AddListener(std::bind(func, instance, _1));
		}

		template<typename T>
		inline void Bind_WindowMove(bool (T::*func)(std::pair<int32_t, int32_t>), T* instance)
		{
			m_Notifiers.windowMove.AddListener(std::bind(func, instance, _1));
		}

		template<typename T>
		inline void Bind_WindowResize(bool (T::*func)(std::pair<int32_t, int32_t>), T* instance)
		{
			m_Notifiers.windowResize.AddListener(std::bind(func, instance, _1));
		}

		template<typename T>
		inline void Bind_WindowFocus(bool (T::*func)(int32_t), T* instance)
		{
			m_Notifiers.windowFocus.AddListener(std::bind(func, instance, _1));
		}

		template<typename T>
		inline void Bind_WindowClose(bool (T::*func)(), T* instance)
		{
			m_Notifiers.windowClose.AddListener(std::bind(func, instance));
		}

		template<typename T>
		inline void Bind_FileDrop(bool (T::*func)(int, const char**), T* instance)
		{
			m_Notifiers.fileDrop.AddListener(std::bind(func, instance, _1, _2));
		}

		////////////////////////////////////////////////////////////////
		/// Facade Functions
		void MakeCentered();

		bool Test(std::pair<double, double> pos);

		void SetVisibility(bool visible, bool toggle = false);

		std::pair<int32_t, int32_t> GetMonitorPosition() const;
		std::pair<int32_t, int32_t> GetMonitorSize() const;

		std::pair<int32_t, int32_t> GetWindowSize() const;
		std::pair<int32_t, int32_t> GetWindowPosition() const;

		inline bool IsVisible() const
		{
			return m_Visible;
		}

	private:
		void BindGlfwCallbacks();

	private:
		WindowModuleConfig m_Config = {};

		GLFWwindow* m_Handle = {};
		bool m_Visible       = {};

		// Notifiers need to be inside a struct to be ezly accessed by window user pointer
		struct NotifierList
		{
			Notifier<std::pair<double, double>> mouseMove   = {};
			Notifier<int32_t, int32_t, int32_t> mouseButton = {};
			Notifier<std::pair<double, double>> mouseScroll = {};
			Notifier<int> mouseEnter                        = {};

			Notifier<int, int, int, int> key = {};
			Notifier<uint32_t> character     = {};

			Notifier<std::pair<int32_t, int32_t>> windowMove   = {};
			Notifier<std::pair<int32_t, int32_t>> windowResize = {};
			Notifier<int32_t> windowFocus                      = {};
			Notifier<> windowClose                             = {};

			Notifier<int, const char**> fileDrop = {};
		} m_Notifiers = {};
	};

	/** @brief Facade of the WindowModule */
	class Window
	{
	public:
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

		////////////////////////////////////////////////////////////////
		/// Event Bindings
		/// @todo Docs
		template<typename T>
		static inline void Bind_MouseMove(bool (T::*func)(std::pair<double, double>), T* instance) { s_Module->Bind_MouseMove<T>(func, instance); }

		template<typename T>
		static inline void Bind_MouseButton(bool (T::*func)(int32_t, int32_t, int32_t), T* instance) { s_Module->Bind_MouseButton<T>(func, instance); }

		template<typename T>
		static inline void Bind_MouseScroll(bool (T::*func)(std::pair<double, double>), T* instance) { s_Module->Bind_MouseScroll<T>(func, instance); }

		template<typename T>
		static inline void Bind_MouseEnter(bool (T::*func)(int32_t), T* instance) { s_Module->Bind_MouseEnter<T>(func, instance); }

		template<typename T>
		static inline void Bind_Key(bool (T::*func)(int32_t, int32_t, int32_t, int32_t), T* instance) { s_Module->Bind_Key<T>(func, instance); }

		template<typename T>
		static inline void Bind_Char(bool (T::*func)(uint32_t), T* instance) { s_Module->Bind_Char<T>(func, instance); }

		template<typename T>
		static inline void Bind_WindowMove(bool (T::*func)(std::pair<int32_t, int32_t>), T* instance) { s_Module->Bind_WindowMove<T>(func, instance); }

		template<typename T>
		static inline void Bind_WindowResize(bool (T::*func)(std::pair<int32_t, int32_t>), T* instance) { s_Module->Bind_WindowResize<T>(func, instance); }

		template<typename T>
		static inline void Bind_WindowFocus(bool (T::*func)(int32_t), T* instance) { s_Module->Bind_WindowFocus<T>(func, instance); }

		template<typename T>
		static inline void Bind_WindowClose(bool (T::*func)(), T* instance) { s_Module->Bind_WindowClose<T>(func, instance); }

		template<typename T>
		static inline void Bind_FileDrop(bool (T::*func)(int, const char**), T* instance) { s_Module->Bind_FileDrop<T>(func, instance); }

		/** @brief Centers the window */
		static inline void MakeCentered() { s_Module->MakeCentered(); }

		/** @brief Set/Toggle visiblity of the window
        * @param visible Wether or not to make the window visible
        * @param toggle  Ignore the @a visible param and toggle the current state of visibility */
		static inline void SetVisibility(bool visible, bool toggle = false) { s_Module->SetVisibility(visible, toggle); }

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
