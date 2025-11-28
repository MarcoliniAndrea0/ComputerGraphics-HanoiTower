#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include "node.h"

#ifndef LIST_H
#define LIST_H

class Light;

struct RenderNode
{
	Node* node;
	glm::mat4 trans;
};

class List
{
public:
	List();
	~List();

	void add(Node* node, const glm::mat4& matrix);

	void render();

	void clear();

private: 
	std::vector<RenderNode> m_lights;
	std::vector<RenderNode> m_objects;
};

#endif // !LIST_H
