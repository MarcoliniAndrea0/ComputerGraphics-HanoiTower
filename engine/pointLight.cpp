#include "pointLight.h"
#include "engine.h"

PointLight::PointLight(const std::string& name, const std::string& type)
    : Light(name, type)
{
    // Default: posizione all'origine locale (0,0,0)
    m_position = glm::vec3(0.0f, 0.0f, 0.0f);

    // Default attenuazione: Nessuna (luce infinita) -> Costante=1, altre=0
    m_constAtt = 1.0f;
    m_linAtt = 0.0f;
    m_quadAtt = 0.0f;
}

void PointLight::render() {
    int id = getLightNumber();
    glEnable(id);

    // Colori (dalla classe base Light)
    glm::vec4 ambient4(m_ambient, 1.0f);
    glm::vec4 diffuse4(m_diffuse, 1.0f);
    glm::vec4 specular4(m_specular, 1.0f);

    glLightfv(id, GL_AMBIENT, glm::value_ptr(ambient4));
    glLightfv(id, GL_DIFFUSE, glm::value_ptr(diffuse4));
    glLightfv(id, GL_SPECULAR, glm::value_ptr(specular4));

    // Posizione
    // w = 1.0f -> Posizione puntiforme
    // Nota: Viene trasformata dalla ModelView Matrix corrente (quindi segue il nodo)
    glm::vec4 pos4(m_position, 1.0f);
    glLightfv(id, GL_POSITION, glm::value_ptr(pos4));

    // Attenuazione
    glLightf(id, GL_CONSTANT_ATTENUATION, m_constAtt);
    glLightf(id, GL_LINEAR_ATTENUATION, m_linAtt);
    glLightf(id, GL_QUADRATIC_ATTENUATION, m_quadAtt);

    // Reset Spot (nel caso questa luce ID fosse usata come spot prima)
    // 180 gradi = omnidirezionale
    glLightf(id, GL_SPOT_CUTOFF, 180.0f);
}

void PointLight::setPosition(const glm::vec3& pos) 
{
    m_position = pos;
}

void PointLight::setConstantAttenuation(float v) 
{ 
    m_constAtt = v;
}

void PointLight::setLinearAttenuation(float v)
{ 
    m_linAtt = v; 
}

void PointLight::setQuadraticAttenuation(float v)
{ 
    m_quadAtt = v;
}


glm::vec3 PointLight::getPosition() const
{
    return m_position;
}

float PointLight::getConstantAttenuation() const 
{
    return m_constAtt; 
}

float PointLight::getLinearAttenuation() const 
{
    return m_linAtt; 
}

float PointLight::getQuadraticAttenuation() const 
{ 
    return m_quadAtt; 
}
