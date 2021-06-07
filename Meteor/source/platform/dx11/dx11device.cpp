#include "pch.h"
#include "dx11device.h"

#include "dx11swapchain.h"
#include "dx11buffer.h"
#include "dx11shader.h"
#include "dx11texture.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

	// --------------------------------------------------------------------------------------------------------------------------------------
	DX11Device::DX11Device()
	{
		mtrUInt deviceCreateFlags = 0;

#if defined(METEOR_DEBUG)
		deviceCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		DX_CALL(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceCreateFlags,
								  NULL, 0, D3D11_SDK_VERSION, &m_Device, NULL, &m_DeviceImmediateContext));
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Ref<VertexBuffer> DX11Device::CreateVertexBuffer(const void* data, mtrUInt size)
	{
		return CreateRef<DX11VertexBuffer>(*this, data, size);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Ref<IndexBuffer> DX11Device::CreateIndexBuffer(const mtrU32* data, mtrUInt count)
	{
		return CreateRef<DX11IndexBuffer>(*this, data, count);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Ref<Shader> DX11Device::CreateShader(const mtrString& filepath)
	{
		return CreateRef<DX11Shader>(*this, filepath);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Ref<Texture2D> DX11Device::CreateTexture2D(const mtrString& filepath, const TextureSpecification& spec)
	{
		return CreateRef<DX11Texture2D>(*this, filepath, spec);
	}

	// --------------------------------------------------------------------------------------------------------------------------------------
	Ref<Texture2D> DX11Device::CreateTexture2D(mtrU32 width, mtrU32 height, const TextureSpecification& spec)
	{
		return CreateRef<DX11Texture2D>(*this, width, height, spec);
	}

#endif // METEOR_PLATFORM_WINDOWS
}