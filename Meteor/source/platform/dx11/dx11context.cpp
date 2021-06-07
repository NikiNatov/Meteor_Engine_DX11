#include "pch.h"
#include "dx11context.h"

#include "meteor/renderer/api/gfxbuffer.h"
#include "meteor/renderer/api/gfxshader.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    // --------------------------------------------------------------------------------------------------------------------------------------
    DX11Context::DX11Context(const SwapChainDescription& swapChainDesc, void* windowHandle)
    {
        m_WindowHandle = static_cast<HWND>(windowHandle);
        
        m_Device = CreateRef<DX11Device>();
        m_SwapChain = CreateRef<DX11SwapChain>(*m_Device, m_WindowHandle, swapChainDesc);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Context::SetVertexBufferInternal(Ref<VertexBuffer> vertexBuffer)
    {
        m_ActiveVertexBuffer = vertexBuffer;
        m_ActiveVertexBuffer->Bind();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Context::SetIndexBufferInternal(Ref<IndexBuffer> indexBuffer)
    {
        m_ActiveIndexBuffer = indexBuffer;
        m_ActiveIndexBuffer->Bind();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Context::SetShaderInternal(Ref<Shader> shader)
    {
        m_ActiveShader = shader;
        m_ActiveShader->Bind();
    }

#endif // METEOR_PLATFORM_WINDOWS 
}
