#pragma one
#include "object.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Node : public Object
{
public:
    Node(const std::string& name = "", const std::string& type = "Node");
    virtual ~Node() = default;

    virtual void render() override;

    Node* getParent() const;
    void addChild(Node *n);
    Node* getChild(int a) const;
    void removeChild(int a);
    glm::mat4* getLocalMatrix();
    glm::mat4* getWorldMatrix();
    Node* findByName(const std::string s);

private:
    glm::mat4 localMatrix;
    glm::mat4 worldMatrix;
    std::vector<Node*> child_nodes;
    Node* parent;
};