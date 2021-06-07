#include "pch.h"
#include "scene.h"

#include "entity.h"
#include "components.h"

#include "meteor\renderer\renderer3d.h"

namespace meteor
{
    // --------------------------------------------------------------------------------------------------------------------------------------
    Entity Scene::CreateEntity(const mtrString& tag)
    {
        Entity newEntity(m_Registry.create(), this);
        newEntity.AddComponent<TagComponent>(tag);
        newEntity.AddComponent<TransformComponent>();
        return newEntity;
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Entity Scene::FindEntity(const mtrString& tag)
    {
        auto view = m_Registry.view<TagComponent>();

        for (auto entity : view)
        {
            auto& tagComponent = view.get<TagComponent>(entity);
            if (tagComponent.Name == tag)
                return Entity(entity, this);
        }

        return Entity();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Scene::DeleteEntity(Entity entity)
    {
        m_Registry.destroy((entt::entity)entity);
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Scene::OnSceneStart()
    {
        m_Registry.view<ScriptComponent>().each([=](auto entityID, auto& scriptComponent)
        {
            if (!scriptComponent.Instance)
            {
                scriptComponent.Instance = scriptComponent.CreateScript();
                scriptComponent.Instance->m_Entity = Entity(entityID, this);
                scriptComponent.Instance->OnCreate();
            }
        });
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Scene::OnSceneUpdate(Timestep ts)
    {
        m_Registry.view<ScriptComponent>().each([=](auto entityID, auto& scriptComponent)
        {
            scriptComponent.Instance->OnUpdate(ts);
        });
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Scene::OnSceneRender()
    {
        auto cameraView = m_Registry.view<CameraComponent, TransformComponent>();
        glm::mat4 cameraTransform;

        for (auto entity : cameraView)
        {
            auto [cameraComponent, transformComponent] = cameraView.get<CameraComponent, TransformComponent>(entity);
            if (cameraComponent.IsPrimary)
            {
                m_Camera = &cameraComponent.Camera;
                cameraTransform = transformComponent.GetTransform();
            }
        }

        if (m_Camera)
        {
            Renderer3D::BeginScene(*m_Camera, cameraTransform);

            auto meshView = m_Registry.view<MeshRendererComponent, TransformComponent>();

            for (auto entity : meshView)
            {
                auto [meshComponent, transformComponent] = meshView.get<MeshRendererComponent, TransformComponent>(entity);
                for(auto& mesh : meshComponent.Meshes)
                    Renderer3D::Submit(mesh, transformComponent.GetTransform());
            }

            Renderer3D::Present();
        }
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Scene::OnSceneEnd()
    {
        m_Registry.view<ScriptComponent>().each([=](auto entityID, auto& scriptComponent)
        {
            if (scriptComponent.Instance)
            {
                scriptComponent.Instance->OnDestroy();
                scriptComponent.DestroyScript(&scriptComponent);
            }
        });
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Scene::OnViewportResize(mtrU32 width, mtrU32 height)
    {
        m_Camera->SetViewport(width, height);
    }
}
