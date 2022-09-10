#pragma once

#include "Mojula/Module.hpp"

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#include "Base.hpp"

namespace Light { namespace Logger {

	struct ModuleConfig
	{
		spdlog::level::level_enum logLevel;
	};

	class Module : public Mojula::Module
	{
	public:
		Module()
		    : Mojula::Module(MODULE_NAME, MODULE_UUID, {}, false)
		{
		}

		virtual ~Module() {}

		////////////////////////////////////////////////////////////////////////////////
		// Mojula::Module Interface

		virtual void OnConfig() final override {}
		virtual void OnInit() final override {}
		virtual void OnUpdate() final override {}
		virtual void OnDeinit() final override {}

		virtual void StoreAPI(uint64_t moduleUUID, Mojula::ModuleAPI* dependency_api) final override {}

		////////////////////////////////////////////////////////////////////////////////
		// API Functions ; to be used by other modules
		void CreateCategory(const char* category, const char* pattern);

		std::shared_ptr<spdlog::logger> GetLogger(const char* category)
		{
			return m_Loggers[category];
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_Loggers;
	};

	class ModuleAPI : public Mojula::ModuleAPI
	{
	public:
		ModuleAPI(Module* module)
		    : m_Module(module) { s_Module = m_Module; }

		inline void CreateCategory(const char* category, const char* pattern = "%^[%l][%H:%M:%S @ %n] => %v%$") { m_Module->CreateCategory(category, pattern); }

		static inline std::shared_ptr<spdlog::logger> GetLogger(const char* category) { return s_Module->GetLogger(category); }

	private:
		static Module* s_Module; // !< static flavor of the module to make logging macros possible
		Module* m_Module;        // !< the module
	};

}} // namespace Light::Logger
