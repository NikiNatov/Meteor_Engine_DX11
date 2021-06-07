#pragma once

#ifdef METEOR_PLATFORM_WINDOWS
	#include "platform/windows/win.h"
#endif // METEOR_PLATFORM_WINDOWS

#include "base.h"

namespace meteor 
{
	enum class MouseButton : mtrInt
	{
		#ifdef METEOR_PLATFORM_WINDOWS

		Left	= VK_LBUTTON,
		Right	= VK_RBUTTON,
		Middle	= VK_MBUTTON,
		X1		= VK_XBUTTON1,
		X2		= VK_XBUTTON2,

		#endif // METEOR_PLATFORM_WINDOWS

	};
}