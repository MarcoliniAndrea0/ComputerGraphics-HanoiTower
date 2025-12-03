#pragma once

#include "pointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight(const std::string& name);
    ~SpotLight() = default;

    void render() override;

    // Setters
    void setDirection(const glm::vec3& dir);
    void setCutoff(float degrees); // Angolo del cono (0-90)
    void setExponent(float exp);   // Sfumatura ai bordi (0-128)

    // Getters
    glm::vec3 getDirection() const;
    float getCutoff() const;
    float getExponent() const;

private:
    glm::vec3 m_direction;
    float m_cutoff;
    float m_exponent;
};
