#pragma once

#include "meteor\core\base.h"
#include "meteor\renderer\api\gfxbuffer.h"
#include "meteor\renderer\api\gfxtexture.h"

#include "dx11common.h"

namespace meteor { namespace dx11utils {

#if defined (METEOR_PLATFORM_WINDOWS)

	static DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return DXGI_FORMAT_R32_FLOAT;
			case ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
			case ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
			case ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case ShaderDataType::Int:		return DXGI_FORMAT_R32_SINT;
			case ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
			case ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
			case ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
		}

		METEOR_ENGINE_ASSERT(false, "Unknown data type!");
		return DXGI_FORMAT_UNKNOWN;
	}

	static DXGI_FORMAT TextureFormatToDXFormat(TextureFormat format)
	{
		switch (format)
		{
			case TextureFormat::R8:					return DXGI_FORMAT_R8_UNORM;
			case TextureFormat::RG8:				return DXGI_FORMAT_R8G8_UNORM;
			case TextureFormat::RGB8:				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::RGBA8:				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case TextureFormat::R32F:				return DXGI_FORMAT_R32_FLOAT;
			case TextureFormat::RG32F:				return DXGI_FORMAT_R32G32_FLOAT;
			case TextureFormat::RGB32F:				return DXGI_FORMAT_R32G32B32_FLOAT;
			case TextureFormat::RGBA32F:			return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case TextureFormat::Depth24Stencil8:	return DXGI_FORMAT_R24G8_TYPELESS;
			case TextureFormat::Depth32:			return DXGI_FORMAT_R32_TYPELESS;
		}

		METEOR_ENGINE_ASSERT(false, "Unknown texture format!");
		return DXGI_FORMAT_UNKNOWN;
	}

	static D3D11_TEXTURE_ADDRESS_MODE WrapModeToDXWrapMode(TextureWrap wrap)
	{
		switch (wrap)
		{
		case TextureWrap::Border:		return D3D11_TEXTURE_ADDRESS_BORDER;
		case TextureWrap::Clamp:		return D3D11_TEXTURE_ADDRESS_CLAMP;
		case TextureWrap::Mirror:		return D3D11_TEXTURE_ADDRESS_MIRROR;
		case TextureWrap::Wrap:			return D3D11_TEXTURE_ADDRESS_WRAP;
		}

		METEOR_ENGINE_ASSERT(false, "Unknown wrap mode!");
		return D3D11_TEXTURE_ADDRESS_CLAMP;
	}

	static D3D11_FILTER FilterToDXFilter(TextureFilter filter)
	{
		switch (filter)
		{
			case TextureFilter::Anisotropic:	return D3D11_FILTER_ANISOTROPIC;
			case TextureFilter::Linear:			return D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
			case TextureFilter::Nearest:		return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}

		METEOR_ENGINE_ASSERT(false, "Unknown filter mode!");
		return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}

	static D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopologyToDXTopology(PrimitiveTopology topology)
	{
		switch (topology)
		{
			case PrimitiveTopology::Triangles:	return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case PrimitiveTopology::Lines:		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		}

		METEOR_ENGINE_ASSERT(false, "Unknown filter mode!");
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}

#endif // METEOR_PLATFORM_WINDOWS

}
}