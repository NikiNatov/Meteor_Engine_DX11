#pragma once

#include "meteor\core\base.h"
#include "meteor\core\timestep.h"
#include "meteor\renderer\camera.h"

#include <entt.hpp>

namespace meteor
{
    class Scene
    {
        friend class Entity;
    public:
        Scene() = default;

        Entity          CreateEntity(const mtrString& tag = "Unnamed");
        Entity          FindEntity(const mtrString& tag);
        void            DeleteEntity(Entity entity);

        void            OnSceneStart();
        void            OnSceneUpdate(Timestep ts);
        void            OnSceneRender();
        void            OnSceneEnd();
        void            OnViewportResize(mtrU32 width, mtrU32 height);

        inline const Camera& GetSceneCamera() const { return *m_Camera; }
    private:
        entt::registry  m_Registry;
        Camera*         m_Camera;
    };
}