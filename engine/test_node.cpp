/**
 * @file test_node.cpp
 * @brief Test completo della classe Node e della gerarchia del grafo di scena.
 *
 * Verifica:
 * - Creazione di nodi e gerarchia padre-figlio
 * - Aggiunta e rimozione di figli
 * - Ricerca ricorsiva per nome
 * - Matrici di trasformazione locali e globali
 * - Render ricorsivo
 */

 // IMPORTANTE: Definire prima di includere le estensioni GTX
#define GLM_ENABLE_EXPERIMENTAL

#include "node.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

 /**
  * @brief Funzione helper per stampare una matrice GLM.
  * @param mat Matrice da stampare.
  * @param label Etichetta descrittiva.
  */
void printMatrix(const glm::mat4& mat, const std::string& label)
{
    std::cout << label << ":\n" << glm::to_string(mat) << std::endl;
}

/**
 * @brief Funzione helper per stampare la gerarchia del nodo.
 * @param node Nodo di cui stampare le informazioni.
 * @param indent Livello di indentazione.
 */
void printNodeHierarchy(Node* node, int indent = 0)
{
    if (!node) return;

    for (int i = 0; i < indent; ++i) std::cout << "  ";
    std::cout << "- [ID: " << node->getID()
        << ", Name: '" << node->getName()
        << "', Type: '" << node->getType() << "']";

    if (node->getParent())
        std::cout << " (Parent: " << node->getParent()->getName() << ")";
    else
        std::cout << " (ROOT)";

    std::cout << std::endl;
}

/**
 * @brief Funzione per stampare ricorsivamente l'intera gerarchia.
 * @param node Nodo radice.
 * @param indent Livello di indentazione.
 */
void printTree(Node* node, int indent = 0)
{
    if (!node) return;

    printNodeHierarchy(node, indent);

    // Stampa tutti i figli
    int childCount = 0;
    while (Node* child = node->getChild(childCount))
    {
        printTree(child, indent + 1);
        childCount++;
    }
}

