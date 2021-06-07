#include "pch.h"
#include "dx11texture.h"

#include "dx11utils.h"

#include <stb_image.h>

namespace meteor
{
	TextureFormat GetFormat(bool isHDR, mtrInt channels)
	{
		if (isHDR)
		{
			switch (channels)
			{
				case 1:	return TextureFormat::R32F;
				case 2:	return TextureFormat::RG32F;
				case 3: return TextureFormat::RGB32F;
				case 4: return TextureFormat::RGBA32F;
			}
		}
		else
		{
			switch (channels)
			{
				case 1:	return TextureFormat::R8;
				case 2:	return TextureFormat::RG8;
				case 3: return TextureFormat::RGB8;
				case 4: return TextureFormat::RGBA8;
			}
		}

		METEOR_ENGINE_ASSERT(false, "Uknown format!");
		return TextureFormat::None;
	}

	mtrU32 GetStrideFromFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::R8:			return 1;
			case TextureFormat::RG8:		return 2;
			case TextureFormat::RGB8:		return 3;
			case TextureFormat::RGBA8:		return 4;
			case TextureFormat::R32F:		return 4;
			case TextureFormat::RG32F:		return 8;
			case TextureFormat::RGB32F:		return 12;
			case TextureFormat::RGBA32F:	return 16;
		}

		METEOR_ENGINE_ASSERT(false, "Invalid format!");
		return 0;
	}

    // ---------------------------------------------------------Texture2D--------------------------------------------------------------------
    DX11Texture2D::DX11Texture2D(DX11Device& device, const mtrString& filepath, const TextureSpecification& spec)
		: m_Device(device), m_Filepath(filepath), m_Specification(spec)
    {
		Initialize();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    DX11Texture2D::DX11Texture2D(DX11Device& device, mtrU32 width, mtrU32 height, const TextureSpecification& spec)
		: m_Device(device), m_Filepath(""), m_Width(width), m_Height(height), m_Specification(spec)
    {
		Initialize();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void DX11Texture2D::Bind(mtrU32 slot)
    {
		m_Device.GetD3DImmediateContext()->PSSetShaderResources(slot, 1, m_TextureSRV.GetAddressOf());
		m_Device.GetD3DImmediateContext()->PSSetSamplers(slot, 1, m_TextureSamplerState.GetAddressOf());
    }

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11Texture2D::SetData(const void* data)
	{
		D3D11_MAPPED_SUBRESOURCE msr = {};

		m_Device.GetD3DImmediateContext()->Map(m_TextureHandle.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, data, m_Width * m_Height * GetStrideFromFormat(m_Specification.Format));
		m_Device.GetD3DImmediateContext()->Unmap(m_TextureHandle.Get(), 0);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	void DX11Texture2D::Initialize()
	{
		void* data = nullptr;

		if (!m_Filepath.empty())
		{
			// Load texture data
			mtrBool isHDR = stbi_is_hdr(m_Filepath.c_str());
			mtrInt width, height, channels;

			data = isHDR ? (void*)stbi_loadf(m_Filepath.c_str(), &width, &height, &channels, 0) : (void*)stbi_load(m_Filepath.c_str(), &width, &height, &channels, 0);

			METEOR_ENGINE_ASSERT(data, "Failed to load texture: {0}", m_Filepath);

			m_Width					= width;
			m_Height				= height;
			m_Specification.Format	= GetFormat(isHDR, channels);
		}

		mtrBool generateMips = data != nullptr;

		// Create texture
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width					= m_Width;
		desc.Height					= m_Height;
		desc.Format					= dx11utils::TextureFormatToDXFormat(m_Specification.Format);
		desc.MipLevels				= generateMips ? 7 : 1;
		desc.ArraySize				= 1;
		desc.Usage					= generateMips ? D3D11_USAGE_DEFAULT : D3D11_USAGE_DYNAMIC;
		desc.SampleDesc.Count		= 1;
		desc.SampleDesc.Quality		= 0;
		desc.BindFlags				= D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags			= desc.Usage == D3D11_USAGE_DYNAMIC ? D3D11_CPU_ACCESS_WRITE : 0;

		if (generateMips)
			desc.BindFlags			|= D3D11_BIND_RENDER_TARGET;

		desc.MiscFlags				= generateMips ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

		D3D11_SUBRESOURCE_DATA textureData = {};
		textureData.pSysMem			 = data;
		textureData.SysMemPitch		 = m_Width * GetStrideFromFormat(m_Specification.Format);
		textureData.SysMemSlicePitch = m_Width * m_Height * GetStrideFromFormat(m_Specification.Format);

		DX_CALL((*m_Device)->CreateTexture2D(&desc, nullptr, &m_TextureHandle));

		// Create SRV
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format						= desc.Format;
		srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels			= desc.MipLevels;

		DX_CALL((*m_Device)->CreateShaderResourceView(m_TextureHandle.Get(), &srvDesc, &m_TextureSRV));

		// Generate mip levels
		if (generateMips)
		{
			m_Device.GetD3DImmediateContext()->UpdateSubresource(m_TextureHandle.Get(), 0, nullptr, textureData.pSysMem, textureData.SysMemPitch, textureData.SysMemSlicePitch);
			m_Device.GetD3DImmediateContext()->GenerateMips(m_TextureSRV.Get());
		}

		// Create sampler
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU		= dx11utils::WrapModeToDXWrapMode(m_Specification.Wrap);
		samplerDesc.AddressV		= dx11utils::WrapModeToDXWrapMode(m_Specification.Wrap);
		samplerDesc.AddressW		= dx11utils::WrapModeToDXWrapMode(m_Specification.Wrap);
		samplerDesc.MinLOD			= 0;
		samplerDesc.MaxLOD			= 11;
		samplerDesc.Filter			= dx11utils::FilterToDXFilter(m_Specification.Filter);
		samplerDesc.ComparisonFunc	= D3D11_COMPARISON_NEVER;

		DX_CALL((*m_Device)->CreateSamplerState(&samplerDesc, &m_TextureSamplerState));

		if(data)
			stbi_image_free(data);
	}
}
