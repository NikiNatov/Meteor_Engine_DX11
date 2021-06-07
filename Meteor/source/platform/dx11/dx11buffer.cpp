#include "pch.h"
#include "dx11buffer.h"

#include "meteor\renderer\api\gfxcontext.h"

#include "dx11utils.h"
#include "dx11device.h"
#include "dx11shader.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    // -------------------------------------------------Vertex Buffer------------------------------------------------------------------------
    DX11VertexBuffer::DX11VertexBuffer(DX11Device& device, const void* data, mtrUInt size)
        : m_Device(device)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth      = size;
        desc.CPUAccessFlags = 0;
        desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
        desc.Usage          = D3D11_USAGE_DEFAULT;

        D3D11_SUBRESOURCE_DATA bufferData = {};
        bufferData.pSysMem  = data;

        DX_CALL((*m_Device)->CreateBuffer(&desc, &bufferData, &m_Buffer));
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11VertexBuffer::Bind()
    {
        mtrUInt stride = m_BufferLayout.GetStride();
        mtrUInt offset = 0;

        m_Device.GetD3DImmediateContext()->IASetInputLayout(m_InputLayout.Get());
        m_Device.GetD3DImmediateContext()->IASetVertexBuffers(0, 1, m_Buffer.GetAddressOf(), &stride, &offset);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11VertexBuffer::SetBufferLayout(const BufferLayout& layout)
    {
        m_BufferLayout = layout;
        auto layoutElements = m_BufferLayout.GetElements();
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout(layoutElements.size());

        for (mtrInt i = 0; i < layoutElements.size(); i++)
        {
            inputLayout[i] = { layoutElements[i].Semantic.c_str(), 
                                0,
                                dx11utils::ShaderDataTypeToDXGIFormat(layoutElements[i].Type),
                                0,
                                layoutElements[i].Offset, 
                                D3D11_INPUT_PER_VERTEX_DATA, 
                                0 };
        }

        wrl::ComPtr<ID3DBlob> vsData = std::static_pointer_cast<DX11Shader>(GfxContext::GetActiveShader())->GetVSData();

        DX_CALL((*m_Device)->CreateInputLayout(inputLayout.data(), layoutElements.size(), vsData->GetBufferPointer(), vsData->GetBufferSize(), &m_InputLayout));
    }

    // --------------------------------------------------Index Buffer------------------------------------------------------------------------

    DX11IndexBuffer::DX11IndexBuffer(DX11Device& device, const mtrU32* data, mtrUInt count)
        : m_Count(count), m_Device(device), m_Topology(PrimitiveTopology::Triangles)
    {
        D3D11_BUFFER_DESC desc = {};
        desc.ByteWidth      = count * sizeof(mtrU32);
        desc.CPUAccessFlags = 0;
        desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
        desc.Usage          = D3D11_USAGE_DEFAULT;
        desc.MiscFlags      = 0;

        D3D11_SUBRESOURCE_DATA bufferData = {};
        bufferData.pSysMem  = data;

        DX_CALL((*m_Device)->CreateBuffer(&desc, &bufferData, &m_Buffer));
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11IndexBuffer::Bind()
    {
        m_Device.GetD3DImmediateContext()->IASetPrimitiveTopology(dx11utils::PrimitiveTopologyToDXTopology(m_Topology));
        m_Device.GetD3DImmediateContext()->IASetIndexBuffer(m_Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11IndexBuffer::SetTopology(PrimitiveTopology topology)
    {
        m_Topology = topology;
    }

#endif // METEOR_PLATFORM_WINDOWS  
}
