#pragma once

#include "node.h"
#include "engine.h"
#include <glm/glm.hpp>

class ENG_API Light : public Node
{
public:
    Light(const std::string& name, const std::string& type);

    virtual ~Light();

    // Getter
    int getLightNumber() const; // Restituisce GL_LIGHT0 + id
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;

    // Setter
    void setAmbient(const glm::vec3& color);
    void setDiffuse(const glm::vec3& color);
    void setSpecular(const glm::vec3& color);

protected:
    static int nextLightId;
    int m_lightId;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};