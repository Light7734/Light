#pragma once

#include "Timer.hpp"

#include <Mojula/Module.hpp>

#include "Base.hpp"

namespace Light {

	class TimeModule : public Module
	{
	public:
		TimeModule()
		    : Module(MODULE_NAME, MODULE_UUID, {}, true) {}

		virtual ~TimeModule() {}

		virtual void OnConfig() final override {}
		virtual void OnInit() final override {}

		virtual void OnUpdate() final override
		{
			static bool firstFrame = true;

			m_DeltaTime = firstFrame ? 1.0 / 60.0 : m_Timer.ElapsedTime();
			firstFrame  = false;

			m_Timer.Reset();
		}

		virtual void OnDeinit() final override {}

		////////////////////////////////////////////////////////////////
		//// API Functions
		inline double GetDeltaTime() const { return m_DeltaTime; }

	private:
		Timer m_Timer;
		double m_DeltaTime;
	};

	class Time
	{
	public:
		Time()  = delete;
		~Time() = delete;

		static void Init(TimeModule* module) { s_Module = module; }

		static inline uint32_t SinceEpoch() { return time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count(); }

		static inline double GetDeltaTime() { return s_Module->GetDeltaTime(); }

	private:
		static TimeModule* s_Module;
	};

} // namespace Light
