#include "mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/freeglut.h>

void Mesh::addVertices(const glm::vec3& vec, const glm::vec3& nVec)
{

	Mesh::vertices.push_back({vec, nVec});
}

singleVertex Mesh::getVertices(int i)
{
	if (i >= 0 && i < Mesh::vertices.size()) {
		return Mesh::vertices.at(i);
	}
	return {glm::vec3(0),glm::vec3(0)};
}

void Mesh::removeVertices(int i)
{
	if (i >= 0 && i < vertices.size()) {
		vertices.erase(vertices.begin() + i);
	}
}

void Mesh::render(const glm::mat4& mat)
{
	//int vertexCount = 0;
	// Set model matrix as current OpenGL matrix:
	glLoadMatrixf(glm::value_ptr(mat));

	for (const singleVertex& v : vertices) {
		//glNormal3f( (glm::vec3) v.normalVector);
		
	}

}
