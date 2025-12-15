#pragma once

#define LIB_NAME      "Engine"
#define LIB_VERSION   10

#include <sstream>

#include "OvoParser.h"
#include "PerspectiveCamera.h"
#include "camera.h"
#include "Common.h"
#include "material.h"
#include "list.h"
#include "mesh.h"
#include "node.h"

/**
 * @class Engine
 * @brief La classe principale (Singleton statico) del motore grafico.
 *
 * Questa classe gestisce l'intero ciclo di vita dell'applicazione grafica.
 * Essendo puramente statica, funge da punto di accesso globale per:
 * - L'inizializzazione del contesto OpenGL e GLUT.
 * - La gestione del grafo della scena (Scene Graph).
 * - La gestione della camera attiva e delle interazioni (Input).
 * - Il ciclo di rendering (Game Loop).
 *
 * @author [Gruppo6]
 * @version 1.0
 */
class LIB_API Engine
{
public:

    // ========================================================================
    // CICLO DI VITA E INIZIALIZZAZIONE
    // ========================================================================

    /**
     * @brief Inizializza il motore grafico e crea la finestra principale.
     *
     * Configura le librerie sottostanti (FreeGLUT, FreeImage), imposta le callback
     * di sistema e prepara il contesto OpenGL (Depth Test, Lighting, Culling).
     *
     * @warning Questo metodo deve essere chiamato prima di qualsiasi altra funzione dell'Engine.
     *
     * @param windowTitle Il titolo da assegnare alla finestra.
     * @param windowWidth La larghezza iniziale della finestra in pixel.
     * @param windowHeight L'altezza iniziale della finestra in pixel.
     */
    static void init(const std::string windowTitle, const int windowWidth, const int windowHeight);

    /**
     * @brief Avvia il ciclo principale del motore o verifica se � in esecuzione.
     * @return `true` se il motore � inizializzato e in esecuzione, altrimenti `false`.
     */
    static bool isRunning();

    /**
     * @brief Esegue il rendering di un singolo frame della scena.
     *
     * Questa funzione viene chiamata automaticamente dal ciclo GLUT, ma pu�
     * essere invocata manualmente per forzare un ridisegno.
     */
    static void render();

    /**
     * @brief Aggiorna la logica del motore (fisica, animazioni, input).
     *
     * Da chiamare all'interno della `idle` function o del timer loop.
     */
    static void update();

    /**
     * @brief Interrompe il ciclo di rendering.
     *
     * Ferma il loop ma non distrugge il contesto o la finestra.
     */
    static void stop();

    /**
     * @brief Termina l'esecuzione del motore e libera le risorse.
     *
     * Chiude la finestra, pulisce la memoria della scena e termina il programma.
     */
    static void quit();

    // ========================================================================
    // GETTERS
    // ========================================================================

    /**
     * @brief Restituisce la telecamera attualmente attiva.
     * @return Puntatore condiviso (std::shared_ptr) alla telecamera attiva.
     */
    static std::shared_ptr<Camera> getActiveCamera();

    /**
     * @brief Restituisce la radice della scena attuale.
     * @return Puntatore condiviso (std::shared_ptr) al nodo radice della scena.
     */
    static std::shared_ptr<Node> getScene();

    // ========================================================================
    // SETTERS
    // ========================================================================

    /**
     * @brief Imposta il colore di sfondo della finestra (Clear Color).
     * @param red Componente rossa (0.0 - 1.0).
     * @param green Componente verde (0.0 - 1.0).
     * @param blue Componente blu (0.0 - 1.0).
     */
    static void setBackGround(const float red, const float green, const float blue);

    /**
     * @brief Sostituisce la scena corrente con una nuova.
     * @param newScene Puntatore condiviso al nodo radice della nuova scena.
     */
    static void setScene(const std::shared_ptr<Node> newScene);

    /**
     * @brief Imposta la telecamera attiva per il rendering.
     * @param newActiveCamera Puntatore condiviso alla nuova telecamera.
     */
    static void setActiveCamera(const std::shared_ptr<Camera> newActiveCamera);

    /**
     * @brief Imposta una stringa di testo da visualizzare in overlay sullo schermo.
     * @param newText Il testo da visualizzare.
     */
    static void setScreenText(const std::string newText);

    // ========================================================================
    // CALLBACKS DI INPUT
    // ========================================================================

    /**
     * @brief Imposta la funzione di callback per la gestione della tastiera (tasti ASCII).
     * @param newKeyboardCallback Puntatore a funzione: void func(unsigned char key, int mouseX, int mouseY).
     */
    static void setKeyboardCallback(void (*newKeyboardCallback) (const unsigned char key, const int mouseX, const int mouseY));

    /**
     * @brief Imposta la funzione di callback per gestire un effetto di lampeggiamento personalizzato.
     * @param callback Puntatore a funzione void func().
     */
    static void setBlinkingCallback(void (*callback)());

    /**
     * @brief Imposta la funzione di callback per la gestione dei click del mouse.
     * @param newMouseCallback Puntatore a funzione: void func(int button, int state, int mouseX, int mouseY).
     */
    static void setMouseCallback(void(*newMouseCallback)(int button, int state, int mouseX, int mouseY));

