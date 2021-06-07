#pragma once

#include "meteor\core\base.h"
#include <glm\glm.hpp>

namespace meteor
{
    class Camera
    {
    public:
        Camera();
        Camera(mtrFloat fov, mtrFloat aspectRatio, mtrFloat nearPlane = 0.1f, mtrFloat farPlane = 1000.0f);

        void                        SetPerspective(mtrFloat fov, mtrFloat nearPlane, mtrFloat farPlane);
        void                        SetViewport(mtrUInt width, mtrUInt height);

        inline void                 SetNearPlane(mtrFloat nearPlane) { m_Near = nearPlane; RecalculateProjection(); }
        inline void                 SetFarPlane(mtrFloat farPlane) { m_Far = farPlane; RecalculateProjection(); }
        inline void                 SetFov(mtrFloat fov) { m_FOV = fov; RecalculateProjection(); }

        inline mtrFloat             GetNearPlane() const { return m_Near; }
        inline mtrFloat             GetFarPlane() const { return m_Far; }
        inline mtrFloat             GetFov() const { return m_FOV; }
        inline const glm::mat4&     GetProjection() const { return m_ProjMatrix; }
    private:
        void                        RecalculateProjection();
    private:
        mtrFloat                    m_AspectRatio = 16.0f / 9.0f;
        mtrFloat                    m_Near = 0.01f;
        mtrFloat                    m_Far = 1000.0f;
        mtrFloat                    m_FOV = 60.0f;

        glm::mat4                   m_ProjMatrix = glm::mat4(1.0f);
    };
}