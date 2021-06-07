#include "pch.h"
#include "gfxcontext.h"

#include "meteor\renderer\api\gfxrendererapi.h"

#include "platform\dx11\dx11context.h"

namespace meteor
{
	ScopedPtr<GfxContext>	GfxContext::ms_Instance = nullptr;

	// --------------------------------------------------------------------------------------------------------------------------------------
    void GfxContext::CreateContext(const SwapChainDescription& swapChainDesc, void* windowHandle)
    {
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::DirectX11:	ms_Instance = CreateScoped<DX11Context>(swapChainDesc, windowHandle); return;
		}

		METEOR_ENGINE_ASSERT(false, "Unknown API");
    }
}