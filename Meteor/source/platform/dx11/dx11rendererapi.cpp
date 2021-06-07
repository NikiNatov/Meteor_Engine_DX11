#include "pch.h"
#include "dx11rendererapi.h"

#include "dx11context.h"

namespace meteor 
{
#if defined(METEOR_PLATFORM_WINDOWS)

    static mtrString BytesToString(mtrU64 bytes)
    {
        mtrString result;

        const mtrUInt gb = 1000 * 1000 * 1000;
        const mtrUInt mb = 1000 * 1000;
        const mtrUInt kb = 1000;

        if (bytes > gb)
            result = std::to_string(bytes / gb) + " GB";
        else if (bytes > mb)
            result = std::to_string(bytes / mb) + " MB";
        else if (bytes > kb)
            result = std::to_string(bytes / kb) + " KB";
        else
            result = std::to_string(bytes) + " B";

        return result;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    DX11RendererAPI::DX11RendererAPI()
    {
        m_Device = std::static_pointer_cast<DX11Device>(GfxContext::GetDevice());
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    DX11RendererAPI::~DX11RendererAPI()
    {
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::InitializeInternal()
    {
        CreateBlendStates();
        CreateDepthStencilStates();
        CreateRasterizerStates();

        SetDepthStencilTest(true);
        SetBlendState(true);
        SetWireframeMode(false);

        IDXGIDevice* dxgiDevice;
        IDXGIAdapter* dxgiAdapter;
        m_Device->GetD3DDevice()->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
        dxgiDevice->GetAdapter(&dxgiAdapter);

        DXGI_ADAPTER_DESC desc;
        dxgiAdapter->GetDesc(&desc);

        char adapterDescription[256]{ 0 };
        wcstombs(adapterDescription, desc.Description, sizeof(desc.Description));

        METEOR_ENGINE_LOG_INFO("------------------------------------------------");
        METEOR_ENGINE_LOG_INFO("DirectX 11:");
        METEOR_ENGINE_LOG_INFO("   Description: {0}", adapterDescription);
        METEOR_ENGINE_LOG_INFO("   Video memory: {0}", BytesToString(desc.DedicatedVideoMemory));
        METEOR_ENGINE_LOG_INFO("------------------------------------------------");
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::ClearTargetsInternal(mtrU32 targets)
    {
        if (targets & COLOR_BUFFER)
            DX11Context::GetSwapChain()->ClearBackBuffer();
        if (targets & DEPTH_BUFFER)
            DX11Context::GetSwapChain()->ClearDepthBuffer();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::DrawIndexedInternal(Ref<IndexBuffer> ib)
    {
        m_Device->GetD3DImmediateContext()->DrawIndexed(ib->GetCount(), 0, 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::SetViewportInternal(mtrUInt x, mtrUInt y, mtrUInt width, mtrUInt height)
    {
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX   = x;
        viewport.TopLeftY   = y;
        viewport.Width      = width;
        viewport.Height     = height;
        viewport.MinDepth   = 0.0f;
        viewport.MaxDepth   = 1.0f;

        m_Device->GetD3DImmediateContext()->RSSetViewports(1, &viewport);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::SetBlendStateInternal(mtrBool state)
    {
        m_Device->GetD3DImmediateContext()->OMSetBlendState(state ? m_BlendStates[1].Get() : m_BlendStates[0].Get(), nullptr, 0xffffffff);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::SetDepthStencilTestInternal(mtrBool state)
    {
        m_Device->GetD3DImmediateContext()->OMSetDepthStencilState(state ? m_DepthStencilStates[1].Get() : m_DepthStencilStates[0].Get(), 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::SetWireframeModeInternal(mtrBool state)
    {
        m_Device->GetD3DImmediateContext()->RSSetState(state ? m_RasterizerStates[0].Get() : m_RasterizerStates[1].Get());
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::CreateBlendStates()
    {
        // Create blend disabled state
        {
            D3D11_BLEND_DESC desc = {};
            desc.RenderTarget[0].BlendEnable            = false;
            desc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;

            wrl::ComPtr<ID3D11BlendState> blendState = nullptr;
            DX_CALL(m_Device->GetD3DDevice()->CreateBlendState(&desc, &blendState));
            m_BlendStates.push_back(blendState);
        }

        // Create blend enabled state
        {
            D3D11_BLEND_DESC desc = {};
            desc.AlphaToCoverageEnable                  = false;
            desc.IndependentBlendEnable                 = false;
            desc.RenderTarget[0].BlendEnable            = true;
            desc.RenderTarget[0].RenderTargetWriteMask  = D3D11_COLOR_WRITE_ENABLE_ALL;
            desc.RenderTarget[0].SrcBlend               = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlend              = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].SrcBlendAlpha          = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlendAlpha         = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].BlendOp                = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[0].BlendOpAlpha           = D3D11_BLEND_OP_ADD;

            wrl::ComPtr<ID3D11BlendState> blendState = nullptr;
            DX_CALL(m_Device->GetD3DDevice()->CreateBlendState(&desc, &blendState));
            m_BlendStates.push_back(blendState);
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11RendererAPI::CreateDepthStencilStates()
    {
        // Create depth disabled state
        {
            D3D11_DEPTH_STENCIL_DESC desc = {};
            desc.DepthEnable                  = false;
            desc.DepthFunc                    = D3D11_COMPARISON_ALWAYS;
            desc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
            desc.StencilEnable                = true;
            desc.StencilReadMask              = 0xFF;
            desc.StencilWriteMask             = 0xFF;

            desc.BackFace.StencilFunc         = D3D11_COMPARISON_NEVER;
            desc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
            desc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
            desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;

            desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
            desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
            desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR_SAT;
            desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

            wrl::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
            DX_CALL(m_Device->GetD3DDevice()->CreateDepthStencilState(&desc, &depthStencilState));
            m_DepthStencilStates.push_back(depthStencilState);
        }

        // Create depth enabled state
        {
            D3D11_DEPTH_STENCIL_DESC desc = {};
            desc.DepthEnable                  = true;
            desc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
            desc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.StencilEnable                = true;
            desc.StencilReadMask              = 0xFF;
            desc.StencilWriteMask             = 0xFF;

            desc.BackFace.StencilFunc         = D3D11_COMPARISON_NEVER;
            desc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
            desc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
            desc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;

            desc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
            desc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
            desc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR_SAT;
            desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

            wrl::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
            DX_CALL(m_Device->GetD3DDevice()->CreateDepthStencilState(&desc, &depthStencilState));
            m_DepthStencilStates.push_back(depthStencilState);
        }
    }

    void DX11RendererAPI::CreateRasterizerStates()
    {
        // Wireframe mode state
        {
            D3D11_RASTERIZER_DESC desc = {};
            desc.AntialiasedLineEnable  = false;
            desc.CullMode               = D3D11_CULL_BACK;
            desc.DepthBias              = 0;
            desc.DepthBiasClamp         = 0.0f;
            desc.DepthClipEnable        = true;
            desc.FillMode               = D3D11_FILL_WIREFRAME;
            desc.FrontCounterClockwise  = true;
            desc.MultisampleEnable      = false;
            desc.ScissorEnable          = false;
            desc.SlopeScaledDepthBias   = 0.0f;

            wrl::ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
            DX_CALL(m_Device->GetD3DDevice()->CreateRasterizerState(&desc, &rasterizerState));
            m_RasterizerStates.push_back(rasterizerState);
        }

        // Solid fill mode state
        {
            D3D11_RASTERIZER_DESC desc = {};
            desc.AntialiasedLineEnable  = false;
            desc.CullMode               = D3D11_CULL_BACK;
            desc.DepthBias              = 0;
            desc.DepthBiasClamp         = 0.0f;
            desc.DepthClipEnable        = true;
            desc.FillMode               = D3D11_FILL_SOLID;
            desc.FrontCounterClockwise  = true;
            desc.MultisampleEnable      = false;
            desc.ScissorEnable          = false;
            desc.SlopeScaledDepthBias   = 0.0f;

            wrl::ComPtr<ID3D11RasterizerState> rasterizerState = nullptr;
            DX_CALL(m_Device->GetD3DDevice()->CreateRasterizerState(&desc, &rasterizerState));
            m_RasterizerStates.push_back(rasterizerState);
        }
    }

#endif // METEOR_PLATFORM_WINDOWS
}
