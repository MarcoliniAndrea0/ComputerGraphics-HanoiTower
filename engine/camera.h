#pragma once

#include "node.h"
#include "engine.h"
#include <glm/glm.hpp>

class ENG_API Camera : public Node
{
public:
    Camera(const std::string& name);

    virtual ~Camera() = default;

    virtual glm::mat4 getProjectionMatrix() const = 0;
};