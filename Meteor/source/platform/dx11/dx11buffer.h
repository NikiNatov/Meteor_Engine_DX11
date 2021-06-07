#pragma once

#include "meteor\renderer\api\gfxbuffer.h"

#include "dx11common.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    class DX11Device;

    class DX11VertexBuffer : public VertexBuffer
    {
    public:
        DX11VertexBuffer(DX11Device& device, const void* data, mtrUInt size);

        virtual void                    Bind() override;
        virtual void                    SetBufferLayout(const BufferLayout& layout) override;

        virtual const BufferLayout&     GetBufferLayout() const override { return m_BufferLayout; }
    private:
        wrl::ComPtr<ID3D11Buffer>       m_Buffer = nullptr;
        wrl::ComPtr<ID3D11InputLayout>  m_InputLayout = nullptr;
        BufferLayout                    m_BufferLayout;

        DX11Device&                     m_Device;
    };

    class DX11IndexBuffer : public IndexBuffer
    {
    public:
        DX11IndexBuffer(DX11Device& device, const mtrU32* data, mtrUInt count);

        virtual void                Bind() override;
        virtual void                SetTopology(PrimitiveTopology topology) override;

        virtual mtrUInt             GetCount() const override { return m_Count; }
    private:
        wrl::ComPtr<ID3D11Buffer>   m_Buffer;
        mtrUInt                     m_Count;
        PrimitiveTopology           m_Topology;

        DX11Device&                 m_Device;
    };

#endif // METEOR_PLATFORM_WINDOWS
}