#pragma once

#if defined(METEOR_PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <shellapi.h>

	#if defined(min)
	#undef min
	#endif

	#if defined(max)
	#undef max
	#endif
#endif