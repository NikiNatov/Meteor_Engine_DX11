#pragma once

#include "meteor\renderer\api\gfxswapchain.h"

#include "dx11common.h"
#include "dx11device.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    class DX11SwapChain : public GfxSwapChain
    {
    public:
        DX11SwapChain(DX11Device& device, void* windowHandle, const SwapChainDescription& description);
        virtual ~DX11SwapChain();

        virtual void                           ClearBackBuffer() override;
        virtual void                           ClearDepthBuffer() override;
        virtual void						   Present() override;
        virtual void                           ToogleVSync() override;
        virtual void                           ToggleFullscreen() override;
        virtual void                           Resize(mtrUInt width, mtrUInt height) override;

        virtual mtrBool						   IsFullscreen() const override { return m_SwapChainDesc.Fullscreen; }
        virtual mtrBool						   IsVSync() const override { return m_SwapChainDesc.VSync; }
        virtual mtrUInt                        GetBufferCount() const override { return m_SwapChainDesc.BufferCount; }
        virtual mtrUInt						   GetWidth() const override { return m_SwapChainDesc.Width; }
        virtual mtrUInt						   GetHeight() const override { return m_SwapChainDesc.Height; }
        virtual const SwapChainDescription&	   GetDescription() const override { return m_SwapChainDesc; }

    private:
        wrl::ComPtr<IDXGISwapChain>            m_SwapChain = nullptr;
        SwapChainDescription                   m_SwapChainDesc;

        wrl::ComPtr<ID3D11RenderTargetView>    m_BackBufferRTV;
        wrl::ComPtr<ID3D11DepthStencilView>    m_DepthStencilView;

        DX11Device&                            m_Device;
    };

#endif // METEOR_PLATFORM_WINDOWS
}