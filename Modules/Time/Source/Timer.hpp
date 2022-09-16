#pragma once

#include <chrono>

namespace Light {

	using namespace std::chrono;

	/** @brief Tells the elapsed time from construction/reset */
	class Timer
	{
	public:
		Timer()
		    : m_Start(steady_clock::now())
		{
		}

		/** @brief Restarts the timer to now */
		inline void Reset() { m_Start = steady_clock::now(); }

		/** @return Elapsed time from construction/reset to now */
		inline double ElapsedTime() const { return duration_cast<milliseconds>(steady_clock::now() - m_Start).count() / 1000.0; }

	private:
		time_point<steady_clock> m_Start;
	};

} // namespace Light
