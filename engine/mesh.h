#pragma once
#include "node.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct singleVertex {
	glm::vec3 vertex;
	glm::vec3 normalVector;
};

class Mesh : public Node {

public:

	void addVertices(const glm::vec3& vec, const glm::vec3& nVec);

	singleVertex getVertices(int i);

	void removeVertices(int i);

	virtual void render(const glm::mat4& mat) override;

private:	
	std::vector<singleVertex> vertices;

};