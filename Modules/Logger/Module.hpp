#pragma once

#include "Source/Logger.hpp"

#define LOG(category, log_level, ...) ::Light::Logger::ModuleAPI::GetLogger(#category)->log(spdlog::level::log_level, __VA_ARGS__)
