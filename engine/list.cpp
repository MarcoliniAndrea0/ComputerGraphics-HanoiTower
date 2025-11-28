#include "list.h"
#include "object.h"
#include "engine.h"
#include "light.h"
#include <iostream>
#include <gl/GL.h>
#include <glm/gtc/type_ptr.hpp>

List::List()
{
	m_objects.reserve(100);
	m_lights.reserve(100);
}

List::~List()
{
	clear;
}

void List::add(Node* node, const glm::mat4& matrix)
{
	RenderNode element;
	element.node = node;
	element.trans = matrix;

	//TODO LIGHT params

	m_objects.push_back(element);
}

void List::render()
{
	//Renderizza luci per primo
	for (const auto& element : m_lights)
	{
		glPushMatrix();
		glMultMatrixf(glm::value_ptr(element.trans));

		element.node->render();

		glPopMatrix();
	}

	//Renderizza Mesh
	for (const auto& element : m_objects)
	{
		glPushMatrix();
		glMultMatrixf(glm::value_ptr(element.trans));

		element.node->render();

		glPopMatrix();
	}
}

void List::clear()
{
	m_lights.clear();
	m_objects.clear();
}
