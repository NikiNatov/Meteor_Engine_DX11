#pragma once

#include "meteor\core\base.h"
#include "meteor\core\timestep.h"

#include "entity.h"

namespace meteor
{
    class Script
    {
        friend class Scene;
    public:
        Script() = default;
        virtual ~Script() = default;

        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

        inline Entity GetEntity() { return m_Entity; }
    protected:
        virtual void OnCreate() {}
        virtual void OnUpdate(Timestep ts) {};
        virtual void OnDestroy() {};
    private:
        Entity m_Entity;
    };

}