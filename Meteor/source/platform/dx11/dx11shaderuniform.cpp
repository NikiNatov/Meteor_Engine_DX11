#include "pch.h"
#include "dx11shaderuniform.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)
    // --------------------------------------------------------------------------------------------------------------------------------------
    DX11UniformBufferDescription::DX11UniformBufferDescription(const mtrString& name, mtrU32 bufferRegister)
        : m_Name(name), m_Register(bufferRegister), m_Size(0)
    {
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11UniformBufferDescription::AddUniform(Ref<ShaderUniform> uniform)
    {
        m_Size += uniform->GetSize();
        m_UniformVariables.emplace_back(uniform);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Ref<ShaderUniform> DX11UniformBufferDescription::FindUniform(const mtrString& name)
    {
        for (Ref<ShaderUniform>& uniform : m_UniformVariables)
        {
            if (uniform->GetName() == name)
                return uniform;
        }

        return nullptr;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11UniformBufferDescription::AddPadding()
    {
        m_Size = ((m_Size + 15) / 16) * 16;
    }
#endif // METEOR_PLATFORM_WINDOWS
}