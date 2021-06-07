#include "pch.h"
#include "material.h"

#include "meteor\renderer\api\gfxcontext.h"

namespace meteor
{
    // -------------------------------------------------------Material-----------------------------------------------------------------
    Material::Material(Ref<Shader> shader, mtrU32 flags /*= 0*/)
        : m_Shader(shader), m_Flags(flags)
    {
        Ref<UniformBufferDescription> vsUniformBuffer = m_Shader->GetVSUserUniformBuffer();
        if (vsUniformBuffer)
        {
            m_VSUserUniformBufferSize = vsUniformBuffer->GetBufferSize();
            m_VSUserUniformBufferData = new mtrByte[m_VSUserUniformBufferSize];

            const auto& uniforms = vsUniformBuffer->GetUniformList();
            m_VSUserUniforms.reserve(uniforms.size());

            for (Ref<ShaderUniform> uniform : uniforms)
            {
                m_VSUserUniforms.emplace_back(uniform);
            }
        }

        Ref<UniformBufferDescription> psUniformBuffer = m_Shader->GetPSUserUniformBuffer();
        if (psUniformBuffer)
        {
            m_PSUserUniformBufferSize = psUniformBuffer->GetBufferSize();
            m_PSUserUniformBufferData = new mtrByte[m_PSUserUniformBufferSize];

            const auto& uniforms = psUniformBuffer->GetUniformList();
            m_PSUserUniforms.reserve(uniforms.size());

            for (Ref<ShaderUniform> uniform : uniforms)
            {
                m_PSUserUniforms.emplace_back(uniform);
            }
        }

        m_Resources = m_Shader->GetResources();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Material::Bind()
    {
        GfxContext::SetShader(m_Shader);

        if(m_VSUserUniformBufferData)
            m_Shader->SetVSUserUniformBuffer(m_VSUserUniformBufferData, m_VSUserUniformBufferSize);

        if(m_PSUserUniformBufferData)
            m_Shader->SetPSUserUniformBuffer(m_PSUserUniformBufferData, m_PSUserUniformBufferSize);

        for (mtrU32 i = 0; i < m_Textures.size(); i++)
        {
            m_Textures[i]->Bind(i);
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Material::SetRenderFlags(mtrU32 flags)
    {
        m_Flags = flags;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Material::SetTexture(const mtrString& name, Ref<Texture> texture)
    {
        Ref<ShaderResource> resource = FindResourceDeclaration(name);
        
        if (!resource)
        {
            METEOR_ENGINE_ASSERT(false, "Texture with name {0} not found!", name);
            return;
        }

        if (m_Textures.size() <= resource->GetRegister())
            m_Textures.resize(resource->GetRegister() + 1);
        m_Textures[resource->GetRegister()] = texture;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Ref<ShaderUniform> Material::FindUniformDeclaration(const mtrString& name, mtrByte*& uniformBuffer)
    {
        for (Ref<ShaderUniform> uniform : m_VSUserUniforms)
        {
            if (uniform->GetName() == name)
            {
                uniformBuffer = m_VSUserUniformBufferData;
                return uniform;
            }
        }

        for (Ref<ShaderUniform> uniform : m_PSUserUniforms)
        {
            if (uniform->GetName() == name)
            {
                uniformBuffer = m_PSUserUniformBufferData;
                return uniform;
            }
        }

        return nullptr;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Ref<ShaderResource> Material::FindResourceDeclaration(const mtrString& name)
    {
        for (Ref<ShaderResource> resource : m_Resources)
            if (resource->GetName() == name)
                return resource;

        return nullptr;
    }

    // ------------------------------------------------------------Material Instance --------------------------------------------------------------
    MaterialInstance::MaterialInstance(Ref<Material> material)
        : m_Material(material)
    {
        m_Flags = material->m_Flags;
        Ref<Shader> materialShader = m_Material->GetShader();

        Ref<UniformBufferDescription> vsUniformBuffer = materialShader->GetVSUserUniformBuffer();
        if (vsUniformBuffer)
        {
            m_VSUserUniformBufferSize = vsUniformBuffer->GetBufferSize();
            m_VSUserUniformBufferData = new mtrByte[m_VSUserUniformBufferSize];
            memcpy(m_VSUserUniformBufferData, m_Material->m_VSUserUniformBufferData, m_Material->m_VSUserUniformBufferSize);

            const auto& uniforms = vsUniformBuffer->GetUniformList();
            m_VSUserUniforms.reserve(uniforms.size());

            for (Ref<ShaderUniform> uniform : uniforms)
            {
                m_VSUserUniforms.emplace_back(uniform);
            }
        }

        Ref<UniformBufferDescription> psUniformBuffer = materialShader->GetPSUserUniformBuffer();
        if (psUniformBuffer)
        {
            m_PSUserUniformBufferSize = psUniformBuffer->GetBufferSize();
            m_PSUserUniformBufferData = new mtrByte[m_PSUserUniformBufferSize];
            memcpy(m_PSUserUniformBufferData, m_Material->m_PSUserUniformBufferData, m_Material->m_PSUserUniformBufferSize);

            const auto& uniforms = psUniformBuffer->GetUniformList();
            m_PSUserUniforms.reserve(uniforms.size());

            for (Ref<ShaderUniform> uniform : uniforms)
            {
                m_PSUserUniforms.emplace_back(uniform);
            }
        }

        m_Resources = materialShader->GetResources();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void MaterialInstance::Bind()
    {
        m_Material->Bind();

        if (m_VSUserUniformBufferData)
            m_Material->m_Shader->SetVSUserUniformBuffer(m_VSUserUniformBufferData, m_VSUserUniformBufferSize);

        if (m_PSUserUniformBufferData)
            m_Material->m_Shader->SetPSUserUniformBuffer(m_PSUserUniformBufferData, m_PSUserUniformBufferSize);

        for (mtrU32 i = 0; i < m_Textures.size(); i++)
        {
            m_Textures[i]->Bind(i);
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void MaterialInstance::SetRenderFlags(mtrU32 flags)
    {
        m_Flags = flags;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void MaterialInstance::SetTexture(const mtrString& name, Ref<Texture> texture)
    {
        Ref<ShaderResource> resource = FindResourceDeclaration(name);

        if (!resource)
        {
            METEOR_ENGINE_ASSERT(false, "Texture with name {0} not found!", name);
            return;
        }

        if (m_Textures.size() <= resource->GetRegister())
            m_Textures.resize(resource->GetRegister() + 1);
        m_Textures[resource->GetRegister()] = texture;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Ref<ShaderUniform> MaterialInstance::FindUniformDeclaration(const mtrString& name, mtrByte*& uniformBuffer)
    {
        for (Ref<ShaderUniform> uniform : m_VSUserUniforms)
        {
            if (uniform->GetName() == name)
            {
                uniformBuffer = m_VSUserUniformBufferData;
                return uniform;
            }
        }

        for (Ref<ShaderUniform> uniform : m_PSUserUniforms)
        {
            if (uniform->GetName() == name)
            {
                uniformBuffer = m_PSUserUniformBufferData;
                return uniform;
            }
        }

        return nullptr;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Ref<ShaderResource> MaterialInstance::FindResourceDeclaration(const mtrString& name)
    {
        for (Ref<ShaderResource> resource : m_Resources)
            if (resource->GetName() == name)
                return resource;

        return nullptr;
    }

}
