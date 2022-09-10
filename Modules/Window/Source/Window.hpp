#include "Logger/Module.hpp"
#include "Mojula/Module.hpp"

#include <utility>
#include <vector>

#include "Base.hpp"

class GLFWwindow;

namespace Light { namespace Window {

	struct ModuleConfig
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
	class Module : public Mojula::Module
	{
	public:
		Module();
		~Module();

		virtual bool HasRequestedAppTermination() const final override;

		virtual void StoreAPI(uint64_t module_uuid, class Mojula::ModuleAPI* api) final override;

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
		Window::ModuleConfig m_Config = {};

		Logger::ModuleAPI* m_LoggerAPI = {};

		bool m_Visible;

		GLFWwindow* m_Handle = {};
	};

	class ModuleAPI : public Mojula::ModuleAPI
	{
	public:
		ModuleAPI(Module* module)
		    : m_Module(module) {}

		void MakeCentered() { m_Module->MakeCentered(); }

		void SetVisibility(bool visible, bool toggle = false) { m_Module->SetVisibility(visible, toggle); }

		inline std::pair<int32_t, int32_t> GetMonitorPosition() const { return m_Module->GetMonitorPosition(); }

		inline std::pair<int32_t, int32_t> GetMonitorSize() const { return m_Module->GetMonitorSize(); }

		inline std::pair<int32_t, int32_t> GetWindowSize() const { return m_Module->GetWindowSize(); }

		inline std::pair<int32_t, int32_t> GetWindowPosition() const { return m_Module->GetWindowPosition(); }

	private:
		Module* m_Module;
	};

}} // namespace Light::Window
