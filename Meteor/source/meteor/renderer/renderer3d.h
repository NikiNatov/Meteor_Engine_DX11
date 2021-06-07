#pragma once

#include "meteor\core\base.h"
#include "meteor\renderer\camera.h"
#include "meteor\renderer\mesh.h"
#include "meteor\renderer\api\gfxshader.h"
#include "meteor\renderer\api\gfxbuffer.h"

namespace meteor
{
    struct RenderCommand
    {
        Ref<Mesh>   Mesh;
        glm::mat4   Transform;
        Ref<Shader> Shader;
    };

    class Renderer3D
    {
    public:
        struct Renderer3DData
        {
            mtrByte*            VSSystemUniformBufferData;
            mtrU32              VSSystemUniformBufferSize;
            std::vector<mtrU32> VSUniformOffsets;

            mtrByte*            PSSystemUniformBufferData;
            mtrU32              PSSystemUniformBufferSize;
            std::vector<mtrU32> PSUniformOffsets;

            glm::mat4           ProjectionMatrix;
            glm::mat4           ViewMatrix;
        };
    public:
        static void                         Initialize();
        static void                         BeginScene(Camera& camera, const glm::mat4& cameraTransform);

        static void                         Submit(Ref<Mesh> mesh, const glm::mat4& transform);
        static void                         Present();
    private:
        static void                         SetSystemUniforms(Ref<Shader> shader);
    private:
        static ScopedPtr<Renderer3DData>    ms_Data;
        static std::vector<RenderCommand>   ms_CommandQueueSolid;
        static std::vector<RenderCommand>   ms_CommandQueueTransparent;
        static std::vector<RenderCommand>   ms_CommandQueueWater;
    };
}