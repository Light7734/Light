#include "Profiler.hpp"

namespace Light {

	ProfilerModule* Profiler::s_Module = {};

	void ProfilerModule::OnConfig()
	{
	}

	void ProfilerModule::OnInit()
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

	void ProfilerModule::OnUpdate()
	{
	}

	void ProfilerModule::OnDeinit()
	{
		m_OutputFile << "]}";
		m_OutputFile.reset();
	}

	void ProfilerModule::SubmitScopeResult(const ScopeResult& scope_result)
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

	ScopeProfiler::ScopeProfiler(const char* name)
	    : m_Name(name), m_Start(Time::SinceEpoch())
	{
	}

	ScopeProfiler::~ScopeProfiler()
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
} // namespace Light
