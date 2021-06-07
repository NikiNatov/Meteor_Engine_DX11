#pragma once

#include "meteor\core\base.h"
#include "meteor\renderer\mesh.h"
#include "meteor\renderer\camera.h"
#include "script.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

namespace meteor
{
    struct TagComponent
    {
        mtrString Name;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const mtrString& name)
            : Name(name)
        {
        }
    };

    struct ScriptComponent
    {
        Script* Instance = nullptr;

        Script* (*CreateScript)();
        void (*DestroyScript)(ScriptComponent*);

        template<typename T>
        void Bind()
        {
            CreateScript = []() { return static_cast<Script*>(new T()); };
            DestroyScript = [](ScriptComponent* script) { delete script->Instance; script->Instance = nullptr; };
        }
    };

    struct TransformComponent
    {
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Rotation = glm::vec3(0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
            : Position(position), Rotation(rotation), Scale(scale)
        {
        }

        glm::mat4 GetTransform()
        {
            glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                 glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                 glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);

            return translation * rotation * scale;
        }

        glm::vec3 GetUpVector()
        {
            return glm::normalize(GetOrientation() * glm::vec3(0.0f, 1.0f, 0.0f));
        }

        glm::vec3 GetRightVector()
        {
            return glm::normalize(GetOrientation() * glm::vec3(1.0f, 0.0f, 0.0f));
        }

        glm::vec3 GetForwardVector()
        {
            return glm::normalize(GetOrientation() * glm::vec3(0.0f, 0.0f, -1.0f));
        }

        glm::quat GetOrientation()
        {
            return glm::normalize(glm::angleAxis(glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                  glm::angleAxis(glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)));
        }
    };

    struct CameraComponent
    {
        Camera Camera;
        mtrBool IsPrimary = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct MeshRendererComponent
    {
        std::vector<Ref<Mesh>> Meshes;

        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent&) = default;
    };
}