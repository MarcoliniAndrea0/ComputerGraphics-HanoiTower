#pragma once

#include "camera.h"

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(const std::string& name, float fov, float aspect, float nearPlane, float farPlane);

    ~PerspectiveCamera() = default;

    glm::mat4 getProjectionMatrix() const override;

    // --- Setters per modificare la lente a runtime ---
    void setFov(float fov);
    void setAspectRatio(float aspect);
    void setNear(float nearPlane);
    void setFar(float farPlane);

    // --- Getters ---
    float getFov() const
    { 
        return m_fov; 
    }
    
    float getAspectRatio() const 
    { 
        return m_aspect; 
    }

    float getNear() const 
    { 
        return m_near; 
    }

    float getFar() const 
    { 
        return m_far; 
    }

private:
    float m_fov;    ///< Field of View in gradi
    float m_aspect; ///< Aspect Ratio
    float m_near;   ///< Near clipping plane
    float m_far;    ///< Far clipping plane
};