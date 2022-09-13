#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <utility>
#include <vector>

#include "Base.hpp"

class GLFWwindow;

namespace Light {

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

	// \brief
	class WindowModule : public Module
	{
	public:
		WindowModule();
		~WindowModule();

		virtual bool HasRequestedAppTermination() const final override;

		virtual void OnConfig() final override {}
		virtual void OnInit() final override;
		virtual void OnUpdate() final override;
		virtual void OnDeinit() final override {}

		void BindEvents();

		////////////////////////////////////////////////////////////////
		/// API Functions
		void MakeCentered();

		void SetVisibility(bool visible, bool toggle = false);

		std::pair<int32_t, int32_t> GetMonitorPosition() const;
		std::pair<int32_t, int32_t> GetMonitorSize() const;

		std::pair<int32_t, int32_t> GetWindowSize() const;
		std::pair<int32_t, int32_t> GetWindowPosition() const;

		inline bool IsVisible() const { return m_Visible; }

	private:
		WindowModuleConfig m_Config = {};

		bool m_Visible;

		GLFWwindow* m_Handle = {};
	};

	class Window
	{
	public:
		Window()  = delete;
		~Window() = delete;

		static void Init(WindowModule* module) { s_Module = module; }

		static void MakeCentered() { s_Module->MakeCentered(); }

		static void SetVisibility(bool visible, bool toggle = false) { s_Module->SetVisibility(visible, toggle); }

		static inline std::pair<int32_t, int32_t> GetMonitorPosition() { return s_Module->GetMonitorPosition(); }

		static inline std::pair<int32_t, int32_t> GetMonitorSize() { return s_Module->GetMonitorSize(); }

		static inline std::pair<int32_t, int32_t> GetWindowSize() { return s_Module->GetWindowSize(); }

		static inline std::pair<int32_t, int32_t> GetWindowPosition() { return s_Module->GetWindowPosition(); }

	private:
		static WindowModule* s_Module;
	};

} // namespace Light
