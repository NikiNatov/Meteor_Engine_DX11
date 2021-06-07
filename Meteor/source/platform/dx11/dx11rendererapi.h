#pragma once

#include "meteor\renderer\api\gfxrendererapi.h"

#include "dx11common.h"
#include "dx11device.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    class DX11RendererAPI : public RendererAPI
    {
    public:
        DX11RendererAPI();
        ~DX11RendererAPI();

        virtual void                                        InitializeInternal() override;
        virtual void                                        ClearTargetsInternal(mtrU32 targets) override;
        virtual void                                        DrawIndexedInternal(Ref<IndexBuffer> ib) override;

        virtual void                                        SetViewportInternal(mtrUInt x, mtrUInt y, mtrUInt width, mtrUInt height) override;
        virtual void                                        SetBlendStateInternal(mtrBool state) override;
        virtual void                                        SetDepthStencilTestInternal(mtrBool state) override;
        virtual void                                        SetWireframeModeInternal(mtrBool state) override;
    private:
        void                                                CreateBlendStates();
        void                                                CreateDepthStencilStates();
        void                                                CreateRasterizerStates();
    private:
        std::vector<wrl::ComPtr<ID3D11BlendState>>          m_BlendStates;
        std::vector<wrl::ComPtr<ID3D11DepthStencilState>>   m_DepthStencilStates;
        std::vector<wrl::ComPtr<ID3D11RasterizerState>>     m_RasterizerStates;
        Ref<DX11Device>                                     m_Device;
    };

#endif // METEOR_PLATFORM_WINDOWS
}