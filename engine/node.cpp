#include "node.h"
#include <glm/gtc/matrix_transform.hpp>

Node::Node(const std::string& name, const std::string& type)
    : Object(name, type),
    localMatrix(glm::mat4(1.0f)),
    worldMatrix(glm::mat4(1.0f)),
    parent(nullptr)
{
}

void Node::render()
{
    for (Node* child : child_nodes)
    {
        if (child) child->render();
    }
}

Node* Node::getParent() const
{
    return parent;
}

void Node::addChild(Node *n)
{
    if (!n) return;
    child_nodes.push_back(n);
    n->parent = this;
}

Node *Node::getChild(int a) const
{
    if (a < 0 || a >= static_cast<int>(child_nodes.size())) return nullptr;
    return child_nodes[a];
}

void Node::removeChild(int a)
{
    if (a < 0 || a >= static_cast<int>(child_nodes.size())) return;
    child_nodes[a]->parent = nullptr;
    child_nodes.erase(child_nodes.begin() + a);
}

glm::mat4 *Node::getLocalMatrix()
{
    return localMatrix;
}

glm::mat4 *Node::getWorldMatrix()
{
    return worldMatrix;
}

Node *Node::findByName(std::string s)
{
    if (name == s) return this;
    for (Node* child : child_nodes)
    {
        Node* result = child->findByName(s);
        if (result) return result;
    }
    return nullptr;
}