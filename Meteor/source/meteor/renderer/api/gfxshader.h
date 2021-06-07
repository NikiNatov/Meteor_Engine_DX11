#pragma once

#include "meteor\core\base.h"

namespace meteor
{
    class UniformBufferDescription;
    class ShaderResource;

    enum class ShaderType
    {
        None = 0,
        VertexShader,
        PixelShader
    };

    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void                                     Bind() = 0;

        virtual void                                     SetVSSystemUniformBuffer(mtrByte* data, mtrU32 size) = 0;
        virtual void                                     SetVSUserUniformBuffer(mtrByte* data, mtrU32 size) = 0;

        virtual void                                     SetPSSystemUniformBuffer(mtrByte* data, mtrU32 size) = 0;
        virtual void                                     SetPSUserUniformBuffer(mtrByte* data, mtrU32 size) = 0;

        virtual Ref<UniformBufferDescription>            GetVSUserUniformBuffer() = 0;
        virtual Ref<UniformBufferDescription>            GetVSSystemUniformBuffer() = 0;
        virtual Ref<UniformBufferDescription>            GetPSUserUniformBuffer() = 0;
        virtual Ref<UniformBufferDescription>            GetPSSystemUniformBuffer() = 0;
        virtual const std::vector<Ref<ShaderResource>>&  GetResources() = 0;
    };
}