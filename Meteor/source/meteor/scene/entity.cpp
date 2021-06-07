#include "pch.h"
#include "entity.h"

namespace meteor
{
    // --------------------------------------------------------------------------------------------------------------------------------------
    Entity::Entity(entt::entity id, Scene* scene)
        : m_EntityID(id), m_Scene(scene)
    {     
    }
}
