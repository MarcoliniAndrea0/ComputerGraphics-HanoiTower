#pragma once

#include "light.h"

class PointLight : public Light
{
public:
    PointLight(const std::string& name, const std::string& type = "PointLight");
    virtual ~PointLight() = default;

    void render() override;

    // Setters
    void setPosition(const glm::vec3& pos);
    void setConstantAttenuation(float v);
    void setLinearAttenuation(float v);
    void setQuadraticAttenuation(float v);

    // Getters
    glm::vec3 getPosition() const;
    float getConstantAttenuation() const;
    float getLinearAttenuation() const;
    float getQuadraticAttenuation() const;

protected:
    glm::vec3 m_position;
    float m_constAtt;
    float m_linAtt;
    float m_quadAtt;
};