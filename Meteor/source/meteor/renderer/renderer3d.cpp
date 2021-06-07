#include "pch.h"
#include "renderer3d.h"

#include <glm\gtc\type_ptr.hpp>
#include "meteor\renderer\api\gfxrendererapi.h"

#include "meteor\renderer\api\gfxcontext.h"

namespace meteor
{
    ScopedPtr<Renderer3D::Renderer3DData> Renderer3D::ms_Data = CreateScoped<Renderer3D::Renderer3DData>();
    std::vector<RenderCommand>            Renderer3D::ms_CommandQueueSolid;
    std::vector<RenderCommand>            Renderer3D::ms_CommandQueueTransparent;
    std::vector<RenderCommand>            Renderer3D::ms_CommandQueueWater;

    enum VSSystemUniformIndices
    {
        VSUniformIndex_ProjectionMatrix = 0,
        VSUniformIndex_ViewMatrix       = 1,
        VSUniformIndex_ModelMatrix      = 2,
        VSSystemUniformCount
    };

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Renderer3D::Initialize()
    {
        ms_CommandQueueSolid.reserve(1000);
        ms_CommandQueueTransparent.reserve(1000);
        ms_CommandQueueWater.reserve(1000);

        ms_Data->VSSystemUniformBufferSize = sizeof(glm::mat4) * 3;
        ms_Data->VSSystemUniformBufferData = new mtrByte[ms_Data->VSSystemUniformBufferSize];

        ms_Data->VSUniformOffsets.resize(VSSystemUniformCount);
        ms_Data->VSUniformOffsets[VSUniformIndex_ProjectionMatrix] = 0;
        ms_Data->VSUniformOffsets[VSUniformIndex_ViewMatrix] = ms_Data->VSUniformOffsets[VSUniformIndex_ProjectionMatrix] + sizeof(glm::mat4);
        ms_Data->VSUniformOffsets[VSUniformIndex_ModelMatrix] = ms_Data->VSUniformOffsets[VSUniformIndex_ViewMatrix] + sizeof(glm::mat4);

        // TODO: PSSystemUniformBuffer
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Renderer3D::BeginScene(Camera& camera, const glm::mat4& cameraTransform)
    {
        RendererAPI::ClearTargets(COLOR_BUFFER | DEPTH_BUFFER);

        ms_CommandQueueSolid.clear();
        ms_CommandQueueTransparent.clear();
        ms_CommandQueueWater.clear();

        ms_Data->ProjectionMatrix = camera.GetProjection();
        ms_Data->ViewMatrix = glm::inverse(cameraTransform);

        memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ProjectionMatrix], glm::value_ptr(ms_Data->ProjectionMatrix), sizeof(glm::mat4));
        memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ViewMatrix], glm::value_ptr(ms_Data->ViewMatrix), sizeof(glm::mat4));
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Renderer3D::Submit(Ref<Mesh> mesh, const glm::mat4& transform)
    {
        if (mesh)
        {
            RenderCommand command;
            command.Mesh = mesh;
            command.Transform = transform;
            command.Shader = mesh->GetMaterial()->GetMaterial()->GetShader();

            if (mesh->GetMaterial()->GetRenderFlags() & Material::Transparent && !(mesh->GetMaterial()->GetRenderFlags() & Material::Water))
                ms_CommandQueueTransparent.push_back(command);
            else if (mesh->GetMaterial()->GetRenderFlags() & Material::Water)
                ms_CommandQueueWater.push_back(command);
            else
                ms_CommandQueueSolid.push_back(command);
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Renderer3D::Present()
    {
        glm::vec3 cameraPos = glm::inverse(ms_Data->ViewMatrix)[3];

        for (RenderCommand& command : ms_CommandQueueSolid)
        {
            // Set model matrix
            memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ModelMatrix], glm::value_ptr(command.Transform), sizeof(glm::mat4));

            // Set render states
            SetSystemUniforms(command.Shader);

            command.Mesh->Render();
        }

        // If under water level render transparent objects first and then water
        if (cameraPos.y < 40.0f)
        {
            for (RenderCommand& command : ms_CommandQueueTransparent)
            {
                // Set model matrix
                memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ModelMatrix], glm::value_ptr(command.Transform), sizeof(glm::mat4));

                // Set render states
                SetSystemUniforms(command.Shader);

                command.Mesh->Render();
            }

            for (RenderCommand& command : ms_CommandQueueWater)
            {
                // Set model matrix
                memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ModelMatrix], glm::value_ptr(command.Transform), sizeof(glm::mat4));

                // Set render states
                SetSystemUniforms(command.Shader);

                command.Mesh->Render();
            }
        }
        else
        {
            for (RenderCommand& command : ms_CommandQueueWater)
            {
                // Set model matrix
                memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ModelMatrix], glm::value_ptr(command.Transform), sizeof(glm::mat4));

                // Set render states
                SetSystemUniforms(command.Shader);

                command.Mesh->Render();
            }

            for (RenderCommand& command : ms_CommandQueueTransparent)
            {
                // Set model matrix
                memcpy(ms_Data->VSSystemUniformBufferData + ms_Data->VSUniformOffsets[VSUniformIndex_ModelMatrix], glm::value_ptr(command.Transform), sizeof(glm::mat4));

                // Set render states
                SetSystemUniforms(command.Shader);

                command.Mesh->Render();
            }
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Renderer3D::SetSystemUniforms(Ref<Shader> shader)
    {
        shader->SetVSSystemUniformBuffer(ms_Data->VSSystemUniformBufferData, ms_Data->VSSystemUniformBufferSize);
        // TODO: Set pixel shader uniform buffer
        //shader->SetPSSystemUniformBuffer(ms_Data->PSSystemUniformBufferData, ms_Data->PSSystemUniformBufferSize);
    }
}
