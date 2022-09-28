#include "Logger.hpp"

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Light {

	LoggerModule* Logger::self = {};

	LoggerModule::LoggerModule()
	{
		Logger::self = this;
	}

	LoggerModule::~LoggerModule()
	{
		Logger::self = {};
	}

	void LoggerModule::OnTick()
	{
	}

	void LoggerModule::OnSync()
	{
	}

	std::shared_ptr<spdlog::logger> LoggerModule::Facade::CreateCategory(const LoggerCategoryCreateInfo& info)
	{
		spdlog::set_level(spdlog::level::trace);
		std::shared_ptr<spdlog::logger> logger;

		switch (info.type)
		{
		case LoggerType::eStdoutColor:
			logger = spdlog::stdout_color_mt(info.name);
			break;
		case LoggerType::eFileAsync:
			logger = spdlog::basic_logger_mt<spdlog::async_factory>(info.name, info.outputFile);
			break;

		default: ASSERT(false, "Failed to create spdlog::logger ; unknown logger type");
		}

		logger->set_pattern(info.pattern);
		self->m_Loggers[info.name] = logger;

		return logger;
	}

} // namespace Light
