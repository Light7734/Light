#include "Profiler.hpp"

namespace Light {

	ProfilerModule* Profiler::self = {};


	ProfilerModule::ProfilerModule()
	{
		Profiler::self = this;

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

	ProfilerModule::~ProfilerModule()
	{
		m_OutputFile << "]}";
		m_OutputFile.reset();

		Profiler::self = {};
	}

	void ProfilerModule::OnTick()
	{
	}

	void ProfilerModule::OnSync()
	{
	}

	void ProfilerModule::Facade::SubmitScopeResult(const ScopeResult& scope_result)
	{
		std::ostringstream sstream;
		self->m_OutputFile << ",{";
		self->m_OutputFile << "\"name\":\"" << scope_result.name << "\",";
		self->m_OutputFile << "\"cat\": \"scope\",";
		self->m_OutputFile << "\"ph\": \"X\",";
		self->m_OutputFile << "\"ts\":" << scope_result.start << ",";
		self->m_OutputFile << "\"dur\":" << scope_result.duration << ",";
		self->m_OutputFile << "\"pid\":0,";
		self->m_OutputFile << "\"tid\":" << scope_result.threadID << "";
		self->m_OutputFile << "}";
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
