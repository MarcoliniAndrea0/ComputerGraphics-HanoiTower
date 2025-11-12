#include "object.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class Node : public Object
{
public:
    Node();
    virtual ~Node() = default;

    virtual void render() override;
    Node *getParent();
    void addChild(Node *n);
    Node *getChild(int a);
    void removeChild(int a);
    glm::mat4 *getLocalMatrix();
    glm::mat4 *getWorldMatrix();
    Node *findByName(std::string s);

private:
    glm::mat4 localMatrix;
    glm::mat4 worldMatrix;
    std::vector<Node *> child_nodes;
    Node *parent;
};