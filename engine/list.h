#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "node.h"
#include "engine.h"

#ifndef LIST_H
#define LIST_H



struct RenderNode
{
	Node* node;
	glm::mat4 trans;
};

class ENG_API List
{
public:
	List();
	~List();

	void add(Node* node, const glm::mat4& matrix);

	void render();

	void clear();

private: 
	std::vector<RenderNode> renderList;
};

#endif // !LIST_H
