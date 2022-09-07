#include "Logger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Light { namespace Logger {

	Module* ModuleAPI::s_Module = nullptr;

	void Module::CreateCategory(const char* category, const char* pattern)
	{
		std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt(category);
		logger->set_pattern(pattern);
		m_Loggers[category] = logger;
	}

}} // namespace Light::Logger
