#include "engine.h"
#include "prospectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const std::string& name, float fov, float aspect, float nearPlane, float farPlane)
    : Camera(name), m_fov(fov), m_aspect(aspect), m_near(nearPlane), m_far(farPlane)
{
    this->setType("PerspectiveCamera");
}

glm::mat4 PerspectiveCamera::getProjectionMatrix() const
{
    // glm::perspective richiede il FOV in radianti
    return glm::perspective(glm::radians(m_fov), m_aspect, m_near, m_far);
}

void PerspectiveCamera::setFov(float fov) 
{
    m_fov = fov; 
}

void PerspectiveCamera::setAspectRatio(float aspect) 
{ 
    m_aspect = aspect; 
}

void PerspectiveCamera::setNear(float nearPlane)
{ 
    m_near = nearPlane;
}

void PerspectiveCamera::setFar(float farPlane)
{
    m_far = farPlane;
}
