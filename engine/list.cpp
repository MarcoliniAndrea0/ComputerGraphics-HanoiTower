#include "list.h"
#include "engine.h"
#include <algorithm> 
#include <iostream>

List::List()
{
	renderList.reserve(100);
}

List::~List()
{
	clear;
}

void List::add(Node* node, const glm::mat4& matrix) {
	if (!node) return;
	
	renderList.push_back({ node, matrix });
}

void List::render()
{
	// 1. ORDINAMENTO (Sorting)
	std::sort(renderList.begin(), renderList.end(), [](const RenderNode& a, const RenderNode& b)
		{
			// Cerca "Light" nel tipo. Se lo trova, find restituisce una posizione valida (!= npos)
			bool aIsLight = (a.node->getType().find("Light") != std::string::npos);
			bool bIsLight = (b.node->getType().find("Light") != std::string::npos);
			// Regola: se A è luce e B no, A viene prima (true).
			if (aIsLight && !bIsLight) return true;

			// Se B è luce e A no, B viene prima (quindi A non viene prima -> false).
			if (!aIsLight && bIsLight) return false;

			// Se sono entrambi luci o entrambi mesh, l'ordine non importa (return false)
			return false;
		});

	// 2. RENDERING
	for (const auto& element : renderList) {
		glPushMatrix();
		glMultMatrixf(glm::value_ptr(element.trans));

		element.node->render();

		glPopMatrix();
	}
}

void List::clear()
{
	renderList.clear();
}
