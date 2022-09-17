#include "Events/Module.hpp"
#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <utility>
#include <variant>
#include <vector>

class GLFWwindow;

namespace Light {

	enum class WindowEventType : uint32_t
	{
		eMouseMove,
		eMouseButton,
		eMouseScroll,

		eKey,
		eChar,

		eWindowMove,
		eWindowResize,
		eWindowFocus,
		eWindowClose,

		nCount
	};

	/** @todo Docs */
	template<typename T>
	using WindowEvent = std::variant<
	    bool (T::*)(int32_t, int32_t, int32_t, int32_t),
	    bool (T::*)(int32_t, int32_t, int32_t),
	    bool (T::*)(std::pair<double, double>),
	    bool (T::*)(std::pair<int32_t, int32_t>),
	    bool (T::*)(int32_t),
	    bool (T::*)(uint32_t),
	    bool (T::*)()>;

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
		template<typename T>
		void AddEventListener(WindowEventType type, WindowEvent<T> event, T* instance)
		{
			using namespace std::placeholders;
			switch (type)
			{
			case WindowEventType::eMouseMove:
				m_Notifiers.mouseMove.AddListener(std::bind(std::get<bool (T::*)(std::pair<double, double>)>(event), instance, _1));
				break;

			case WindowEventType::eMouseButton:
				m_Notifiers.mouseButton.AddListener(std::bind(std::get<bool (T::*)(int32_t, int32_t, int32_t)>(event), instance, _1, _2, _3));
				break;

			case WindowEventType::eMouseScroll:
				m_Notifiers.mouseScroll.AddListener(std::bind(std::get<bool (T::*)(std::pair<double, double>)>(event), instance, _1));
				break;

			case WindowEventType::eKey:
				m_Notifiers.key.AddListener(std::bind(std::get<bool (T::*)(int32_t, int32_t, int32_t, int32_t)>(event), instance, _1, _2, _3, _4));
				break;

			case WindowEventType::eChar:
				m_Notifiers.key.AddListener(std::bind(std::get<bool (T::*)(uint32_t)>(event), instance, _1));
				break;

			case WindowEventType::eWindowMove:
				m_Notifiers.windowMove.AddListener(std::bind(std::get<bool (T::*)(std::pair<int32_t, int32_t>)>(event), instance, _1));
				break;

			case WindowEventType::eWindowResize:
				m_Notifiers.windowResize.AddListener(std::bind(std::get<bool (T::*)(std::pair<int32_t, int32_t>)>(event), instance, _1));
				break;

			case WindowEventType::eWindowFocus:
				m_Notifiers.windowFocus.AddListener(std::bind(std::get<bool (T::*)(int32_t)>(event), instance, _1));
				break;

			case WindowEventType::eWindowClose:
				m_Notifiers.windowClose.AddListener(std::bind(std::get<bool (T::*)()>(event), instance));
				break;

			default:
				ASSERT(false, "Invalid WindowEventType value");
			}
		}

		void MakeCentered();

		bool Test(std::pair<double, double> pos);

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

		GLFWwindow* m_Handle = {};
		bool m_Visible       = {};

		// Notifiers need to be inside a struct to be ezly accessed by window user pointer
		struct NotifierList
		{
			Notifier<std::pair<double, double>> mouseMove   = {};
			Notifier<int32_t, int32_t, int32_t> mouseButton = {};
			Notifier<std::pair<double, double>> mouseScroll = {};

			Notifier<int, int, int, int> key = {};
			Notifier<uint32_t> character     = {};

			Notifier<std::pair<int32_t, int32_t>> windowMove   = {};
			Notifier<std::pair<int32_t, int32_t>> windowResize = {};
			Notifier<int32_t> windowFocus                      = {};
			Notifier<> windowClose                             = {};
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

		template<typename T>
		static inline void AddEventListener(WindowEventType type, WindowEvent<T> event, T* instance) { s_Module->AddEventListener<T>(type, event, instance); }

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
