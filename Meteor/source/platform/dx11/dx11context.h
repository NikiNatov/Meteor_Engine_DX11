#pragma once

#include "meteor\renderer\api\gfxcontext.h"

#include "dx11device.h"
#include "dx11swapchain.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    class DX11Context : public GfxContext
    {
    public:
        DX11Context(const SwapChainDescription& swapChainDesc, void* windowHandle);

    protected:
        virtual void                          SetVertexBufferInternal(Ref<VertexBuffer> vertexBuffer) override;
        virtual void                          SetIndexBufferInternal(Ref<IndexBuffer> indexBuffer) override;
        virtual void                          SetShaderInternal(Ref<Shader> shader) override;

        virtual const Ref<VertexBuffer>&      GetActiveVertexBufferInternal() const override { return m_ActiveVertexBuffer; }
        virtual const Ref<IndexBuffer>&       GetActiveIndexBufferInternal() const override { return m_ActiveIndexBuffer; }
        virtual const Ref<Shader>&            GetActiveShaderInternal() const override { return m_ActiveShader; }

        virtual Ref<GfxDevice>                GetDeviceInternal() override { return m_Device; }
        virtual Ref<GfxSwapChain>             GetSwapChainInternal() override { return m_SwapChain; }
    private:
        HWND                                  m_WindowHandle;
        Ref<DX11Device>                       m_Device;
        Ref<DX11SwapChain>                    m_SwapChain;

        Ref<VertexBuffer>                     m_ActiveVertexBuffer;
        Ref<IndexBuffer>                      m_ActiveIndexBuffer;
        Ref<Shader>                           m_ActiveShader;
    };

#endif// METEOR_PLATFORM_WINDOWS
}