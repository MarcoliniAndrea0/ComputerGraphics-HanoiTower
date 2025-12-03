/**
 * @file main.cpp
 * @brief Visualizzatore Stanza (Scena 2)
 */
#define _CRT_SECURE_NO_WARNINGS
#include "definitions.h"
#include "engine.h"
#include "Camera.h"
#include "ProspectiveCamera.h"
#include "Mesh.h"
#include "Material.h"
#include "Light.h"
#include "PointLight.h" 
#include "List.h"
#include "OvoParser.h"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 // Globals
List renderList;
Node* root = nullptr;
PerspectiveCamera* activeCamera = nullptr;
PointLight* roomLight = nullptr;

// Trova la camera nel grafo (ricorsiva)
Node* findFirstCamera(Node* node) {
    if (!node) return nullptr;
    if (node->getName().find("Camera") != std::string::npos) return node;

    // Iterazione manuale sui figli
    int i = 0;
    while (Node* child = node->getChild(i++)) {
        Node* res = findFirstCamera(child);
        if (res) return res;
    }
    return nullptr;
}

void displayCallback() {
    // 1. SFONDO BLU (Così capiamo se il rendering funziona)
    std::cout << "Render Frame..." << std::endl; // DEBUG: Controlla se lo stampa a ciclo continuo

    glClearColor(0.2f, 0.3f, 0.5f, 1.0f); // Blu
    Eng::Base::getInstance().clearWindow();

    // 2. STATO OPENGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); // Luci accese per vedere le pareti 3D
    glEnable(GL_LIGHT0);   // Luce fissa camera
    glEnable(GL_NORMALIZE); // Importante per le normali scalate

    renderList.clear();

    // 3. AGGIUNTA CAMERA
    if (activeCamera) renderList.add(activeCamera, glm::mat4(1.0f));

    // 4. LUCE STANZA (La aggiungiamo manuale per sicurezza)
    if (roomLight) renderList.add(roomLight, glm::mat4(1.0f));

    // 5. SCENA
    if (root) renderList.add(root, glm::mat4(1.0f));

    renderList.render();
    Eng::Base::getInstance().swapBuffer();
}

void reshapeCallback(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    if (activeCamera) activeCamera->setAspectRatio((float)w / h);
}

void keyboardCallback(unsigned char key, int mouseX, int mouseY) {
    if (key == 27) { // ESC
        if (root) delete root;
        if (activeCamera) delete activeCamera;
        if (roomLight) delete roomLight;
        Eng::Base::getInstance().free();
        exit(0);
    }

    // Movimento camera WASD
    if (activeCamera) {
        float speed = 10.0f; // Veloce perché la stanza potrebbe essere grande
        glm::mat4 mat = activeCamera->getMatrix();

        if (key == 'w') mat = glm::translate(mat, glm::vec3(0, 0, -speed));
        if (key == 's') mat = glm::translate(mat, glm::vec3(0, 0, speed));
        if (key == 'a') mat = glm::translate(mat, glm::vec3(-speed, 0, 0));
        if (key == 'd') mat = glm::translate(mat, glm::vec3(speed, 0, 0));
        if (key == 'q') mat = glm::translate(mat, glm::vec3(0, speed, 0));
        if (key == 'e') mat = glm::translate(mat, glm::vec3(0, -speed, 0));

        activeCamera->setMatrix(mat);
    }
}

int main(int argc, char* argv[]) {
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init("Room Viewer", 1024, 768, argc, argv);

    eng.setDisplayCallback(displayCallback);
    eng.setReshapeCallback(reshapeCallback);
    eng.setKeyboardCallback(keyboardCallback);

    // --- CARICAMENTO ---
    OvoParser parser;
    std::cout << "[MAIN] Caricamento scena2.ovo (Stanza)..." << std::endl;
    root = parser.loadFile("scena2.ovo");

    if (!root) root = new Node("Dummy");

    // --- CAMERA ---
    activeCamera = new PerspectiveCamera("MainCam", 60.0f, 1.33f, 0.1f, 10000.0f);
    Node* sceneCam = findFirstCamera(root);

    if (sceneCam) {
        std::cout << "[MAIN] Camera trovata nel file: " << sceneCam->getName() << std::endl;
        activeCamera->setMatrix(sceneCam->getMatrix());
    }
    else {
        std::cout << "[MAIN] Camera non trovata. Uso default panoramico." << std::endl;
        // Posizione "Dall'alto" per vedere il pavimento
        glm::mat4 pos = glm::translate(glm::mat4(1.0f), glm::vec3(0, 200, 200));
        pos = glm::rotate(pos, glm::radians(-45.0f), glm::vec3(1, 0, 0));
        activeCamera->setMatrix(pos);
    }

    // --- LUCE EXTRA ---
    // Aggiungiamo una luce al centro della scena per illuminare i muri
    roomLight = new PointLight("LightBulb");
    roomLight->setPosition(glm::vec3(0, 100, 0)); // In alto al centro
    roomLight->setDiffuse(glm::vec3(1.0f, 1.0f, 0.8f)); // Luce calda
    roomLight->setLinearAttenuation(0.005f); // Attenuazione lenta
    roomLight->setMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0, 100, 0)));

    eng.run();
    return 0;
}