#include "light.h"

#include <GL/freeglut.h>
#include <iostream>

// Tiene traccia del prossimo ID da assegnare a una nuova luce.
int Light::nextLightId = 0;

/**
 * @brief Costruttore della classe `Light`.
 * @param type Il tipo di luce (es. "PointLight", "SpotLight").
 */
Light::Light(const std::string& type)
    : Node{ type }
{
    this->setPriority(1);

    int maxNumberOfLights;
    glGetIntegerv(GL_MAX_LIGHTS, &maxNumberOfLights);

    this->_lightId = Light::nextLightId;
    std::cout << "This light: " << this->_lightId << std::endl;

    if (this->_lightId >= maxNumberOfLights)
    {
        WARNING("Maximum number of lights exceeded: (" << maxNumberOfLights << ").");
        return;
    }

    this->setAmbientColor(glm::vec3(0.0f, 0.0f, 0.0f));
    this->setDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
    this->setSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));

    Light::nextLightId++;
}

/**
 * @brief Distruttore della classe `Light`.
 */
Light::~Light()
{
    nextLightId--;
}

// Getter

/**
 * @brief Restituisce il numero della luce OpenGL associato all'ID.
 * @param lightId L'ID della luce.
 * @return Il numero della luce OpenGL.
 */
int LIB_API Light::getCurrentLight(const int lightId) const
{
    return GL_LIGHT0 + lightId;
}

/**
 * @brief Restituisce il colore ambientale della luce.
 */
glm::vec3 LIB_API Light::getAmbientColor() const
{
    return _ambientColor;
}

/**
 * @brief Restituisce il colore diffuso della luce.
 */
glm::vec3 LIB_API Light::getDiffuseColor() const
{
    return _diffuseColor;
}

/**
 * @brief Restituisce il colore speculare della luce.
 */
glm::vec3 LIB_API Light::getSpecularColor() const
{
    return _specularColor;
}

// Setter

/**
 * @brief Imposta il colore ambientale della luce.
 */
void LIB_API Light::setAmbientColor(const glm::vec3 newColor)
{
    this->_ambientColor = newColor;
}

/**
 * @brief Imposta il colore diffuso della luce.
 */
void LIB_API Light::setDiffuseColor(const glm::vec3 newColor)
{
    this->_diffuseColor = newColor;
}

/**
 * @brief Imposta il colore speculare della luce.
 */
void LIB_API Light::setSpecularColor(const glm::vec3 newColor)
{
    this->_specularColor = newColor;
}

/**
 * @brief Resetta il contatore degli ID delle luci.
 */
void LIB_API Light::resetNextLightId()
{
    Light::nextLightId = 0;
}
