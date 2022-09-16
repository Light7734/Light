#pragma once

#include "Exception/Module.hpp"
#include "Mojula/Module.hpp"
#include "Timer.hpp"

namespace Light {

	/** @brief Keeps track of the time related things */
	class TimeModule final : public Module
	{
	public:
		TimeModule();
		virtual ~TimeModule() override;


		////////////////////////////////////////////////////////////////
		/// Module Interface
		virtual void OnConfig() override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void OnDeinit() override;

		////////////////////////////////////////////////////////////////
		//// Facade Fuctions
		inline double GetDeltaTime() const { return m_DeltaTime; }

	private:
		Timer m_Timer;
		double m_DeltaTime;
	};

	/** @brief Facade of the TimeModule */
	class Time
	{
	public:
		Time()  = delete;
		~Time() = delete;

		/** @brief Initialize the facade with the actual module 
         * @note Do not manually call this */
		static void Init(TimeModule* module)
		{
			ASSERT(!s_Module, "Time::Init was called more than once");
			s_Module = module;
		}

		/** @return Elapsed time since epoch */
		static inline uint32_t SinceEpoch() { return time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count(); }

		/** @return Frame delta time */
		static inline double GetDeltaTime() { return s_Module->GetDeltaTime(); }

	private:
		static TimeModule* s_Module;
	};

} // namespace Light
