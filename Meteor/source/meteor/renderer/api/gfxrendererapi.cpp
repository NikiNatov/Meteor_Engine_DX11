#include "pch.h"
#include "gfxrendererapi.h"

#include "platform\dx11\dx11rendererapi.h"

namespace meteor
{
    ScopedPtr<RendererAPI>  RendererAPI::ms_Instance = nullptr;
    RendererAPI::API        RendererAPI::ms_API = RendererAPI::API::DirectX11;

    // --------------------------------------------------------------------------------------------------------------------------------------
    void RendererAPI::Initialize()
    {
        switch (ms_API)
        {
            case RendererAPI::API::DirectX11:    ms_Instance = CreateScoped<DX11RendererAPI>(); break;
            default:                             METEOR_ENGINE_ASSERT(false, "Unknown API"); return;
        }

        ms_Instance->InitializeInternal();
    }
}