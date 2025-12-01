#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "node.h"

#ifndef LIST_H
#define LIST_H



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
	std::vector<RenderNode> m_renderList;
};

#endif // !LIST_H
