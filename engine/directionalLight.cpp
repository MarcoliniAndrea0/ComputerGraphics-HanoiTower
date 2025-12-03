#include "directionalLight.h"
#include "engine.h"

DirectionalLight::DirectionalLight(const std::string& name)
    : Light(name, "DirectionalLight")
{
    // Default: luce che viene dall'alto (asse Y)
    m_direction = glm::vec3(0.0f, 1.0f, 0.0f);
}

void DirectionalLight::setDirection(const glm::vec3& direction) {
    m_direction = direction;
}

glm::vec3 DirectionalLight::getDirection() const {
    return m_direction;
}

void DirectionalLight::render() {
    int lightNum = getLightNumber();

    
    glEnable(lightNum);

    // Imposta i colori
    glm::vec4 ambient4(m_ambient, 1.0f);
    glm::vec4 diffuse4(m_diffuse, 1.0f);
    glm::vec4 specular4(m_specular, 1.0f);

    glLightfv(lightNum, GL_AMBIENT, glm::value_ptr(ambient4));
    glLightfv(lightNum, GL_DIFFUSE, glm::value_ptr(diffuse4));
    glLightfv(lightNum, GL_SPECULAR, glm::value_ptr(specular4));

    // Imposta la posizione/direzione
    // Importante: w = 0.0f indica che è una luce direzionale!
    // Nota: OpenGL trasformerà questo vettore con la matrice ModelView corrente.
    // Poiché la RenderList ha già applicato le trasformazioni del nodo, 
    // possiamo passare la direzione locale.
    glm::vec4 pos(m_direction, 0.0f);
    glLightfv(lightNum, GL_POSITION, glm::value_ptr(pos));
}