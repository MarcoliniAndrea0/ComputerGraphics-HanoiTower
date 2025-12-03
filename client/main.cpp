/**
 * @file		main.cpp
 * @brief	Applicazione Client con caricamento Scena OVO
 */

#include "definitions.h"
#include "engine.h"
#include "camera.h"
#include "prospectiveCamera.h"
#include "mesh.h"
#include "material.h"
 // #include "Texture.h" // Texture disabilitate per ora
#include "light.h"
#include "directionalLight.h"
#include "list.h"
#include "ovoParser.h" // <--- Fondamentale per leggere il file

#include <iostream>
#include <filesystem>

// --- VARIABILI GLOBALI ---
List renderList;      // La lista degli oggetti da disegnare
Node* root = nullptr; // La radice della scena caricata

// --- CALLBACK DI DISEGNO (Viene chiamata ogni frame) ---
void displayCallback() {
    // 1. Pulisce lo schermo (Colore e Profondità)
    Eng::Base::getInstance().clearWindow();

    // 2. Prepara la lista di rendering
    renderList.clear();

    // Se abbiamo caricato una scena, la aggiungiamo alla lista
    if (root) {
        // Aggiungiamo la radice con matrice identità.
        // Il metodo render() dei nodi gestirà la gerarchia (figli).
        renderList.add(root, glm::mat4(1.0f));
    }

    // 3. Esegue il disegno effettivo (Luci -> Mesh)
    renderList.render();

    // 4. Scambia i buffer (Double Buffering) per mostrare l'immagine
    Eng::Base::getInstance().swapBuffer();
}

// --- CALLBACK RIDIMENSIONAMENTO ---
void reshapeCallback(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    // Qui in futuro aggiorneremo l'aspect ratio della camera
}

// --- CALLBACK TASTIERA ---
void keyboardCallback(unsigned char key, int mouseX, int mouseY) {
    switch (key) {
    case 27: // Tasto ESC
        if (root) delete root;
        Eng::Base::getInstance().free();
        exit(0); // Chiude il programma
        break;
    }
}

void specialCallback(int key, int mouseX, int mouseY) {
    switch (key) {
    case GLUT_KEY_UP:
        std::cout << "Freccia SU" << std::endl;
        // Qui potrai muovere la camera in avanti
        break;
    case GLUT_KEY_DOWN:
        std::cout << "Freccia GIU" << std::endl;
        break;
    case GLUT_KEY_LEFT:
        std::cout << "Freccia SINISTRA" << std::endl;
        break;
    case GLUT_KEY_RIGHT:
        std::cout << "Freccia DESTRA" << std::endl;
        break;
    }
}

// --- MAIN ---
int main(int argc, char* argv[]) 
{
    // Inizializza l'Engine
    Eng::Base& eng = Eng::Base::getInstance();
    eng.init("Hanoi Tower - Ovo Scene", 800, 600, argc, argv);

    // Registra le funzioni di callback
    eng.setDisplayCallback(displayCallback);
    eng.setReshapeCallback(reshapeCallback);
    eng.setKeyboardCallback(keyboardCallback);
    eng.setSpecialCallback(specialCallback);

    // --- DEBUG PERCORSO ---
    // Questo ti dice esattamente da dove sta girando il programma!
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "CARTELLA DI LAVORO ATTUALE: " << std::filesystem::current_path() << std::endl;
    std::cout << "Il programma cerca il file qui: " << std::filesystem::current_path() / "scena01.ovo" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    // ----------------------

    // --- CARICAMENTO SCENA ---
    std::cout << "Tentativo di caricamento 'scena01.ovo'..." << std::endl;
    OvoParser parser;

    // Carica il file (Assicurati che scena1.ovo sia nella cartella dell'EXE!)
    root = parser.loadFile("scena01.ovo");

    if (root) {
        std::cout << "Scena caricata con successo!" << std::endl;

        // --- AGGIUNTA DI CORTESIA (Camera e Luce) ---
        // Se la scena OVO non ha camera/luci, ne mettiamo noi per vedere qualcosa

        // 1. Camera (Posizione tattica per vedere il tavolo)
        PerspectiveCamera* cam = new PerspectiveCamera("MainCam", 45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
        glm::mat4 camPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 20.0f));
        camPos = glm::rotate(camPos, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // Imposta la matrice di vista in OpenGL
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(glm::value_ptr(glm::inverse(camPos)));
        root->addChild(cam);

        // 2. Luce Direzionale (Sole)
        DirectionalLight* sun = new DirectionalLight("Sole");
        sun->setDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
        root->addChild(sun);

    }
    else {
        std::cerr << "ERRORE CRITICO: 'scena1.ovo' non trovato o non valido!" << std::endl;
        std::cout << "Premi INVIO per chiudere..." << std::endl;
        std::cin.get(); // Blocca la console per farti leggere l'errore
    }

    std::cout << "Avvio Loop di Rendering..." << std::endl;

    // --- PUNTO CHIAVE: Loop Infinito ---
    eng.run();

    // Pulizia (raggiunta solo alla chiusura)
    if (root) delete root;
    return 0;
}