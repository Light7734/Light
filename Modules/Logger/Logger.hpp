#pragma once

#include "Source/Logger.hpp"

#define LOG(category, log_level, ...) SPDLOG_LOGGER_CALL(::Light::Logger::ModuleAPI::GetLogger(#category), spdlog::level::log_level, __VA_ARGS__)
