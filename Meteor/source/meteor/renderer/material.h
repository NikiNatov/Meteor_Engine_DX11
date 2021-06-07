#pragma once

#include "meteor\core\base.h"
#include "meteor\renderer\api\gfxtexture.h"
#include "meteor\renderer\api\gfxshader.h"
#include "meteor\renderer\api\gfxshaderuniform.h"

#include <glm\gtc\type_ptr.hpp>

namespace meteor
{
    class Material
    {
        friend class MaterialInstance;
    public:
        enum RenderFlags
        {
            None             = 0,
            Transparent      = BIT(0),
            Wireframe        = BIT(1),
            DisableDepthTest = BIT(2),
            Water            = BIT(3)
        };
    public:
        Material(Ref<Shader> shader, mtrU32 flags = 0);

        void                             Bind();                                     
        void                             SetRenderFlags(mtrU32 flags);
        void                             SetTexture(const mtrString& name, Ref<Texture> texture);

        template<typename T>
        void                             SetUniform(const mtrString& name, const T& data)
        {
            mtrByte* uniformBuffer;
            Ref<ShaderUniform> uniform = FindUniformDeclaration(name, uniformBuffer);

            if (!uniform)
            {
                METEOR_ENGINE_ASSERT(false, "Uniform with name {0} not found!", name);
                return;
            }

            memcpy(uniformBuffer + uniform->GetOffset(), &data, uniform->GetSize());
        }

                                         
        inline mtrU32                    GetRenderFlags() const { return m_Flags; }
        inline Ref<Shader>               GetShader() { return m_Shader; }
    private:
        Ref<ShaderUniform>               FindUniformDeclaration(const mtrString& name, mtrByte*& uniformBuffer);
        Ref<ShaderResource>              FindResourceDeclaration(const mtrString& name);
    private:
        Ref<Shader>                      m_Shader;
        std::vector<Ref<Texture>>        m_Textures;
                                         
        mtrU32                           m_Flags = 0;
                                         
        mtrByte*                         m_VSUserUniformBufferData = nullptr;
        mtrU32                           m_VSUserUniformBufferSize = 0;
        std::vector<Ref<ShaderUniform>>  m_VSUserUniforms;
                                         
        mtrByte*                         m_PSUserUniformBufferData = nullptr;
        mtrU32                           m_PSUserUniformBufferSize = 0;
        std::vector<Ref<ShaderUniform>>  m_PSUserUniforms;
        std::vector<Ref<ShaderResource>> m_Resources;
    };

    class MaterialInstance
    {
    public:
        MaterialInstance(Ref<Material> material);

        void                             Bind();
        void                             SetRenderFlags(mtrU32 flags);
        void                             SetTexture(const mtrString& name, Ref<Texture> texture);

        template<typename T>
        void                             SetUniform(const mtrString& name, const T& data)
        {
            mtrByte* uniformBuffer;
            Ref<ShaderUniform> uniform = FindUniformDeclaration(name, uniformBuffer);

            if (!uniform)
            {
                METEOR_ENGINE_ASSERT(false, "Uniform with name {0} not found!", name);
                return;
            }

            memcpy(uniformBuffer + uniform->GetOffset(), &data, uniform->GetSize());
        }


        inline mtrU32                    GetRenderFlags() const { return m_Flags; }
        inline Ref<Material>             GetMaterial() { return m_Material; }
    private:
        Ref<ShaderUniform>               FindUniformDeclaration(const mtrString& name, mtrByte*& uniformBuffer);
        Ref<ShaderResource>              FindResourceDeclaration(const mtrString& name);
    private:
        Ref<Material> m_Material;

        std::vector<Ref<Texture>>        m_Textures;

        mtrU32                           m_Flags = 0;

        mtrByte* m_VSUserUniformBufferData = nullptr;
        mtrU32                           m_VSUserUniformBufferSize = 0;
        std::vector<Ref<ShaderUniform>>  m_VSUserUniforms;

        mtrByte* m_PSUserUniformBufferData = nullptr;
        mtrU32                           m_PSUserUniformBufferSize = 0;
        std::vector<Ref<ShaderUniform>>  m_PSUserUniforms;
        std::vector<Ref<ShaderResource>> m_Resources;
    };
}