#pragma once

#include "meteor\renderer\api\gfxtexture.h"

#include "dx11common.h"
#include "dx11device.h"

namespace meteor
{
    class DX11Texture2D : public Texture2D
    {
    public:
        DX11Texture2D(DX11Device& device, const mtrString& filepath, const TextureSpecification& spec);
        DX11Texture2D(DX11Device& device, mtrU32 width, mtrU32 height, const TextureSpecification& spec);

        virtual void							         Bind(mtrU32 slot) override;
        virtual mtrU32                                   GetWidth() const override { return m_Width; }
        virtual mtrU32                                   GetHeight() const override { return m_Height; }

        virtual void	                                 SetData(const void* data) override;
                                                         
        virtual const mtrString&                         GetFilepath() const override { return m_Filepath;}
        virtual const TextureSpecification&              GetSpecification() const override { return m_Specification; }
        virtual void*                                    GetTextureHandle() const { return m_TextureSRV.Get(); }
        // DirectX specific functions 
        inline wrl::ComPtr<ID3D11Texture2D>              GetD3DTexture() const { return m_TextureHandle; }
        inline wrl::ComPtr<ID3D11ShaderResourceView>     GetD3DTextureSRV() const { return m_TextureSRV; }
        inline wrl::ComPtr<ID3D11SamplerState>           GetD3DTextureSamplerState() const { return m_TextureSamplerState; }
    private:
        void                                             Initialize();
    private:
        mtrString                                        m_Filepath;
        mtrU32                                           m_Width;
        mtrU32                                           m_Height;
        TextureSpecification                             m_Specification;
                                                         
        wrl::ComPtr<ID3D11Texture2D>                     m_TextureHandle;
        wrl::ComPtr<ID3D11ShaderResourceView>            m_TextureSRV;
        wrl::ComPtr<ID3D11SamplerState>                  m_TextureSamplerState;

        DX11Device&                                      m_Device;

    };
}