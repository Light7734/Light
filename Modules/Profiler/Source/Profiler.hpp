#pragma once

#include "FileManager/Module.hpp"
#include "Mojula/Module.hpp"
#include "Time/Module.hpp"

#include <fstream>
#include <memory>
#include <ostream>

#define PROFILE_SCOPE(name)                    PROFILE_SCOPE_INDIRECTION(name, __LINE__)
#define PROFILE_SCOPE_INDIRECTION(name, line)  PROFILE_SCOPE_INDIRECTION2(name, line)
#define PROFILE_SCOPE_INDIRECTION2(name, line) ::Light::ScopeProfiler scope_profiler##line(name)

namespace Light {

	struct ScopeResult
	{
		const char* name;
		uint64_t start, duration;
		uint32_t threadID;
	};

	class ProfilerModule : public Module
	{
	public:
		virtual void OnConfig() final override {}
		virtual void OnInit() final override
		{
			m_OutputFile = FileManager::CreateTxt(Paths::GetLogsDir() / "ProfileResult.json");
			m_OutputFile << "{\"traceEvents\":[";

			m_OutputFile << "{";
			m_OutputFile << "\"name\":\"init\",";
			m_OutputFile << "\"cat\": \"scope\",";
			m_OutputFile << "\"ph\": \"X\",";
			m_OutputFile << "\"ts\":" << Time::SinceEpoch() << ",";
			m_OutputFile << "\"dur\":" << 0ull << ",";
			m_OutputFile << "\"pid\":0,";
			m_OutputFile << "\"tid\":" << 0ull << "";
			m_OutputFile << "}";
		}

		virtual void OnDeinit() final override
		{
			m_OutputFile << "]}";
			m_OutputFile.reset();
		}

		virtual void OnUpdate() final override {}

		void SubmitScopeResult(const ScopeResult& scope_result)
		{
			std::ostringstream sstream;
			m_OutputFile << ",{";
			m_OutputFile << "\"name\":\"" << scope_result.name << "\",";
			m_OutputFile << "\"cat\": \"scope\",";
			m_OutputFile << "\"ph\": \"X\",";
			m_OutputFile << "\"ts\":" << scope_result.start << ",";
			m_OutputFile << "\"dur\":" << scope_result.duration << ",";
			m_OutputFile << "\"pid\":0,";
			m_OutputFile << "\"tid\":" << scope_result.threadID << "";
			m_OutputFile << "}";
		}

	private:
		std::shared_ptr<TxtFile> m_OutputFile = {};
	};

	class Profiler
	{
	public:
		Profiler()  = delete;
		~Profiler() = delete;

		static void Init(ProfilerModule* module) { s_Module = module; }

		static inline void SubmitScopeResult(const ScopeResult& scope_result) { s_Module->SubmitScopeResult(scope_result); }

	private:
		static ProfilerModule* s_Module;
	};

	class ScopeProfiler
	{
	public:
		ScopeProfiler(const char* name)
		    : m_Name(name), m_Start(Time::SinceEpoch())
		{
		}

		~ScopeProfiler()
		{
			const uint64_t duration = Time::SinceEpoch() - m_Start;

			const ScopeResult result {
				m_Name,   // name
				m_Start,  // start
				duration, // duration
				{}        // threadID
			};

			Profiler::SubmitScopeResult(result);
		}

	private:
		const char* m_Name;
		const uint64_t m_Start;
	};

} // namespace Light
