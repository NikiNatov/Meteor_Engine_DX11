#include "pch.h"
#include "camera.h"

#include <glm\gtc\matrix_transform.hpp>

namespace meteor
{
    // --------------------------------------------------------------------------------------------------------------------------------------
    Camera::Camera()
    {
        RecalculateProjection();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    Camera::Camera(mtrFloat fov, mtrFloat aspectRatio, mtrFloat nearPlane /*= 0.1f*/, mtrFloat farPlane /*= 1000.0f*/)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(nearPlane), m_Far(farPlane)
    {
        RecalculateProjection();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Camera::SetPerspective(mtrFloat fov, mtrFloat nearPlane, mtrFloat farPlane)
    {
        m_FOV = fov;
        m_Near = nearPlane;
        m_Far = farPlane;

        RecalculateProjection();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Camera::SetViewport(mtrUInt width, mtrUInt height)
    {
        if (height == 0)
            return;

        m_AspectRatio = (mtrFloat)width / (mtrFloat)height;
        RecalculateProjection();
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Camera::RecalculateProjection()
    {
        m_ProjMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
    }
}
