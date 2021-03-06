#include "pch.h"
#include "timer.h"

namespace meteor
{
	// --------------------------------------------------------------------------------------------------------------------------------------
	Timer::Timer()
	{
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Timer::Reset()
	{
		m_Last = std::chrono::steady_clock::now();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void Timer::Stop()
	{
		auto now = std::chrono::steady_clock::now();
		std::chrono::duration<mtrFloat> frameTime = now - m_Last;
		m_Timestep = frameTime.count();
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Timestep Timer::GetTimestep()
	{
		return m_Timestep;
	}
}