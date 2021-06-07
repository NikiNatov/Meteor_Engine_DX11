#pragma once

#if defined(METEOR_PLATFORM_WINDOWS)
#include "meteor\core\base.h"
#include "dxerror\dxerr.h"

#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

namespace meteor
{
	#define DX_CALL(hr) \
		if (FAILED(hr))\
		{\
			METEOR_ENGINE_LOG_ERROR("[File]: {0}", __FILE__);\
			METEOR_ENGINE_LOG_ERROR("[Line]: {0}", __LINE__);\
			METEOR_ENGINE_LOG_ERROR("[Error]: {0}", DXGetErrorString(hr));\
			mtrChar errDescription[256];\
			DXGetErrorDescription(hr, errDescription, 256);\
			METEOR_ENGINE_LOG_ERROR("[Description]: {0}", errDescription);\
			__debugbreak();\
		}
}
#endif // METEOR_PLATFORM_WINDOWS