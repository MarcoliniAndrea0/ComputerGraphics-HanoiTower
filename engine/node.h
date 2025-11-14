/**
 * @file node.h
 * @brief Definizione della classe Node, nodo del grafo di scena con gestione della gerarchia e delle trasformazioni.
 *
 * Node eredita da Object e gestisce la relazione padre-figlio, la trasformazione locale e globale,
 * e fornisce i metodi per la ricerca ricorsiva e la renderizzazione dei nodi.
 */
#pragma one
#include "object.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>


/**
 * @class Node
 * @brief Rappresenta un nodo del grafo di scena 3D.
 *
 * Permette la gestione della gerarchia tra nodi, le trasformazioni con matrici locali e globali,
 * e la ricerca ricorsiva per nome.
 */
class Node : public Object
{
public:
    /**
     * @brief Costruttore della classe Node.
     * @param name Nome del nodo (opzionale).
     * @param type Tipo del nodo (di default "Node").
     */
    Node(const std::string& name = "", const std::string& type = "Node");
    
    /**
     * @brief Distruttore virtuale.
     */
    virtual ~Node() = default;

    /**
     * @brief Esegue la renderizzazione ricorsiva di questo nodo e di tutti i figli.
     */
    virtual void render() override;

    /**
     * @brief Restituisce il puntatore al nodo genitore.
     * @return Puntatore al parent oppure nullptr se è un nodo radice.
     */
    Node* getParent() const;
    
    /**
     * @brief Aggiunge un nodo figlio.
     * @param n Puntatore al nodo figlio da aggiungere.
     */
    void addChild(Node *n);
    
    /**
     * @brief Restituisce il figlio all'indice specificato.
     * @param a Indice del figlio.
     * @return Puntatore al nodo figlio o nullptr se l'indice è errato.
     */
    Node* getChild(int a) const;
    
    /**
    * @brief Rimuove il figlio all'indice specificato.
    * @param a Indice del figlio da rimuovere.
    */
    void removeChild(int a);
    
    /**
     * @brief Restituisce la matrice di trasformazione locale.
     * @return Puntatore alla matrice locale.
     */
    glm::mat4* getLocalMatrix();
    
    /**
     * @brief Restituisce la matrice di trasformazione globale.
     * @return Puntatore alla matrice globale.
     */
    glm::mat4* getWorldMatrix();
    
    /**
     * @brief Ricerca ricorsivamente un nodo per nome in tutta la sotto-gerarchia.
     * @param s Nome da cercare.
     * @return Puntatore al nodo trovato, oppure nullptr se non esiste.
     */
    Node* findByName(const std::string s);

private:
    glm::mat4 localMatrix;              // Matrice di trasformazione locale
    glm::mat4 worldMatrix;              // Matrice di trasformazione globale
    std::vector<Node*> child_nodes;     // Vettore di figli
    Node* parent;                       // Puntatore al nodo genitore
};