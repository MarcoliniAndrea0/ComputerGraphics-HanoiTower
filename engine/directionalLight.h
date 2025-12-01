#pragma once

#include "light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const std::string& name);
	~DirectionalLight() = default;

	void render() override;

	void setDirection(const glm::vec3& direction);
	glm::vec3 getDirection() const;

private:
	glm::vec3 m_direction;
};