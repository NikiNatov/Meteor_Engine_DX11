#pragma once

#include "meteor\core\base.h"

namespace meteor
{
	enum class TextureWrap
	{
		None = 0,

		Wrap,
		Mirror,
		Border,
		Clamp
	};

	enum class TextureFilter
	{
		None = 0,

		Anisotropic,
		Linear,
		Nearest
	};

	enum class TextureFormat
	{
		None = 0,

		R8, R32F,
		RG8, RG32F,
		RGB8, RGB32F,
		RGBA8, RGBA32F,

		Depth24Stencil8, Depth32
	};

	struct TextureSpecification
	{
		TextureFormat		Format = TextureFormat::RGBA8;
		TextureWrap			Wrap = TextureWrap::Clamp;
		TextureFilter		Filter = TextureFilter::Linear;
	};

    class Texture
    {
    public:
        virtual ~Texture() = default;

		virtual void							Bind(mtrU32 slot) = 0;

		virtual const mtrString&				GetFilepath() const = 0;
		virtual const TextureSpecification&		GetSpecification() const = 0;
		virtual void*							GetTextureHandle() const = 0;

    };

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual mtrU32	GetWidth() const = 0;
		virtual mtrU32	GetHeight() const = 0;

		virtual void	SetData(const void* data) = 0;
	};
}