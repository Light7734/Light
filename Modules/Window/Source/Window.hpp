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


	public:
		/** @brief Facade of the WindowModule */
		class Facade
		{
			friend WindowModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

			/// @todo Docs
			STATIC_NOTIF_BINDING(MouseMove, self->m_Notifiers.mouseMove, 1, std::pair<double, double>);
			STATIC_NOTIF_BINDING(MouseButton, self->m_Notifiers.mouseButton, 3, int32_t, int32_t, int32_t);
			STATIC_NOTIF_BINDING(MouseScroll, self->m_Notifiers.mouseScroll, 1, std::pair<double, double>);
			STATIC_NOTIF_BINDING(MouseEnter, self->m_Notifiers.mouseEnter, 1, int32_t);
			STATIC_NOTIF_BINDING(Key, self->m_Notifiers.key, 4, int32_t, int32_t, int32_t, int32_t);
			STATIC_NOTIF_BINDING(Char, self->m_Notifiers.character, 1, uint32_t);
			STATIC_NOTIF_BINDING(WindowMove, self->m_Notifiers.windowMove, 1, std::pair<int32_t, int32_t>);
			STATIC_NOTIF_BINDING(WindowResize, self->m_Notifiers.windowResize, 1, std::pair<int32_t, int32_t>);
			STATIC_NOTIF_BINDING(WindowFocus, self->m_Notifiers.windowFocus, 1, int32_t);
			STATIC_NOTIF_BINDING(WindowClose, self->m_Notifiers.windowClose, 0);
			STATIC_NOTIF_BINDING(FileDrop, self->m_Notifiers.fileDrop, 2, int, const char**);

			/** @brief Centers the window */
			static void MakeCentered();

			/** @brief Set/Toggle visiblity of the window
            * @param visible Wether or not to make the window visible
            * @param toggle  Ignore the @a visible param and toggle the current state of visibility */
			static void SetVisibility(bool visible, bool toggle = false);

			/** @return Position of the monitor on the virtual desktop in screen coordinates */
			static std::pair<int32_t, int32_t> GetMonitorPosition();

			/** @return Size of the video mode in screen coordinates */
			static std::pair<int32_t, int32_t> GetMonitorSize();

			/** @return Size of the content area(not framebuffer) of the window in screen coordinates */
			static std::pair<int32_t, int32_t> GetWindowSize();

			/** @return Position of the upper-left corner of the content area in screen coordinates */
			static std::pair<int32_t, int32_t> GetWindowPosition();

		private:
			static WindowModule* self;
		};
	};

	using Window = WindowModule::Facade;

} // namespace Light
