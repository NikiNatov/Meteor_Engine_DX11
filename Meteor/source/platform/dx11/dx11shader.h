#pragma once

#include "meteor\renderer\api\gfxshader.h"

#include "dx11common.h"
#include "dx11device.h"
#include "dx11buffer.h"
#include "dx11shaderuniform.h"

namespace meteor
{
#if defined (METEOR_PLATFORM_WINDOWS)

    class DX11Shader : public Shader
    {
    public:
        DX11Shader(DX11Device& device, const mtrString& filepath);

        virtual void                                        Bind() override;

        virtual void                                        SetVSSystemUniformBuffer(mtrByte* data, mtrU32 size) override;
        virtual void                                        SetVSUserUniformBuffer(mtrByte* data, mtrU32 size) override;

        virtual void                                        SetPSSystemUniformBuffer(mtrByte* data, mtrU32 size) override;
        virtual void                                        SetPSUserUniformBuffer(mtrByte* data, mtrU32 size) override;

        virtual Ref<UniformBufferDescription>               GetVSUserUniformBuffer() override { return m_VSUserUniformBuffer; }
        virtual Ref<UniformBufferDescription>               GetVSSystemUniformBuffer() override { return m_VSSystemUniformBuffer; }
        virtual Ref<UniformBufferDescription>               GetPSUserUniformBuffer() override { return m_PSUserUniformBuffer; }
        virtual Ref<UniformBufferDescription>               GetPSSystemUniformBuffer() override { return m_PSSystemUniformBuffer; }
        virtual const std::vector<Ref<ShaderResource>>&     GetResources() override { return m_Resources; }

        // DirectX specific functions
        inline wrl::ComPtr<ID3DBlob>                        GetVSData() { return m_VSDataBlob; }
        inline wrl::ComPtr<ID3DBlob>                        GetPSData() { return m_PSDataBlob; }
    private:
        void                                                Compile(const mtrString& source);
        void                                                Reflect(ShaderType shaderType);
        void                                                CreateConstantBuffers();
    private:
        mtrString                                           m_Name;
        DX11Device&                                         m_Device;
        std::vector<Ref<ShaderResource>>                    m_Resources;

        // Vertex shader
        wrl::ComPtr<ID3D11VertexShader>                     m_VertexShader = nullptr;
        wrl::ComPtr<ID3DBlob>                               m_VSDataBlob = nullptr;
        Ref<DX11UniformBufferDescription>                   m_VSSystemUniformBuffer = nullptr;
        Ref<DX11UniformBufferDescription>                   m_VSUserUniformBuffer = nullptr;
        std::vector<wrl::ComPtr<ID3D11Buffer>>              m_VSConstantBuffers;

        // Pixel shader
        wrl::ComPtr<ID3D11PixelShader>                      m_PixelShader = nullptr;
        wrl::ComPtr<ID3DBlob>                               m_PSDataBlob = nullptr;
        Ref<DX11UniformBufferDescription>                   m_PSSystemUniformBuffer = nullptr;
        Ref<DX11UniformBufferDescription>                   m_PSUserUniformBuffer = nullptr;
        std::vector<wrl::ComPtr<ID3D11Buffer>>              m_PSConstantBuffers;
    };

#endif // METEOR_PLATFORM_WINDOWS
}