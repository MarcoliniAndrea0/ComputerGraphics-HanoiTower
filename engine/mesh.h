#pragma once
#include "node.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>


class Mesh : public Node {

public:

	void addVertices(const glm::vec3& vec);

	glm::vec3 getVertices(int i);

	void removeVertices(int i);

	virtual void render() override;


private:
	std::vector<glm::vec3> vertices;

};