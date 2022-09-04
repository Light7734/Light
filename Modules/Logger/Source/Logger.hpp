#pragma once

#include "Mojula.hpp"

#include <iostream>
#include <stdexcept>

namespace Light { namespace Logger {

	struct LoggerData: public Mojula::ModuleData
	{
	};

	class Logger: public Mojula::Module
	{
	public:
		Logger()
		    : Module("Logger", 69ull, {}, nullptr /* test if we can pass data right here */, false)
		{
			if (s_Instance)
			{
				throw std::runtime_error("Something's wrong, I can feel it!");
			}

			s_Instance = this;

			m_ExposedData = &m_LoggerData;
		}

		virtual ~Logger()
		{
		}

		virtual void OnBirth() final override
		{
			std::cout << "Logger on birth\n";
		}

		virtual void OnDeath() final override
		{
			std::cout << "Logger on death\n";
		}

		virtual void Tick() final override {}

		virtual void StoreDependencyDataPointer(uint64_t module_uuid, Mojula::ModuleData* dependency_data) final override {}

	private:
		static Logger* s_Instance;
		LoggerData m_LoggerData = {};
	};

}} // namespace Light::Logger
