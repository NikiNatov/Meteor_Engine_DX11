#pragma once

#include "meteor\renderer\api\gfxdevice.h"

#include "dx11common.h"

namespace meteor
{
#if defined(METEOR_PLATFORM_WINDOWS)

	class DX11Device : public GfxDevice
	{
	public:
		DX11Device();

		virtual Ref<VertexBuffer>					CreateVertexBuffer(const void* data, mtrUInt size) override;
		virtual Ref<IndexBuffer>					CreateIndexBuffer(const mtrU32* data, mtrUInt count) override;
		virtual Ref<Shader>							CreateShader(const mtrString& filepath) override;

		virtual Ref<Texture2D>						CreateTexture2D(const mtrString& filepath, const TextureSpecification& spec) override;
		virtual Ref<Texture2D>						CreateTexture2D(mtrU32 width, mtrU32 height, const TextureSpecification& spec) override;

		// DirectX specific functions
		inline wrl::ComPtr<ID3D11Device>			operator*() { return m_Device; }
		inline wrl::ComPtr<ID3D11Device>			GetD3DDevice() { return m_Device; }
		inline wrl::ComPtr<ID3D11DeviceContext>		GetD3DImmediateContext() { return m_DeviceImmediateContext; }
	private:
		wrl::ComPtr<ID3D11Device>					m_Device = nullptr;
		wrl::ComPtr<ID3D11DeviceContext>			m_DeviceImmediateContext = nullptr;
	};

#endif // METEOR_PLATFORM_WINDOWS
}