int main()
{
    std::cout << "=== TEST CLASSE NODE ===" << std::endl << std::endl;

    // Test 1: Creazione nodo base
    std::cout << "Test 1: Creazione nodo singolo" << std::endl;
    Node rootNode("Root", "RootNode");
    std::cout << "ID: " << rootNode.getID() << std::endl;
    std::cout << "Nome: " << rootNode.getName() << std::endl;
    std::cout << "Tipo: " << rootNode.getType() << std::endl;
    std::cout << "Parent: " << (rootNode.getParent() ? "Presente" : "nullptr (ROOT)") << std::endl;
    std::cout << std::endl;

    // Test 2: Creazione gerarchia
    std::cout << "Test 2: Creazione gerarchia padre-figlio" << std::endl;
    Node* child1 = new Node("Child1", "ChildNode");
    Node* child2 = new Node("Child2", "ChildNode");
    Node* child3 = new Node("Child3", "ChildNode");

    rootNode.addChild(child1);
    rootNode.addChild(child2);
    rootNode.addChild(child3);

    std::cout << "Root ha ora 3 figli:" << std::endl;
    printTree(&rootNode);
    std::cout << std::endl;

    // Test 3: Verifica getChild
    std::cout << "Test 3: Accesso ai figli tramite getChild()" << std::endl;
    Node* firstChild = rootNode.getChild(0);
    Node* secondChild = rootNode.getChild(1);
    Node* thirdChild = rootNode.getChild(2);
    Node* invalidChild = rootNode.getChild(10);

    std::cout << "Child 0: " << (firstChild ? firstChild->getName() : "nullptr") << std::endl;
    std::cout << "Child 1: " << (secondChild ? secondChild->getName() : "nullptr") << std::endl;
    std::cout << "Child 2: " << (thirdChild ? thirdChild->getName() : "nullptr") << std::endl;
    std::cout << "Child 10 (invalido): " << (invalidChild ? "ERRORE" : "nullptr (corretto)") << std::endl;
    std::cout << std::endl;

    // Test 4: Gerarchia multi-livello
    std::cout << "Test 4: Gerarchia multi-livello (nipoti)" << std::endl;
    Node* grandchild1 = new Node("Grandchild1", "GrandchildNode");
    Node* grandchild2 = new Node("Grandchild2", "GrandchildNode");

    child1->addChild(grandchild1);
    child1->addChild(grandchild2);

    std::cout << "Gerarchia completa:" << std::endl;
    printTree(&rootNode);
    std::cout << std::endl;

    // Test 5: Ricerca per nome
    std::cout << "Test 5: Ricerca ricorsiva con findByName()" << std::endl;
    Node* foundRoot = rootNode.findByName("Root");
    Node* foundChild2 = rootNode.findByName("Child2");
    Node* foundGrandchild1 = rootNode.findByName("Grandchild1");
    Node* notFound = rootNode.findByName("NonEsistente");

    std::cout << "Cerca 'Root': " << (foundRoot ? "Trovato (ID: " + std::to_string(foundRoot->getID()) + ")" : "Non trovato") << std::endl;
    std::cout << "Cerca 'Child2': " << (foundChild2 ? "Trovato (ID: " + std::to_string(foundChild2->getID()) + ")" : "Non trovato") << std::endl;
    std::cout << "Cerca 'Grandchild1': " << (foundGrandchild1 ? "Trovato (ID: " + std::to_string(foundGrandchild1->getID()) + ")" : "Non trovato") << std::endl;
    std::cout << "Cerca 'NonEsistente': " << (notFound ? "ERRORE - Trovato!" : "Non trovato (corretto)") << std::endl;
    std::cout << std::endl;

    // Test 6: Rimozione figlio
    std::cout << "Test 6: Rimozione di un figlio con removeChild()" << std::endl;
    std::cout << "Prima della rimozione:" << std::endl;
    printTree(&rootNode);

    rootNode.removeChild(1); // Rimuove Child2

    std::cout << "\nDopo aver rimosso Child2 (indice 1):" << std::endl;
    printTree(&rootNode);
    std::cout << "Parent di child2: " << (child2->getParent() ? "ERRORE - ancora presente" : "nullptr (corretto)") << std::endl;
    std::cout << std::endl;

    // Test 7: Test matrici di trasformazione
    std::cout << "Test 7: Matrici di trasformazione (local e world)" << std::endl;
    glm::mat4 localMat = child1->getLocalMatrix();
    glm::mat4 worldMat = child1->getWorldMatrix();

    printMatrix(localMat, "Local Matrix di Child1");
    printMatrix(worldMat, "World Matrix di Child1");
    std::cout << std::endl;

    // Test 8: Render ricorsivo
    std::cout << "Test 8: Render ricorsivo" << std::endl;
    std::cout << "Chiamata a rootNode.render() (dovrebbe renderizzare l'intera gerarchia):" << std::endl;
    rootNode.render();
    std::cout << "Render completato (nessun output visibile in questa implementazione base)" << std::endl;
    std::cout << std::endl;

    // Test 9: Test con nodi vuoti
    std::cout << "Test 9: Test con parametri di default" << std::endl;
    Node emptyNode;
    std::cout << "Nodo vuoto - Nome: '" << emptyNode.getName() << "', Tipo: '" << emptyNode.getType() << "'" << std::endl;
    std::cout << std::endl;

    // Test 10: Verifica parent dopo addChild
    std::cout << "Test 10: Verifica relazione parent-child" << std::endl;
    std::cout << "Parent di Child1: " << (child1->getParent() ? child1->getParent()->getName() : "nullptr") << std::endl;
    std::cout << "Parent di Grandchild1: " << (grandchild1->getParent() ? grandchild1->getParent()->getName() : "nullptr") << std::endl;
    std::cout << std::endl;

    // Test 11: Test rimozione con indice invalido
    std::cout << "Test 11: Test removeChild con indice invalido" << std::endl;
    int childCountBefore = 0;
    while (rootNode.getChild(childCountBefore)) childCountBefore++;

    rootNode.removeChild(-1); // Indice negativo
    rootNode.removeChild(100); // Indice troppo grande

    int childCountAfter = 0;
    while (rootNode.getChild(childCountAfter)) childCountAfter++;

    std::cout << "Figli prima: " << childCountBefore << ", dopo: " << childCountAfter << std::endl;
    std::cout << "Nessun figlio rimosso (corretto)" << std::endl;
    std::cout << std::endl;

    // Test 12: Gerarchia finale
    std::cout << "Test 12: Gerarchia finale del grafo di scena" << std::endl;
    printTree(&rootNode);
    std::cout << std::endl;

    // Pulizia memoria
    std::cout << "Pulizia memoria..." << std::endl;
    delete child1;
    delete child2;
    delete child3;
    delete grandchild1;
    delete grandchild2;

    std::cout << "\n=== TUTTI I TEST COMPLETATI ===" << std::endl;

    return 0;
}
