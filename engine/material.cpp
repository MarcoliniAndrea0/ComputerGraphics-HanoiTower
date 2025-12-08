#include "material.h"
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Costruttore di default per la classe `Material`.
 *
 * Crea una nuova istanza di `Material` con i seguenti parametri di default:
 * - Colore di emissione: (0.0f, 0.0f, 0.0f)
 * - Colore ambientale: (0.75f, 0.75f, 0.75f)
 * - Colore diffuso: (0.75f, 0.75f, 0.75f)
 * - Colore speculare: (0.75f, 0.75f, 0.75f)
 * - Lucentezza: 64.0f
 * - Valore alpha: 1.0f
 * - Texture: Nessuna texture applicata.
 */
Material::Material()
    : Object("Material")
{
    this->setEmissionColor(glm::vec3(0.0f, 0.0f, 0.0f));
    this->setAmbientColor(glm::vec3(0.75f, 0.75f, 0.75f));
    this->setDiffuseColor(glm::vec3(0.75f, 0.75f, 0.75f));
    this->setSpecularColor(glm::vec3(0.75f, 0.75f, 0.75f));
    this->setShininess(64.0f);
    this->setAlpha(1.0f);
    this->setTexture(nullptr);
}

// Getter

glm::vec3 LIB_API Material::getEmissionColor() const {
    return _emissionColor;
}

// Setter

void LIB_API Material::setEmissionColor(const glm::vec3 newColor) {
    this->_emissionColor = newColor;
}

void LIB_API Material::setAmbientColor(const glm::vec3 newColor) {
    this->_ambientColor = newColor;
}

void LIB_API Material::setDiffuseColor(const glm::vec3 newColor) {
    this->_diffuseColor = newColor;
}

void LIB_API Material::setSpecularColor(const glm::vec3 newColor) {
    this->_specularColor = newColor;
}

void LIB_API Material::setShininess(const float newShininess) {
    this->_shininess = newShininess;
}

void LIB_API Material::setAlpha(const float newAlpha) {
    this->_alpha = newAlpha;
}

void LIB_API Material::setTexture(const std::shared_ptr<Texture> newTexture) {
    this->_texture = newTexture;
}

// Render Material

void LIB_API Material::render(const glm::mat4 viewMatrix) const {
    glDisable(GL_TEXTURE_2D);

    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(this->_emissionColor));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(this->_ambientColor));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(this->_diffuseColor));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(this->_specularColor));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, this->_shininess);

    if (this->_texture != nullptr) {
        this->_texture->render(viewMatrix);
    }
}
