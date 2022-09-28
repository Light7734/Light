#include "Time.hpp"

namespace Light {

	TimeModule* Time::self = nullptr;

	TimeModule::TimeModule()
	    : Module(Module::TickType::eGameThread)
	{
		Time::self = this;
	}

	TimeModule::~TimeModule()
	{
	}

	void TimeModule::OnTick()
	{
		static bool firstFrame = true;

		m_DeltaTime = firstFrame ? 1.0 / 60.0 : m_Timer.ElapsedTime();
		firstFrame  = false;

		m_Timer.Reset();
	}

	void TimeModule::OnSync()
	{
	}

} // namespace Light
