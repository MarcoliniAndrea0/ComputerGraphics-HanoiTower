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
	// Set model matrix as current OpenGL matrix:
	glLoadMatrixf(glm::value_ptr(mat));

	for (int x = 0; x < vertices.size();x+=3) {
	
		glBegin(GL_TRIANGLES);

		glNormal3fv(glm::value_ptr(vertices.at(x).normalVector));
		
		glVertex3fv(glm::value_ptr(vertices.at(x).vertex));
		glVertex3fv(glm::value_ptr(vertices.at(x+1).vertex));
		glVertex3fv(glm::value_ptr(vertices.at(x+2).vertex));

		glEnd();
		
	}

}
