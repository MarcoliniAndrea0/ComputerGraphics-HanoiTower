#pragma once

#include <memory>
#include "Object.h"
#include "Common.h"

/**
 * @class Node
 * @brief Rappresenta un nodo nella gerarchia di una scena 3D.
 *
 * La classe `Node` è utilizzata per rappresentare un oggetto nella scena, con supporto per trasformazioni,
 * gerarchie di nodi figli e rendering.
 */
class LIB_API Node : public Object, public std::enable_shared_from_this<Node>
{
public:
    Node();
    Node(std::string type);
    virtual ~Node() = default;


    // Getter
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;
    int getPriority() const;
    glm::mat4 getLocalMatrix() const;
    glm::mat4 getTransform() const; // Alias di getLocalMatrix
    glm::mat4 getWorldMatrix(const glm::mat4& parentMatrix = glm::mat4(1.0f)) const;
    std::shared_ptr<Node> getParent() const;
    std::vector<std::shared_ptr<Node>> getChildren() const;
    std::vector<std::shared_ptr<Node>>& getChildren();

    // Setter
    void setPosition(const glm::vec3 newPosition);
    void setRotation(const glm::vec3 newPosition);
    void setScale(const glm::vec3 newPosition);
    void setBaseMatrix(const glm::mat4 newBaseMatrix);
    void setPriority(int p);

    void addChild(const std::shared_ptr<Node> newChild);
    void removeAllChildren();
    void render(const glm::mat4 viewMatrix) const override;

private:
    int priority; ///< Priorità del nodo.
    std::vector<std::shared_ptr<Node>> children; ///< Vettore di nodi figli.
    glm::mat4 _baseMatrix; ///< Matrice di trasformazione di base.
    glm::vec3 _position;   ///< Posizione relativa del nodo.
    glm::vec3 _rotation;   ///< Rotazione relativa del nodo.
    glm::vec3 _scale;      ///< Scala relativa del nodo.
    std::weak_ptr<Node> parent; ///< Nodo genitore.
};
