#pragma once

#include "Exception/Module.hpp"
#include "Mojula/Module.hpp"

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

#include "Base.hpp"

namespace Light {

	enum class LogLevel : uint32_t
	{
		eTrace    = SPDLOG_LEVEL_TRACE,
		eDebug    = SPDLOG_LEVEL_DEBUG,
		eInfo     = SPDLOG_LEVEL_INFO,
		eWarn     = SPDLOG_LEVEL_WARN,
		eError    = SPDLOG_LEVEL_ERROR,
		eCritical = SPDLOG_LEVEL_CRITICAL,
		eOff      = SPDLOG_LEVEL_OFF,

		nCount
	};

	enum class LogType : uint32_t
	{
		eStdoutColor,
		eFile,
		eFileAsync,

		nCount,
	};

	struct LoggerCategoryCreateInfo
	{
		const char* name;
		const char* pattern;
		LogType type;
		const char* outputFile;
	};

	class LoggerModule : public Module
	{
	public:
		LoggerModule()
		    : Module(MODULE_NAME, MODULE_UUID, {}, false)
		{
		}

		virtual ~LoggerModule() {}

		////////////////////////////////////////////////////////////////////////////////
		// Mojula::Module Interface
		virtual void OnConfig() final override {}
		virtual void OnInit() final override {}
		virtual void OnUpdate() final override {}
		virtual void OnDeinit() final override {}

		////////////////////////////////////////////////////////////////////////////////
		// To be used by other modules
		void CreateCategory(const LoggerCategoryCreateInfo& info);

		inline std::shared_ptr<spdlog::logger> GetLogger(const char* category) { return m_Loggers[category]; }

	private:
		std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_Loggers;
	};

	class Logger
	{
	public:
		Logger()  = delete;
		~Logger() = delete;

		static void Init(LoggerModule* loggerModule)
		{
			s_Module = loggerModule;
		}

		static inline std::shared_ptr<spdlog::logger> CreateCategory(const LoggerCategoryCreateInfo& info)
		{
			s_Module->CreateCategory(info);
			return s_Module->GetLogger(info.name);
		}

		template<typename... Args>
		static void Log(const char* category, LogLevel level, spdlog::format_string_t<Args...> fmt, Args&&... targs)
		{
			s_Module->GetLogger(category)->log(spdlog::level::trace, fmt, std::forward<Args>(targs)...);
		}

		static inline std::shared_ptr<spdlog::logger> GetLogger(const char* category) { return s_Module->GetLogger(category); }

	private:
		static LoggerModule* s_Module;
	};

} // namespace Light
