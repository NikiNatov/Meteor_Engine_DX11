#pragma once

#include "meteor\core\base.h"
#include <entt.hpp>

namespace meteor
{
    class Scene;

    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity id, Scene* scene);

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            METEOR_ENGINE_ASSERT(!HasComponent<T>(), "Entity already has that component!");
            T& component = m_Scene->m_Registry.emplace<T>(m_EntityID, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        void RemoveComponent()
        {
            METEOR_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have that component!");
            m_Scene->m_Registry.remove<T>(m_EntityID);
        }

        template<typename T>
        T& GetComponent()
        {
            METEOR_ENGINE_ASSERT(HasComponent<T>(), "Entity does not have that component!");
            return m_Scene->m_Registry.get<T>(m_EntityID);
        }

        template<typename T>
        bool HasComponent()
        {
            return m_Scene->m_Registry.has<T>(m_EntityID);
        }

        inline Scene* GetScene() const { return m_Scene; }

        operator entt::entity() { return m_EntityID; }
 
        operator mtrBool() const { return m_EntityID != entt::null; }
        mtrBool operator==(const Entity& entity) const { return m_EntityID == entity.m_EntityID && m_Scene == entity.m_Scene; }
        mtrBool operator!=(const Entity& entity) const { return !(*this == entity); }
    private:
        entt::entity    m_EntityID{ entt::null };
        Scene*          m_Scene = nullptr;
    };
}