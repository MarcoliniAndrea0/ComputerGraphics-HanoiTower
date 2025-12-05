#pragma once

#define LIB_NAME      "Engine"
#define LIB_VERSION   10

#include <sstream>

#include "Camera.h"
#include "Common.h"
#include "Material.h"
#include "List.h"
#include "Mesh.h"

/**
 * @class Engine
 * @brief La classe principale del motore grafico.
 *
 * Gestisce l'inizializzazione del motore, la gestione delle finestre, il rendering,
 * e la gestione della scena e delle camere.
 * Necessita di una chiamata a `init` per inizializzare il motore prima dell'uso
 * e `quit` per liberare le risorse al termine.
 */
class LIB_API Engine
{
public:

    /**
     * @brief Inizializza il motore grafico.
     * @param windowTitle Titolo della finestra.
     * @param windowWidth Larghezza della finestra.
     * @param windowHeight Altezza della finestra.
     */
    static void init(const std::string windowTitle, const int windowWidth, const int windowHeight);

    // Getter

    /**
     * @brief Restituisce la telecamera attiva.
     * @return Puntatore condiviso alla telecamera attiva.
     */
    static std::shared_ptr<Camera> getActiveCamera();

    /**
     * @brief Restituisce la scena attuale.
     * @return Puntatore condiviso alla scena.
     */
    static std::shared_ptr<Node> getScene();

    // Setter

    /**
     * @brief Imposta il colore di sfondo.
     * @param red Componente rossa (0.0 - 1.0).
     * @param green Componente verde (0.0 - 1.0).
     * @param blue Componente blu (0.0 - 1.0).
     */
    static void setBackGround(const float red, const float green, const float blue);

    /**
     * @brief Imposta la scena corrente.
     * @param newScene Puntatore condiviso alla nuova scena.
     */
    static void setScene(const std::shared_ptr<Node> newScene);

    /**
     * @brief Imposta la telecamera attiva.
     * @param newActiveCamera Puntatore condiviso alla nuova telecamera attiva.
     */
    static void setActiveCamera(const std::shared_ptr<Camera> newActiveCamera);

    /**
     * @brief Imposta il testo da visualizzare sullo schermo.
     * @param newText Testo da visualizzare.
     */
    static void setScreenText(const std::string newText);

    // Set Callback

    /**
     * @brief Imposta la funzione di callback per la tastiera.
     * @param newKeyboardCallback Funzione di callback per la tastiera.
     */
    static void setKeyboardCallback(void (*newKeyboardCallback) (const unsigned char key, const int mouseX, const int mouseY));

    /**
     * @brief Imposta la funzione di callback per il lampeggiamento.
     * @param callback Funzione di callback per il lampeggiamento.
     */
    static void setBlinkingCallback(void (*callback)());

    /**
     * @brief Imposta la funzione di callback per il mouse.
     * @param newMouseCallback Funzione di callback per il mouse.
     */
    static void setMouseCallback(void(*newMouseCallback)(int button, int state, int mouseX, int mouseY));

    /**
     * @brief Imposta la funzione di callback per i tasti speciali.
     * @param newSpecialCallback Funzione di callback per i tasti speciali.
     */
    static void setMethodSpecialCallback(void(*newSpecialCallback) (int key, int mouseX, int mouseY));

    // Funzioni di stato e rendering

    /**
     * @brief Verifica se il motore e' in esecuzione.
     * @return `true` se il motore e' in esecuzione, altrimenti `false`.
     */
    static bool isRunning();

    /**
     * @brief Esegue il rendering della scena.
     */
    static void render();

    /**
     * @brief Funzione di callback per il timer.
     * @param value Valore associato al timer.
     */
    static void timerCallback(int value);

    /**
     * @brief Aggiorna lo stato del motore.
     */
    static void update();

    /**
     * @brief Pulisce lo schermo.
     */
    static void clearScreen();

    /**
     * @brief Scambia i buffer per il rendering.
     */
    static void swapBuffers();

    /**
     * @brief Ferma l'esecuzione del motore.
     */
    static void stop();

    /**
     * @brief Termina l'esecuzione del motore e libera le risorse.
     */
    static void quit();

    // Funzioni per la gestione degli oggetti

    /**
     * @brief Trova un oggetto nella scena tramite il nome.
     * @param nameToFind Nome dell'oggetto da cercare.
     * @return Puntatore condiviso all'oggetto trovato, o `nullptr` se non trovato.
     */
    static std::shared_ptr<Node> findObjectByName(const std::string nameToFind);

    /**
     * @brief Trova un oggetto nella scena tramite l'ID.
     * @param idToFind ID dell'oggetto da cercare.
     * @return Puntatore condiviso all'oggetto trovato, o `nullptr` se non trovato.
     */
    static std::shared_ptr<Node> findObjectByID(int idToFind);

    /**
     * @brief Ottiene il nodo selezionato tramite un click.
     * @param mouseX Coordinata X del mouse.
     * @param mouseY Coordinata Y del mouse.
     * @return Puntatore condiviso al nodo selezionato.
     */
    static std::shared_ptr<Node> getNodeByClick(int mouseX, int mouseY);

    /**
     * @brief Rimuove un oggetto dalla scena.
     * @param nodeToRemove Puntatore condiviso all'oggetto da rimuovere.
     * @param root Radice della scena.
     * @return `true` se l'oggetto e' stato rimosso, altrimenti `false`.
     */
    static bool removeObject(const std::shared_ptr<Node>& nodeToRemove, const std::shared_ptr<Node>& root);

    /**
     * @brief Rimuove un oggetto dalla scena.
     * @param nodeToRemove Puntatore condiviso all'oggetto da rimuovere.
     * @return `true` se l'oggetto e' stato rimosso, altrimenti `false`.
     */
    static bool removeObject(const std::shared_ptr<Node>& nodeToRemove);

    /**
     * @brief Rimuove tutti gli oggetti dalla scena.
     */
    static void removeAllObjects();
    static glm::mat4 getGlobalTransform(const std::shared_ptr<Node>& node);
    static glm::vec3 getGlobalPosition(const std::shared_ptr<Node>& node);

private:

    /**
     * @brief Funzione di callback per il ridimensionamento della finestra.
     * @param width Nuova larghezza della finestra.
     * @param height Nuova altezza della finestra.
     */
    static void resizeCallback(const int width, const int height);

    static void (*blinkingCallback)(); ///< Funzione di callback per il lampeggiamento.

    static std::shared_ptr<Node> findObjectByName(const std::string nameToFind, const std::shared_ptr<Node> root);
    static std::shared_ptr<Node> findObjectByID(int idToFind, const std::shared_ptr<Node> root);

    static bool isInitializedFlag; ///< Flag che indica se il motore e' stato inizializzato.
    static bool isRunningFlag; ///< Flag che indica se il motore e' in esecuzione.
    static int windowId;  ///< ID della finestra.

    static int windowWidth; ///< Larghezza della finestra.
    static int windowHeight; ///< Altezza della finestra.

    static std::shared_ptr<Node> scene; ///< Puntatore alla scena.
    static std::shared_ptr<Camera> activeCamera;  ///< Puntatore alla telecamera attiva.
    static std::shared_ptr<Material> shadowMaterial; ///< Puntatore al materiale per le ombre.
    static std::string screenText; ///< Testo da visualizzare sullo schermo.
    static int frames; ///< Contatore di frames
    static float fps; ///< Frames al secondo
};
