#pragma once

#include "meteor\core\base.h"

namespace meteor
{
    struct SwapChainDescription
    {
        mtrUInt BufferCount;
        mtrUInt Width;
        mtrUInt Height;
        mtrBool Fullscreen;
        mtrBool VSync;
    };

    class GfxSwapChain
    {
    public:
        virtual ~GfxSwapChain() = default;

        virtual void                         ClearBackBuffer() = 0;
        virtual void                         ClearDepthBuffer() = 0;
        virtual void						 Present() = 0;
        virtual void                         ToogleVSync() = 0;
        virtual void                         ToggleFullscreen() = 0;
        virtual void                         Resize(mtrUInt width, mtrUInt height) = 0;

        virtual mtrBool						 IsFullscreen() const = 0;
        virtual mtrBool						 IsVSync() const = 0;
        virtual mtrUInt                      GetBufferCount() const = 0;
        virtual mtrUInt						 GetWidth() const = 0;
        virtual mtrUInt						 GetHeight() const = 0;
        virtual const SwapChainDescription&	 GetDescription() const = 0;

    };
}