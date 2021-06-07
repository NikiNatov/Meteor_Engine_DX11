#pragma once

#include "meteor\core\base.h"

namespace meteor
{
    class ShaderUniform
    {
    public:
        virtual ~ShaderUniform() = default;

        virtual const mtrString&    GetName() const = 0;
        virtual mtrU32              GetSize() const = 0;
        virtual mtrU32              GetOffset() const = 0;
    };

    class UniformBufferDescription
    {
    public:
        virtual ~UniformBufferDescription() = default;

        virtual void                                     AddUniform(Ref<ShaderUniform> uniform) = 0;
        virtual Ref<ShaderUniform>                       FindUniform(const mtrString& name) = 0;

        virtual const mtrString&                         GetBufferName() const = 0;
        virtual mtrU32                                   GetBufferSize() const = 0;
        virtual mtrU32                                   GetRegister() const = 0;
        virtual const std::vector<Ref<ShaderUniform>>&   GetUniformList() const = 0;
    };

    class ShaderResource
    {
    public:
        enum class Type
        {
            None = 0,
            Texture2D, TextureCube,
            SamplerState
        };
    public:
        virtual ~ShaderResource() = default;

        virtual const mtrString&     GetName() const = 0;
        virtual Type                 GetType() const = 0;
        virtual mtrU32               GetRegister() const = 0;
    };
}