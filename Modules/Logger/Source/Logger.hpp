#pragma once

#include "Exception/Module.hpp"
#include "Mojula/Module.hpp"

#include <spdlog/spdlog.h>

#include <memory>
#include <string>

/** @def LOGGER_DEFAULT_PATTERN  
 * The default logging pattern: [severity][hour:minute:second @ category] => message
 */
#define LOGGER_DEFAULT_PATTERN "%^[%l][%H:%M:%S @ %n] => %v%$"

/** @def LOG
 * Logs formatted string to the console
 * @param category The category of the log
 * @param log_level a ::Light::LogLvl value ; the severity of the log
 * @param ... Variadic arguments to be logged
 * @note @a category should be log category created witihn the invoking module */
#define LOG(category, log_level, ...) ::Light::Logger::Log(category, log_level, __VA_ARGS__)

namespace Light {

	/** @brief Severity of the log message */
	enum class LogLvl : uint32_t
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

	/** @brief Type of the logger */
	enum class LoggerType : uint32_t
	{
		eStdoutColor,
		eFile,
		eFileAsync,

		nCount,
	};

	struct LoggerCategoryCreateInfo
	{
		const char* name;       /** name of the category */
		const char* pattern;    /** log message formatting pattern */
		LoggerType type;        /** type of the logger */
		const char* outputFile; /** output file if type is a LoggerType::eFile* */
	};

	/** @brief Formats and logs messages to console/files */
	class LoggerModule : public Module
	{
	public:
		LoggerModule();

		virtual ~LoggerModule() override;

		////////////////////////////////////////////////////////////////////////////////
		// Module Interface
		virtual void OnConfig() override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void OnDeinit() override;

		////////////////////////////////////////////////////////////////////////////////
		// Facade Functions
		void CreateCategory(const LoggerCategoryCreateInfo& info);

		inline std::shared_ptr<spdlog::logger> GetLogger(const char* category) { return m_Loggers[category]; }

	private:
		std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> m_Loggers;

	public:
		/** @brief Facade of LoggerModule*/
		class Facade
		{
			friend LoggerModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

			/** @brief Creates a logging category that can be acessed using info.name in Logger functions 
             * @param info [self-explanatory] */
			static std::shared_ptr<spdlog::logger> CreateCategory(const LoggerCategoryCreateInfo& info);

			/** @brief Logs formatted string to the console */
			template<typename... Args>
			static inline void Log(const char* category, LogLvl level, spdlog::format_string_t<Args...> fmt, Args&&... targs)
			{
				self->m_Loggers[category]->log(spdlog::level::trace, fmt, std::forward<Args>(targs)...);
			}

			/** @return Logger named @a category */
			static inline std::shared_ptr<spdlog::logger> GetLogger(const char* category)
			{
				return self->m_Loggers[category];
			}

		private:
			static LoggerModule* self;
		};
	};

	using Logger = LoggerModule::Facade;

} // namespace Light
