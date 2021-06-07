#pragma once

#if defined(METEOR_PLATFORM_WINDOWS)
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <shellapi.h>
#endif

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <unordered_set>
#include <string>
#include <functional>
#include <sstream>
#include <chrono>
#include <mutex>
#include <thread>