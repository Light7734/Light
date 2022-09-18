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

	private:
		Timer m_Timer;
		double m_DeltaTime;

	public:
		/** @brief Facade of the TimeModule */
		class Facade
		{
			friend TimeModule;

		public:
			Facade()  = delete;
			~Facade() = delete;

			/** @return Elapsed time since epoch */
			static inline uint32_t SinceEpoch() { return time_point_cast<microseconds>(steady_clock::now()).time_since_epoch().count(); }

			/** @return Frame delta time */
			static inline double GetDeltaTime() { return self->m_DeltaTime; }

		private:
			static TimeModule* self;
		};
	};

	using Time = TimeModule::Facade;

} // namespace Light
