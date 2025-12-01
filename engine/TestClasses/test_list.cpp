/**
 * @file test_list.cpp
 * @brief Test unitario per la classe List.
 */
#include <iostream>
#include <string>
#include "../list.h"
#include "../node.h"
#include "../engine.h" 


 // --- Classi Mock (Finti oggetti per il test) ---
class MockMesh : public Node {
public:
    MockMesh(const std::string& n) : Node(n, "Mesh") {}
    void render() override {
        std::cout << "  [RENDER] Disegno Mesh: " << this->getName() << std::endl;
    }
};

class MockLight : public Node {
public:
    MockLight(const std::string& n) : Node(n, "PointLight") {}
    void render() override {
        std::cout << "  [RENDER] Attivo Luce: " << this->getName() << std::endl;
    }
};

// Funzione con la logica del test
void runListTest() {
    std::cout << "=== INIZIO TEST LISTA (Sorting) ===" << std::endl;
    List renderList;
    glm::mat4 identity(1.0f);

    // Creiamo oggetti misti
    MockMesh* m1 = new MockMesh("Tavolo (Mesh)");
    MockLight* l1 = new MockLight("Sole (Light)");
    MockMesh* m2 = new MockMesh("Sedia (Mesh)");
    MockLight* l2 = new MockLight("Lampadina (Light)");

    // Aggiungiamo alla lista in ordine disordinato
    renderList.add(m1, identity);
    renderList.add(l1, identity);
    renderList.add(m2, identity);
    renderList.add(l2, identity);

    std::cout << "Eseguo render()..." << std::endl;

    // Il metodo render() della lista dovrebbe ordinare e chiamare i render() dei nodi
    renderList.render();

    std::cout << "=== FINE TEST LISTA ===" << std::endl;

    // Pulizia
    delete m1; delete l1; delete m2; delete l2;
}

// --- MAIN (FONDAMENTALE!) ---
// Questo è il punto di ingresso che mancava e causava l'errore WinMain
int main(int argc, char* argv[]) {
    runListTest();
    return 0;
}