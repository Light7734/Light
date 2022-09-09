#pragma once

#include <chrono>

namespace Light { namespace Time {

	using namespace std::chrono;

	class Timer
	{
	public:
		Timer()
		    : m_Start(steady_clock::now())
		{
		}

		inline void Reset() { m_Start = steady_clock::now(); }

		inline double ElapsedTime() const { return duration_cast<milliseconds>(steady_clock::now() - m_Start).count() / 1000.0; }

	private:
		time_point<steady_clock> m_Start;
	};
}
} // namespace Light::Time
