#include "light.h"
#include "engine.h"
#include <iostream>

int Light::nextLightId = 0;

Light::Light(const std::string& name, const std::string& type)
    : Node(name, type)
{
    // Assegna un ID univoco
    m_lightId = nextLightId++;

    // Verifica limite luci OpenGL
    int maxLights;
    glGetIntegerv(GL_MAX_LIGHTS, &maxLights);

    if (m_lightId >= maxLights) {
        std::cerr << "[WARNING] Numero massimo di luci OpenGL superato!" << std::endl;
    }

    // Colori di default (bianco)
    m_ambient = glm::vec3(0.0f);
    m_diffuse = glm::vec3(1.0f);
    m_specular = glm::vec3(1.0f);
}

Light::~Light() {
    if (m_lightId == nextLightId - 1) {
        nextLightId--;
    }
}

int Light::getLightNumber() const {
    return GL_LIGHT0 + m_lightId;
}

glm::vec3 Light::getAmbient() const 
{
    return m_ambient; 
}

glm::vec3 Light::getDiffuse() const 
{ 
    return m_diffuse; 
}

glm::vec3 Light::getSpecular() const 
{ 
    return m_specular; 
}

void Light::setAmbient(const glm::vec3& color) 
{ 
    m_ambient = color; 
}

void Light::setDiffuse(const glm::vec3& color) 
{ 
    m_diffuse = color; 
}

void Light::setSpecular(const glm::vec3& color) 
{ 
    m_specular = color; 
}
