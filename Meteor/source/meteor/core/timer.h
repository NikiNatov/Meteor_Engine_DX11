#pragma once

#include "base.h"
#include "timestep.h"
#include <chrono>

namespace meteor
{
	class Timer
	{
	public:
		Timer();

		void		Reset();
		void		Stop();
		Timestep	GetTimestep();
	private:
		std::chrono::steady_clock::time_point m_Last;
		Timestep m_Timestep;
	};
}