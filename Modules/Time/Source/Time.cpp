#include "Time.hpp"

namespace Light {

	TimeModule* Time::s_Module = nullptr;

	TimeModule::TimeModule()
	    : Module(true)
	{
	}

	TimeModule::~TimeModule()
	{
	}

	void TimeModule::OnConfig()
	{
	}

	void TimeModule::OnInit()
	{
	}

	void TimeModule::OnUpdate()
	{
		static bool firstFrame = true;

		m_DeltaTime = firstFrame ? 1.0 / 60.0 : m_Timer.ElapsedTime();
		firstFrame  = false;

		m_Timer.Reset();
	}

	void TimeModule::OnDeinit()
	{
	}

} // namespace Light
