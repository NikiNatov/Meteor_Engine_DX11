#pragma once

#include "meteor\renderer\api\gfxshaderuniform.h"

#include "dx11common.h"
#include "dx11device.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

    class DX11ShaderUniform : public ShaderUniform
    {
    public:
        DX11ShaderUniform(const mtrString& name, mtrU32 size, mtrU32 offset)
            : m_Name(name), m_Size(size), m_Offset(offset)
        {}

        virtual const mtrString&    GetName() const override { return m_Name; }
        virtual mtrU32              GetSize() const override { return m_Size; }
        virtual mtrU32              GetOffset() const override { return m_Offset; }
    private:
        mtrString                   m_Name;
        mtrU32                      m_Size;
        mtrU32                      m_Offset;
    };

    class DX11UniformBufferDescription : public UniformBufferDescription
    {
        friend class DX11Shader;
    public:
        DX11UniformBufferDescription(const mtrString& name, mtrU32 bufferRegister);

        virtual void                                     AddUniform(Ref<ShaderUniform> uniform) override;
        virtual Ref<ShaderUniform>                       FindUniform(const mtrString& name) override;

        virtual const mtrString&                         GetBufferName() const override { return m_Name; }
        virtual mtrU32                                   GetBufferSize() const override { return m_Size; }
        virtual mtrU32                                   GetRegister() const override { return m_Register; }
        virtual const std::vector<Ref<ShaderUniform>>&   GetUniformList() const override { return m_UniformVariables; }

        void                                             AddPadding();
    private:
        mtrString                                        m_Name;
        mtrU32                                           m_Size;
        mtrU32                                           m_Register;
        std::vector<Ref<ShaderUniform>>                  m_UniformVariables;
    };

    class DX11ShaderResource : public ShaderResource
    {
    public:
        DX11ShaderResource(const mtrString& name, Type type, mtrU32 resourceRegister)
            : m_Name(name), m_Type(type), m_Register(resourceRegister)
        {}

        virtual const mtrString&     GetName() const override { return m_Name; }
        virtual Type                 GetType() const override { return m_Type; }
        virtual mtrU32               GetRegister() const override { return m_Register; }
    private:
        mtrString                    m_Name;
        Type                         m_Type;
        mtrU32                       m_Register;
    };

#endif // METEOR_PLATFORM_WINDOWS
}