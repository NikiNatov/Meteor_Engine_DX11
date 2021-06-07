#include "pch.h"
#include "dx11shader.h"

#include <d3dcompiler.h>
#include <fstream>
#include <sstream>

#include "meteor\core\logger.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    // --------------------------------------------------------------------------------------------------------------------------------------
    static mtrString ReadFromFile(const mtrString& filepath)
    {
        std::stringstream ss;
        std::ifstream stream(filepath, std::ios::binary | std::ios::in);

        mtrString line = "";

        if (stream.is_open())
        {
            while (std::getline(stream, line))
            {
                ss << line << "\n";
            }
        }
        else
            METEOR_ENGINE_ASSERT(false, "Failed to open shader file: {0}", filepath);

        return ss.str();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    DX11Shader::DX11Shader(DX11Device& device, const mtrString& filepath)
        : m_Device(device)
    {
        // Generate name
        auto lastSlash = filepath.find_last_of("/\\");
        mtrU32 startPos = lastSlash == mtrString::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        mtrU32 nameLength = lastDot - startPos;
        m_Name = filepath.substr(startPos, nameLength);

        // Read source code
        mtrString source = ReadFromFile(filepath);

        // Compile and reflect
        Compile(source);
        Reflect(ShaderType::VertexShader);
        Reflect(ShaderType::PixelShader);
        CreateConstantBuffers();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::Bind()
    {
        m_Device.GetD3DImmediateContext()->VSSetShader(m_VertexShader.Get(), 0, 0);
        m_Device.GetD3DImmediateContext()->PSSetShader(m_PixelShader.Get(), 0, 0);

        m_Device.GetD3DImmediateContext()->VSSetConstantBuffers(0, m_VSConstantBuffers.size(), m_VSConstantBuffers.data()->GetAddressOf());
        m_Device.GetD3DImmediateContext()->PSSetConstantBuffers(0, m_PSConstantBuffers.size(), m_PSConstantBuffers.data()->GetAddressOf());
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::SetVSSystemUniformBuffer(mtrByte* data, mtrU32 size)
    {
        D3D11_MAPPED_SUBRESOURCE msr = {};
        auto buffer = m_VSConstantBuffers[m_VSSystemUniformBuffer->GetRegister()];

        m_Device.GetD3DImmediateContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, data, size);
        m_Device.GetD3DImmediateContext()->Unmap(buffer.Get(), 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::SetVSUserUniformBuffer(mtrByte* data, mtrU32 size)
    {
        D3D11_MAPPED_SUBRESOURCE msr = {};
        auto buffer = m_VSConstantBuffers[m_VSUserUniformBuffer->GetRegister()];

        m_Device.GetD3DImmediateContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, data, size);
        m_Device.GetD3DImmediateContext()->Unmap(buffer.Get(), 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::SetPSSystemUniformBuffer(mtrByte* data, mtrU32 size)
    {
        D3D11_MAPPED_SUBRESOURCE msr = {};
        auto buffer = m_PSConstantBuffers[m_PSSystemUniformBuffer->GetRegister()];

        m_Device.GetD3DImmediateContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, data, size);
        m_Device.GetD3DImmediateContext()->Unmap(buffer.Get(), 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::SetPSUserUniformBuffer(mtrByte* data, mtrU32 size)
    {
        D3D11_MAPPED_SUBRESOURCE msr = {};
        auto buffer = m_PSConstantBuffers[m_PSUserUniformBuffer->GetRegister()];

        m_Device.GetD3DImmediateContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, data, size);
        m_Device.GetD3DImmediateContext()->Unmap(buffer.Get(), 0);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::Reflect(ShaderType shaderType)
    {
        wrl::ComPtr<ID3D11ShaderReflection> reflector = nullptr;

        switch (shaderType)
        {
            case ShaderType::VertexShader:
                DX_CALL(D3DReflect(m_VSDataBlob->GetBufferPointer(), m_VSDataBlob->GetBufferSize(), IID_PPV_ARGS(&reflector)));
                METEOR_ENGINE_LOG_INFO("Vertex Shader: {0}", m_Name);
                break;
            case ShaderType::PixelShader:
                DX_CALL(D3DReflect(m_PSDataBlob->GetBufferPointer(), m_PSDataBlob->GetBufferSize(), IID_PPV_ARGS(&reflector)));
                METEOR_ENGINE_LOG_INFO("Pixel Shader: {0}", m_Name);
                break;
        }   

        D3D11_SHADER_DESC shaderDesc = {};
        DX_CALL(reflector->GetDesc(&shaderDesc));
        
        // Constant buffers
        for (mtrInt i = 0; i < shaderDesc.ConstantBuffers; i++)
        {
            ID3D11ShaderReflectionConstantBuffer* cb = reflector->GetConstantBufferByIndex(i);
            D3D11_SHADER_BUFFER_DESC cbDesc = {};
            cb->GetDesc(&cbDesc);

            METEOR_ENGINE_LOG_INFO("    Constant buffer:");
            METEOR_ENGINE_LOG_INFO("        Name: {0}", cbDesc.Name);
            METEOR_ENGINE_LOG_INFO("        Size: {0}", cbDesc.Size);
            METEOR_ENGINE_LOG_INFO("        Uniforms: {0}", cbDesc.Variables);

            mtrString cbName = cbDesc.Name;

            Ref<DX11UniformBufferDescription> currentBuffer = nullptr;

            for (mtrInt j = 0; j < cbDesc.Variables; j++)
            {
                ID3D11ShaderReflectionVariable* var = cb->GetVariableByIndex(j);
                D3D11_SHADER_VARIABLE_DESC varDesc = {};
                var->GetDesc(&varDesc);

                mtrString varName = varDesc.Name;

                Ref<DX11ShaderUniform> uniform = CreateRef<DX11ShaderUniform>(varName, varDesc.Size, varDesc.StartOffset);

                if (!currentBuffer)
                {
                    // If the buffer is not initialize then initialize it and determine what type it is based on the uniform names
                    currentBuffer = CreateRef<DX11UniformBufferDescription>(cbName, i);
                    if (varName.find("sys_") == 0)
                    {
                        // If the variable name starts with "sys_" set the system uniform buffer
                        switch (shaderType)
                        {
                            case ShaderType::VertexShader:
                                METEOR_ENGINE_ASSERT(!m_VSUserUniformBuffer, "Vertex shader system uniform buffer already set! Only one system uniform buffer is supported!");
                                m_VSSystemUniformBuffer = currentBuffer;
                                break;
                            case ShaderType::PixelShader:
                                METEOR_ENGINE_ASSERT(!m_PSUserUniformBuffer, "Pixel shader system uniform buffer already set! Only one system uniform buffer is supported!");
                                m_PSSystemUniformBuffer = currentBuffer;
                                break;
                        }
                    }
                    else
                    {
                        // If the variable name does not start with "sys_" set the user uniform buffer
                        switch (shaderType)
                        {
                            case ShaderType::VertexShader:
                                METEOR_ENGINE_ASSERT(!m_VSUserUniformBuffer, "Vertex shader user uniform buffer already set! Only one user uniform buffer is supported!");
                                m_VSUserUniformBuffer = currentBuffer;
                                break;
                            case ShaderType::PixelShader:
                                METEOR_ENGINE_ASSERT(!m_PSUserUniformBuffer, "Pixel shader user uniform buffer already set! Only one user uniform buffer is supported!");
                                m_PSUserUniformBuffer = currentBuffer;
                                break;
                        }
                    }
                } 

                currentBuffer->AddUniform(uniform);
            }
            currentBuffer->AddPadding();
        }

        // Resources
        for (mtrInt i = 0; i < shaderDesc.BoundResources; i++)
        {
            D3D11_SHADER_INPUT_BIND_DESC desc = {};
            reflector->GetResourceBindingDesc(i, &desc);

            Ref<DX11ShaderResource> resource = nullptr;

            if (desc.Type == D3D_SIT_TEXTURE)
            {
                if (desc.Dimension == D3D11_SRV_DIMENSION_TEXTURE2D)
                    resource = CreateRef<DX11ShaderResource>(desc.Name, ShaderResource::Type::Texture2D, desc.BindPoint);
                else if (desc.Dimension == D3D11_SRV_DIMENSION_TEXTURECUBE)
                    resource = CreateRef<DX11ShaderResource>(desc.Name, ShaderResource::Type::TextureCube, desc.BindPoint);
            }
            else if (desc.Type == D3D_SIT_SAMPLER)
            {
                resource = CreateRef<DX11ShaderResource>(desc.Name, ShaderResource::Type::SamplerState, desc.BindPoint);
            }

            if(resource)
                m_Resources.push_back(resource);
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::CreateConstantBuffers()
    {
        mtrU32 vsBufferCount = 0;
        mtrU32 psBufferCount = 0;

        // Get the number of constant buffers for each shader type
        if (m_VSSystemUniformBuffer)    vsBufferCount++;
        if (m_VSUserUniformBuffer)      vsBufferCount++;
        if (m_PSSystemUniformBuffer)    psBufferCount++;
        if (m_PSUserUniformBuffer)      psBufferCount++;

        // Create VS constant buffers
        if (vsBufferCount)
        {
            m_VSConstantBuffers.resize(vsBufferCount);
            if (m_VSSystemUniformBuffer)
            {
                D3D11_BUFFER_DESC desc = {};
                desc.Usage          = D3D11_USAGE_DYNAMIC;
                desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.ByteWidth      = m_VSSystemUniformBuffer->GetBufferSize();

                D3D11_SUBRESOURCE_DATA data = {};
                data.pSysMem        = new mtrByte[desc.ByteWidth];

                DX_CALL((*m_Device)->CreateBuffer(&desc, &data, &m_VSConstantBuffers[m_VSSystemUniformBuffer->GetRegister()]));
            }

            if (m_VSUserUniformBuffer)
            {
                D3D11_BUFFER_DESC desc = {};
                desc.Usage          = D3D11_USAGE_DYNAMIC;
                desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.ByteWidth      = m_VSUserUniformBuffer->GetBufferSize();

                D3D11_SUBRESOURCE_DATA data = {};
                data.pSysMem        = new mtrByte[desc.ByteWidth];

                DX_CALL((*m_Device)->CreateBuffer(&desc, &data, &m_VSConstantBuffers[m_VSUserUniformBuffer->GetRegister()]));
            }
        } 

        // Create PS constant buffers
        if (psBufferCount)
        {
            m_PSConstantBuffers.resize(psBufferCount);
            if (m_PSSystemUniformBuffer)
            {
                D3D11_BUFFER_DESC desc = {};
                desc.Usage          = D3D11_USAGE_DYNAMIC;
                desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.ByteWidth      = m_PSSystemUniformBuffer->GetBufferSize();

                D3D11_SUBRESOURCE_DATA data = {};
                data.pSysMem        = new mtrByte[desc.ByteWidth];

                DX_CALL((*m_Device)->CreateBuffer(&desc, &data, &m_PSConstantBuffers[m_PSSystemUniformBuffer->GetRegister()]));
            }

            if (m_PSUserUniformBuffer)
            {
                D3D11_BUFFER_DESC desc = {};
                desc.Usage          = D3D11_USAGE_DYNAMIC;
                desc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
                desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
                desc.ByteWidth      = m_PSUserUniformBuffer->GetBufferSize();

                D3D11_SUBRESOURCE_DATA data = {};
                data.pSysMem        = new mtrByte[desc.ByteWidth];

                DX_CALL((*m_Device)->CreateBuffer(&desc, &data, &m_PSConstantBuffers[m_PSUserUniformBuffer->GetRegister()]));
            }
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Shader::Compile(const mtrString& source)
    {
        wrl::ComPtr<ID3DBlob> errorBlob = nullptr;

        // Compile vertex shader
        D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "VSMain", "vs_4_0", D3DCOMPILE_DEBUG, 0, &m_VSDataBlob, &errorBlob);
        if (errorBlob && errorBlob->GetBufferSize())
        {
            METEOR_ENGINE_ASSERT(false, "Vertex Shader Compilation Failure: {0}", (mtrChar*)errorBlob->GetBufferPointer());
            return;
        }

        DX_CALL((*m_Device)->CreateVertexShader(m_VSDataBlob->GetBufferPointer(), m_VSDataBlob->GetBufferSize(), nullptr, &m_VertexShader));

        // Compile pixel shader
        D3DCompile(source.c_str(), source.size(), nullptr, nullptr, nullptr, "PSMain", "ps_4_0", D3DCOMPILE_DEBUG, 0, &m_PSDataBlob, &errorBlob);
        if (errorBlob && errorBlob->GetBufferSize())
        {
            METEOR_ENGINE_ASSERT(false, "Pixel Shader Compilation Failure: {0}", (mtrChar*)errorBlob->GetBufferPointer());
            return;
        }

        DX_CALL((*m_Device)->CreatePixelShader(m_PSDataBlob->GetBufferPointer(), m_PSDataBlob->GetBufferSize(), nullptr, &m_PixelShader));
    }

#endif // METEOR_PLATFORM_WINDOWS
}
