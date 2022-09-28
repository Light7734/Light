#include "Events/Module.hpp"
#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <utility>
#include <variant>
#include <vector>

class GLFWwindow;

namespace Light {

	using namespace std::placeholders;

	struct WindowEventList
	{
		std::vector<std::tuple<double, double>> mouseMove              = {};
		std::vector<std::tuple<int32_t, int32_t, int32_t>> mouseButton = {};
		std::vector<std::tuple<double, double>> mouseScroll            = {};
		std::vector<std::tuple<int>> mouseEnter                        = {};

		std::vector<std::tuple<int, int, int, int>> key = {};
		std::vector<std::tuple<uint32_t>> character     = {};

		std::vector<std::tuple<int32_t, int32_t>> windowMove   = {};
		std::vector<std::tuple<int32_t, int32_t>> windowResize = {};
		std::vector<std::tuple<int32_t>> windowFocus           = {};
		std::vector<std::tuple<>> windowClose                  = {};

		std::vector<std::tuple<int, const char**>> fileDrop = {};
	};

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

		virtual void OnTick() override;
		virtual void OnSync() override;

		inline bool IsVisible() const
		{
			return m_Visible;
		}

	private:
		void BindGlfwCallbacks();

	private:
		WindowModuleConfig m_Config = {};

		GLFWwindow* m_Handle {};
		bool m_Visible {};

		WindowEventList m_Events;

	public:
		/** @brief Facade of the WindowModule */
		class Facade
		{
			friend WindowModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

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

			static std::vector<std::tuple<double, double>> GetMouseMoveEvents();

			static std::vector<std::tuple<int32_t, int32_t, int32_t>> GetMouseButtonEvents();

			static std::vector<std::tuple<double, double>> GetMouseScrollEvents();

			static std::vector<std::tuple<int>> GetMouseEnterEvents();

			static std::vector<std::tuple<int, int, int, int>> GetKeyEvents();

			static std::vector<std::tuple<uint32_t>> GetCharacterEvents();

			static std::vector<std::tuple<int32_t, int32_t>> GetWindowMoveEvents();

			static std::vector<std::tuple<int32_t, int32_t>> GetWindowResizeEvents();

			static std::vector<std::tuple<int32_t>> GetWindowFocusEvents();

			static std::vector<std::tuple<>> GetWindowCloseEvents();

			static std::vector<std::tuple<int, const char**>> GetFileDropEvents();

		private:
			static WindowModule* self;
		};
	};

	using Window = WindowModule::Facade;

} // namespace Light
