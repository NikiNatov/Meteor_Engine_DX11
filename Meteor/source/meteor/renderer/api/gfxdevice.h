#pragma once

#include "meteor\core\base.h"

namespace meteor
{
	class VertexBuffer;
	class IndexBuffer;
	class Shader;
	class Texture;
	class Texture2D;
	class TextureCube;
	struct TextureSpecification;

	class GfxDevice
	{
	public:
		virtual ~GfxDevice() = default;

		virtual Ref<VertexBuffer>	CreateVertexBuffer(const void* data, mtrUInt size) = 0;
		virtual Ref<IndexBuffer>	CreateIndexBuffer(const mtrU32* data, mtrUInt count) = 0;
		virtual Ref<Shader>			CreateShader(const mtrString& filepath) = 0;

		virtual Ref<Texture2D>		CreateTexture2D(const mtrString& filepath, const TextureSpecification& spec) = 0;
		virtual Ref<Texture2D>		CreateTexture2D(mtrU32 width, mtrU32 height, const TextureSpecification& spec) = 0;
	};
}