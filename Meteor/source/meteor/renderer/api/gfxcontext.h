#pragma once

#include "meteor\core\base.h"

#include "meteor\renderer\api\gfxdevice.h"
#include "meteor\renderer\api\gfxswapchain.h"

namespace meteor
{
    class GfxContext
    {
    public:
        virtual ~GfxContext() = default;

        inline static void                          SetVertexBuffer(Ref<VertexBuffer> vertexBuffer) { ms_Instance->SetVertexBufferInternal(vertexBuffer); }
        inline static void                          SetIndexBuffer(Ref<IndexBuffer> indexBuffer) { ms_Instance->SetIndexBufferInternal(indexBuffer); }
        inline static void                          SetShader(Ref<Shader> shader) { ms_Instance->SetShaderInternal(shader); }

        inline static const Ref<VertexBuffer>&      GetActiveVertexBuffer() { return ms_Instance->GetActiveVertexBufferInternal(); }
        inline static const Ref<IndexBuffer>&       GetActiveIndexBuffer() { return ms_Instance->GetActiveIndexBufferInternal(); }
        inline static const Ref<Shader>&            GetActiveShader() { return ms_Instance->GetActiveShaderInternal(); }

        inline static Ref<GfxDevice>                GetDevice() { return ms_Instance->GetDeviceInternal(); }
        inline static Ref<GfxSwapChain>             GetSwapChain() { return ms_Instance->GetSwapChainInternal(); }

        static void                                 CreateContext(const SwapChainDescription& swapChainDesc, void* windowHandle);
    protected:
        virtual void                                SetVertexBufferInternal(Ref<VertexBuffer> vertexBuffer) = 0;
        virtual void                                SetIndexBufferInternal(Ref<IndexBuffer> indexBuffer) = 0;
        virtual void                                SetShaderInternal(Ref<Shader> shader) = 0;

        virtual const Ref<VertexBuffer>&            GetActiveVertexBufferInternal() const = 0;
        virtual const Ref<IndexBuffer>&             GetActiveIndexBufferInternal() const = 0;
        virtual const Ref<Shader>&                  GetActiveShaderInternal() const = 0;

        virtual Ref<GfxDevice>                      GetDeviceInternal() = 0;
        virtual Ref<GfxSwapChain>                   GetSwapChainInternal() = 0;
    protected:
        static ScopedPtr<GfxContext>                ms_Instance;
    };
}