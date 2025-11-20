#include "mesh.h"

void Mesh::addVertices(const glm::vec3& vec)
{
	Mesh::vertices.push_back(vec);
}

glm::vec3 Mesh::getVertices(int i)
{
	if (i >= 0 && i < Mesh::vertices.size()) {
		return Mesh::vertices.at(i);
	}
	return glm::vec3(0);
}

void Mesh::removeVertices(int i)
{
	if (i >= 0 && i < vertices.size()) {
		vertices.erase(vertices.begin() + i);
	}
}

void Mesh::render()
{

}
