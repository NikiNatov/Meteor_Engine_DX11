#pragma once

#include "base.h"

namespace meteor
{
	class Timestep
	{
	public:
		Timestep(mtrFloat time = 0.0f)
			: m_Time(time)
		{
		}

		inline mtrFloat GetSeconds() const { return m_Time; }
		inline mtrFloat GetMilliseconds() const { return m_Time * 1000.0f; }

		operator mtrFloat() const { return GetSeconds(); }
	private:
		mtrFloat m_Time;
	};
}