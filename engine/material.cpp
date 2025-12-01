#include "Material.h"
#include "engine.h"
#include <iostream>

Material::Material(const std::string& name, const glm::vec4& emission, const glm::vec4& ambient, const glm::vec4& diffuse, const glm::vec4& specular, float shininess)
    : Object(name, "Material"), // Inizializza la classe base Object
    m_emission(emission),
    m_ambient(ambient),
    m_diffuse(diffuse),
    m_specular(specular),
    m_shininess(shininess),
    m_texture(nullptr)
{
}

Material::~Material() {
    // Se il materiale possiede una texture, la distrugge per liberare memoria.
    if (m_texture != nullptr) {
        // delete m_texture; // <--- SCOMMENTARE quando avrai la classe Texture
        m_texture = nullptr;
    }
}

void Material::render()
{
    // Imposta i parametri del materiale in OpenGL per entrambe le facce (Front & Back)
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, glm::value_ptr(m_emission));
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(m_ambient));
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(m_diffuse));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(m_specular));
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

    // Gestione Texture
    /*
    if (m_texture) {
        glEnable(GL_TEXTURE_2D);
        m_texture->render(); // Assumendo che Texture abbia un metodo render() o bind()
    } else {
        glDisable(GL_TEXTURE_2D);
    }
    */
}

// --- Implementazione Setters ---

void Material::setEmission(const glm::vec4& color) 
{ 
    m_emission = color; 
}

void Material::setAmbient(const glm::vec4& color)
{ 
    m_ambient = color; 
}

void Material::setDiffuse(const glm::vec4& color)
{ 
    m_diffuse = color; 
}

void Material::setSpecular(const glm::vec4& color) 
{ 
    m_specular = color; 
}

void Material::setShininess(float shininess) 
{
    // OpenGL accetta valori tra 0 e 128
    if (shininess < 0.0f) m_shininess = 0.0f;
    else if (shininess > 128.0f) m_shininess = 128.0f;
    else m_shininess = shininess;
}
/*
void Material::setTexture(Texture* texture)
{ 
    if (m_texture != nullptr && m_texture != texture) {
        delete m_texture;
    }
    m_texture = texture; 
}
*/

// --- Implementazione Getters ---

glm::vec4 Material::getEmission() const 
{ 
    return m_emission; 
}

glm::vec4 Material::getAmbient() const 
{ 
    return m_ambient; 
}

glm::vec4 Material::getDiffuse() const 
{ 
    return m_diffuse; 
}

glm::vec4 Material::getSpecular() const 
{ 
    return m_specular; 
}

float Material::getShininess() const 
{ 
    return m_shininess; 
}

/*
Texture* Material::getTexture() const 
{ 
    return m_texture; 
}
*/
