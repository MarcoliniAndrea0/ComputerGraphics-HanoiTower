#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>
#include <glm/glm.hpp>
#include "object.h"
#include "engine.h"

//class Texture;

class ENG_API Material : public Object
{
public:
    Material(const std::string& name,
        const glm::vec4& emission = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
        const glm::vec4& ambient = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
        const glm::vec4& diffuse = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f),
        const glm::vec4& specular = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f),
        float shininess = 10.0f);

    ~Material();

    void render() override;

    // --- Setters ---
    void setEmission(const glm::vec4& color);
    void setAmbient(const glm::vec4& color);
    void setDiffuse(const glm::vec4& color);
    void setSpecular(const glm::vec4& color);
    void setShininess(float shininess);
    //void setTexture(Texture* texture);

    // --- Getters ---
    glm::vec4 getEmission() const;
    glm::vec4 getAmbient() const;
    glm::vec4 getDiffuse() const;
    glm::vec4 getSpecular() const;
    float getShininess() const;
    //Texture* getTexture() const;

private:
    glm::vec4 m_emission;
    glm::vec4 m_ambient;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    float m_shininess;

    //Texture* m_texture;
};

#endif // MATERIAL_H