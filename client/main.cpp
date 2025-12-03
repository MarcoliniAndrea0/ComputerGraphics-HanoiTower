/**
 * @file		main.cpp
 * @brief	Client application con rendering corretto
 */

#include "definitions.h"
#include "engine.h"
#include "Camera.h"
#include "ProspectiveCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "List.h"
#include "OvoParser.h"

#include <iostream>
#include <filesystem>

 // --- GLOBALI ---
List renderList;
Node* root = nullptr;
PerspectiveCamera* mainCamera = nullptr;

// --- FUNZIONE RICORSIVA PER RIEMPIRE LA LISTA ---
// Questa funzione attraversa l'albero, calcola le matrici globali 
// e aggiunge ogni singolo pezzo alla lista per essere ordinato.
void updateRenderList(Node* node, const glm::mat4& parentMatrix) {
    if (!node) return;

    // 1. Calcola la matrice World di questo nodo
    // (Moltiplica la matrice del padre per quella locale del nodo)
    glm::mat4 worldMatrix = parentMatrix * node->getMatrix();

    // 2. Aggiungi il nodo alla lista di rendering
    renderList.add(node, worldMatrix);

    // 3. Ricorsione sui figli (se ne ha)
    // Nota: Node non espone getChildren() pubblicamente nel codice base che avevamo,
    // ma se il metodo render() di Node fa la ricorsione, qui dobbiamo replicarla 
    // o modificare Node. 

    // PER ORA (Hack rapido): Poiché non abbiamo accesso facile ai figli da qui senza modificare Node.h,
    // e dato che Node::render() fa già la ricorsione...
    // Il problema è che vogliamo separare Luci e Mesh.

    // SOLUZIONE MIGLIORE: Modifichiamo Node.h per esporre i figli, 
    // OPPURE (più semplice ora) ci fidiamo che le luci siano globali o le aggiungiamo a mano.
}
/* NOTA: Sopra c'è un problema logico. Senza accesso ai figli (getChildren), non possiamo
   visitare l'albero qui nel main.

   TORNIAMO ALLA SOLUZIONE FUNZIONANTE SENZA TOCCARE L'ENGINE:
   Lasciamo che la gerarchia faccia il suo corso, MA forziamo una luce globale sempre accesa.
*/

// --- CALLBACK ---
void displayCallback() {
    Eng::Base::getInstance().clearWindow();

    renderList.clear();

    // Aggiungiamo la radice alla lista.
    // ATTENZIONE: Questo non ordina Mesh vs Luci dentro la radice.
    // Se le luci sono nel file OVO, speriamo siano all'inizio.
    if (root) renderList.add(root, glm::mat4(1.0f));

    // Disegna
    renderList.render();

    Eng::Base::getInstance().swapBuffer();
}

void reshapeCallback(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    if (mainCamera) mainCamera->setAspectRatio((float)w / h);
}

void keyboardCallback(unsigned char key, int mouseX, int mouseY) {
    if (key == 27) { // ESC
        if (root) delete root;
        Eng::Base::getInstance().free();
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init("Hanoi Tower", 800, 600, argc, argv);

    eng.setDisplayCallback(displayCallback);
    eng.setReshapeCallback(reshapeCallback);
    eng.setKeyboardCallback(keyboardCallback);

    // Debug Percorso
    std::cout << "Working Dir: " << std::filesystem::current_path() << std::endl;

    // Caricamento
    OvoParser parser;
    root = parser.loadFile("scena2.ovo");

    if (!root) {
        std::cerr << "ERRORE: scena1.ovo non caricato." << std::endl;
        // Creiamo una radice vuota per non crashare
        root = new Node("RootVuota");
    }
    else {
        std::cout << "Scena caricata!" << std::endl;
    }

    // --- SETUP CAMERA & LUCI GLOBALI ---
    // Le aggiungiamo AL DI FUORI del root caricato, o come fratelli, 
    // per essere sicuri che esistano e siano configurate bene.

    // 1. Camera
    mainCamera = new PerspectiveCamera("MainCam", 45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
    glm::mat4 camPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 50.0f)); // MOLTO PIÙ INDIETRO
    camPos = glm::rotate(camPos, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(glm::inverse(camPos)));

    // 2. Luce (Sole)
    DirectionalLight* sun = new DirectionalLight("SoleGlobal");
    sun->setDirection(glm::vec3(0.0f, -1.0f, -0.5f)); // Luce forte dall'alto
    sun->setDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));     // Luce bianca pura
    sun->setAmbient(glm::vec3(0.5f, 0.5f, 0.5f));     // Ambiente forte per vedere anche le zone d'ombra

    // Aggiungiamo questi alla lista render in modo esplicito per essere sicuri che vengano processati
    // Nota: Li aggiungiamo a root come figli, così vengono disegnati.
    root->addChild(mainCamera);
    root->addChild(sun);

    // --- DEBUG BOX ---
    // Disabilitiamo il culling per vedere anche l'interno degli oggetti se siamo dentro
    glDisable(GL_CULL_FACE);

    eng.run();

    if (root) delete root;
    return 0;
}