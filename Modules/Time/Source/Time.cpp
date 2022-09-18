#include "Time.hpp"

namespace Light {

	TimeModule* Time::self = nullptr;

	TimeModule::TimeModule()
	    : Module(true)
	{
		Time::self = this;
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
