#include "spotLight.h"
#include "engine.h"

SpotLight::SpotLight(const std::string& name)
    : PointLight(name, "SpotLight")
{
    // Default: Punta verso il basso, cono di 45 gradi
    m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
    m_cutoff = 45.0f;
    m_exponent = 20.0f;
}

void SpotLight::render() {
    // Chiamiamo il render del padre per impostare Posizione, Colori e Attenuazione
    PointLight::render();

    int id = getLightNumber();

    // Impostiamo i parametri specifici Spot
    glm::vec4 dir4(m_direction, 0.0f); // w=0 direzione
    glLightfv(id, GL_SPOT_DIRECTION, glm::value_ptr(dir4));

    glLightf(id, GL_SPOT_CUTOFF, m_cutoff);
    glLightf(id, GL_SPOT_EXPONENT, m_exponent);
}

void SpotLight::setDirection(const glm::vec3& dir) 
{
    m_direction = dir; 
}

void SpotLight::setCutoff(float degrees) 
{
    m_cutoff = degrees; 
}

void SpotLight::setExponent(float exp) 
{
    m_exponent = exp; 
}


glm::vec3 SpotLight::getDirection() const 
{
    return m_direction; 
}

float SpotLight::getCutoff() const 
{
    return m_cutoff;
}

float SpotLight::getExponent() const 
{
    return m_exponent; 
}
