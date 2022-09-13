#include "Logger.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Light {

	LoggerModule* Logger::s_Module = nullptr;

	void LoggerModule::CreateCategory(const LoggerCategoryCreateInfo& info)
	{
		spdlog::set_level(spdlog::level::trace);
		std::shared_ptr<spdlog::logger> logger;

		switch (info.type)
		{
		case LogType::eStdoutColor:
			logger = spdlog::stdout_color_mt(info.name);
			break;
		case LogType::eFileAsync:
			logger = spdlog::basic_logger_mt<spdlog::async_factory>(info.name, info.outputFile);
			break;

		default: ASSERT(false, "Failed to create spdlog::logger ; unknown logger type");
		}

		logger->set_pattern(info.pattern);
		m_Loggers[info.name] = logger;
	}

} // namespace Light