    /**
     * @brief Imposta la funzione di callback per i tasti speciali (frecce, F1-F12, ecc.).
     * @param newSpecialCallback Puntatore a funzione: void func(int key, int mouseX, int mouseY).
     */
    static void setMethodSpecialCallback(void(*newSpecialCallback) (int key, int mouseX, int mouseY));

    // ========================================================================
    // UTILITY DI RENDERING
    // ========================================================================

    /**
     * @brief Funzione interna di callback per il timer di FreeGLUT.
     * Gestisce il framerate e richiama l'aggiornamento.
     * @param value Valore intero passato dal timer precedente.
     */
    static void timerCallback(int value);

    /**
     * @brief Pulisce i buffer dello schermo (Colore e Profondit�).
     */
    static void clearScreen();

    /**
     * @brief Scambia i buffer anteriore e posteriore (Double Buffering).
     * Da chiamare alla fine del rendering del frame.
     */
    static void swapBuffers();

    // ========================================================================
    // GESTIONE OGGETTI (SCENE GRAPH)
    // ========================================================================

    /**
     * @brief Cerca un oggetto nella scena tramite il suo nome.
     * @param nameToFind Nome dell'oggetto da cercare.
     * @return Puntatore condiviso all'oggetto se trovato, `nullptr` altrimenti.
     */
    static std::shared_ptr<Node> findObjectByName(const std::string nameToFind);

    /**
     * @brief Cerca un oggetto nella scena tramite il suo ID univoco.
     * @param idToFind ID numerico dell'oggetto.
     * @return Puntatore condiviso all'oggetto se trovato, `nullptr` altrimenti.
     */
    static std::shared_ptr<Node> findObjectByID(int idToFind);

    /**
     * @brief Esegue il "Picking": trova il nodo visualizzato alle coordinate del mouse.
     * Utilizza un rendering con codifica colore per identificare l'oggetto.
     *
     * @param mouseX Coordinata X del mouse nella finestra.
     * @param mouseY Coordinata Y del mouse nella finestra.
     * @return Puntatore condiviso al nodo selezionato, o `nullptr` se si clicca sullo sfondo.
     */
    static std::shared_ptr<Node> getNodeByClick(int mouseX, int mouseY);

    /**
     * @brief Rimuove ricorsivamente un oggetto da un sotto-albero specifico.
     * @param nodeToRemove L'oggetto da rimuovere.
     * @param root Il nodo radice da cui iniziare la ricerca.
     * @return `true` se l'oggetto � stato trovato e rimosso, `false` altrimenti.
     */
    static bool removeObject(const std::shared_ptr<Node>& nodeToRemove, const std::shared_ptr<Node>& root);

    /**
     * @brief Rimuove un oggetto cercandolo nell'intera scena corrente.
     * @param nodeToRemove L'oggetto da rimuovere.
     * @return `true` se l'oggetto � stato rimosso, `false` altrimenti.
     */
    static bool removeObject(const std::shared_ptr<Node>& nodeToRemove);

    /**
     * @brief Rimuove tutti gli oggetti dalla scena corrente, lasciando la radice vuota.
     */
    static void removeAllObjects();

    /**
     * @brief Calcola la matrice di trasformazione globale (World Matrix) di un nodo.
     * Risale la gerarchia dei padri moltiplicando le matrici locali.
     *
     * @param node Il nodo di cui calcolare la trasformazione.
     * @return Matrice 4x4 glm::mat4 risultante.
     */
    static glm::mat4 getGlobalTransform(const std::shared_ptr<Node>& node);

    /**
     * @brief Calcola la posizione globale (World Position) di un nodo nello spazio 3D.
     * Estrae la posizione dalla matrice di trasformazione globale.
     *
     * @param node Il nodo di cui calcolare la posizione.
     * @return Vettore 3D glm::vec3 contenente le coordinate (x, y, z).
     */
    static glm::vec3 getGlobalPosition(const std::shared_ptr<Node>& node);

private:

    /**
     * @brief Callback interna per gestire il ridimensionamento della finestra.
     * Aggiorna il viewport OpenGL e la projection matrix della camera.
     * @param width Nuova larghezza.
     * @param height Nuova altezza.
     */
    static void resizeCallback(const int width, const int height);

    // Helper ricorsivi privati per la ricerca
    static std::shared_ptr<Node> findObjectByName(const std::string nameToFind, const std::shared_ptr<Node> root);
    static std::shared_ptr<Node> findObjectByID(int idToFind, const std::shared_ptr<Node> root);

    // Puntatori a funzione per le callback
    static void (*blinkingCallback)(); ///< Funzione di callback per il lampeggiamento.

    // Stato del motore
    static bool isInitializedFlag;  ///< True se init() � stato chiamato con successo.
    static bool isRunningFlag;      ///< True se il loop principale � attivo.
    static int windowId;            ///< Handle della finestra GLUT.

    // Dimensioni finestra
    static int windowWidth;
    static int windowHeight;

    // Risorse condivise
    static std::shared_ptr<Node> scene;             ///< Radice del Scene Graph.
    static std::shared_ptr<Camera> activeCamera;    ///< Camera usata per il rendering.
    static std::shared_ptr<Material> shadowMaterial;///< Materiale di default per le ombre.


    // UI e Statistiche
    static std::string screenText;  ///< Testo overlay.
    static int frames;              ///< Contatore frame per calcolo FPS.
    static float fps;               ///< Valore FPS corrente.
};
