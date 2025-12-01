#pragma once

#include "node.h"
#include <glm/glm.hpp>

class Camera : public Node
{
public:
    Camera(const std::string& name);

    virtual ~Camera() = default;

    virtual glm::mat4 getProjectionMatrix() const = 0;
};