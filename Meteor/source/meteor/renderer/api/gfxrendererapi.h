#pragma once

#include <glm\glm.hpp>

#include "meteor\core\base.h"
#include "meteor\renderer\api\gfxbuffer.h"

namespace meteor
{
    enum class BlendFunction
    {
        None = 0,

        ZERO,
        ONE,
        DEST_ALPHA,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA
    };

    enum class BlendEquation
    {
        None = 0,

        ADD,
        SUBTRACT,
    };

    enum RenderBufferTarget
    {
        COLOR_BUFFER    = BIT(0),
        DEPTH_BUFFER    = BIT(1),
    };

    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0,
            DirectX11
        };
    public:
        virtual ~RendererAPI() = default;

        static void                     Initialize();
        inline static void              ClearTargets(mtrU32 targets) { ms_Instance->ClearTargetsInternal(targets); }
        inline static void              DrawIndexed(Ref<IndexBuffer> ib) { ms_Instance->DrawIndexedInternal(ib); }

        inline static void              SetViewport(mtrUInt x, mtrUInt y, mtrUInt width, mtrUInt height) { ms_Instance->SetViewportInternal(x, y, width, height); }
        inline static void              SetBlendState(mtrBool state) { ms_Instance->SetBlendStateInternal(state); }
        inline static void              SetDepthStencilTest(mtrBool state) { ms_Instance->SetDepthStencilTestInternal(state); }
        inline static void              SetWireframeMode(mtrBool state) { ms_Instance->SetWireframeModeInternal(state); }

        static API                      GetAPI() { return ms_API; }
    protected:
        virtual void                    InitializeInternal() = 0;
        virtual void                    ClearTargetsInternal(mtrU32 targets) = 0;
        virtual void                    DrawIndexedInternal(Ref<IndexBuffer> ib) = 0;

        virtual void                    SetViewportInternal(mtrUInt x, mtrUInt y, mtrUInt width, mtrUInt height) = 0;
        virtual void                    SetBlendStateInternal(mtrBool state) = 0;
        virtual void                    SetDepthStencilTestInternal(mtrBool state) = 0;
        virtual void                    SetWireframeModeInternal(mtrBool state) = 0;
    protected:
        static ScopedPtr<RendererAPI>   ms_Instance;
        static API                      ms_API;
    };
}