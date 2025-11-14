/**
 * @file node.cpp
 * @brief Implementazione della classe Node del grafo di scena.
 *
 * Fornisce la logica per la gestione della gerarchia tra nodi,
 * la ricerca ricorsiva per nome, e le trasformazioni tramite matrici.
 */
#include "node.h"
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Costruttore della classe Node. Inizializza le matrici a identità e il parent a nullptr.
 * @param name Nome del nodo.
 * @param type Tipo del nodo.
 */
Node::Node(const std::string& name, const std::string& type)
    : Object(name, type),
    localMatrix(glm::mat4(1.0f)),
    worldMatrix(glm::mat4(1.0f)),
    parent(nullptr)
{
}

/**
 * @brief Render ricorsivo: richiama render su tutti i figli del nodo corrente.
 */
void Node::render()
{
    for (Node* child : child_nodes)
    {
        if (child) child->render();
    }
}

/**
 * @brief Restituisce il parent del nodo.
 * @return Puntatore al nodo genitore.
 */
Node* Node::getParent() const
{
    return parent;
}

/**
 * @brief Aggiunge un figlio e imposta il parent del figlio.
 * @param n Puntatore al nodo figlio da aggiungere.
 */
void Node::addChild(Node *n)
{
    if (!n) return;
    child_nodes.push_back(n);
    n->parent = this;
}

/**
 * @brief Restituisce il figlio all'indice indicato.
 * @param a Indice del figlio.
 * @return Puntatore al figlio oppure nullptr se indice errato.
 */
Node *Node::getChild(int a) const
{
    if (a < 0 || a >= static_cast<int>(child_nodes.size())) return nullptr;
    return child_nodes[a];
}

/**
 * @brief Rimuove il figlio all'indice indicato e cancella il parent del figlio.
 * @param a Indice del figlio da rimuovere.
 */
void Node::removeChild(int a)
{
    if (a < 0 || a >= static_cast<int>(child_nodes.size())) return;
    child_nodes[a]->parent = nullptr;
    child_nodes.erase(child_nodes.begin() + a);
}

/**
 * @brief Restituisce un puntatore alla matrice locale.
 * @return Puntatore a glm::mat4 locale.
 */
glm::mat4 *Node::getLocalMatrix()
{
    return localMatrix;
}

/**
 * @brief Restituisce un puntatore alla matrice globale.
 * @return Puntatore a glm::mat4 globale.
 */
glm::mat4 *Node::getWorldMatrix()
{
    return worldMatrix;
}

/**
 * @brief Ricerca ricorsivamente il nodo con il nome fornito tra il nodo e i suoi figli.
 * @param s Nome da cercare.
 * @return Puntatore al nodo trovato oppure nullptr se assente.
 */
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