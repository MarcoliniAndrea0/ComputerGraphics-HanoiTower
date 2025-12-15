/**
 * @file camera.h
 * @brief Definizione della classe base Camera per il sistema di telecamere del motore grafico.
 *
 * Questo file contiene la dichiarazione della classe Camera, che rappresenta
 * la classe base astratta per tutte le telecamere nel sistema di rendering.
 * Fornisce funzionalità comuni come campo visivo, piani di clipping e gestione
 * dello stato attivo.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "node.h"
#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

 /**
  * @class Camera
  * @brief Classe base per rappresentare telecamere nella scena 3D.
  *
  * La classe Camera rappresenta una telecamera virtuale nella scena 3D e fornisce
  * funzionalità di base come il campo visivo (FOV), i piani di clipping near e far,
  * le dimensioni della finestra di rendering e la gestione dello stato attivo.
  *
  * Questa classe eredita da Node, permettendo alle telecamere di far parte del
  * grafo della scena e di supportare trasformazioni gerarchiche.
  *
  * Le classi derivate devono implementare i dettagli specifici della proiezione
  * (es. prospettica, ortografica).
  *
  * @see Node
  * @see PerspectiveCamera
  */
class LIB_API Camera : public Node
{
public:
    /**
     * @brief Costruttore della classe Camera.
     *
     * Inizializza una nuova telecamera con il tipo specificato.
     * Il tipo viene passato alla classe base Node per identificare
     * il tipo di oggetto nella scena.
     *
     * @param type Stringa che identifica il tipo di camera (es. "PerspectiveCamera")
     */
    Camera(const std::string& type);

    /**
     * @brief Distruttore virtuale di default.
     *
     * Permette la corretta distruzione delle classi derivate.
     */
    virtual ~Camera() = default;

    // ========================================================================
    // GETTER METHODS
    // ========================================================================

    /**
     * @brief Calcola e restituisce la matrice inversa della telecamera.
     *
     * Questa matrice è utilizzata per trasformare le coordinate dal sistema
     * di riferimento della camera allo spazio world. È fondamentale per
     * il calcolo della view matrix nel pipeline di rendering.
     *
     * @return La matrice inversa 4x4 della telecamera come glm::mat4
     */
    glm::mat4 getInverseMatrix();

    /**
     * @brief Restituisce il campo visivo (Field of View) della telecamera.
     *
     * Il FOV determina l'ampiezza dell'angolo di visione della telecamera.
     * Un valore più alto crea un effetto "wide angle", mentre un valore
     * più basso crea un effetto "zoom".
     *
     * @return Il valore del campo visivo in gradi
     */
    float getFov() const;

    /**
     * @brief Restituisce la distanza del piano di clipping vicino (near plane).
     *
     * Gli oggetti più vicini di questa distanza dalla telecamera non vengono
     * renderizzati. Questo valore deve essere maggiore di zero.
     *
     * @return La distanza del piano di clipping vicino
     */
    float getNearClipping() const;

    /**
     * @brief Restituisce la distanza del piano di clipping lontano (far plane).
     *
     * Gli oggetti più lontani di questa distanza dalla telecamera non vengono
     * renderizzati. Questo valore determina la profondità massima della scena visibile.
     *
     * @return La distanza del piano di clipping lontano
     */
    float getFarClipping() const;

    /**
     * @brief Restituisce la larghezza della finestra di rendering.
     *
     * @return La larghezza della finestra in pixel
     */
    int getWindowWidth() const;

    /**
     * @brief Restituisce l'altezza della finestra di rendering.
     *
     * @return L'altezza della finestra in pixel
     */
    int getWindowHeight() const;

    /**
     * @brief Verifica se la telecamera è attualmente attiva.
     *
     * Solo la telecamera attiva viene utilizzata per il rendering della scena.
     * Tipicamente, in una scena c'è una sola telecamera attiva alla volta.
     *
     * @return true se la telecamera è attiva, false altrimenti
     */
    bool isActive() const;

    // ========================================================================
    // SETTER METHODS
    // ========================================================================

    /**
     * @brief Imposta la distanza del piano di clipping vicino.
     *
     * Modifica la distanza minima alla quale gli oggetti vengono renderizzati.
     * Valori troppo piccoli possono causare problemi di z-fighting.
     *
     * @param newNearClipping La nuova distanza del piano di clipping vicino (deve essere > 0)
     */
    void setNearClipping(const float newNearClipping);

    /**
     * @brief Imposta la distanza del piano di clipping lontano.
     *
     * Modifica la distanza massima alla quale gli oggetti vengono renderizzati.
     * Questo valore dovrebbe essere significativamente maggiore del near clipping
     * per evitare problemi di precisione del depth buffer.
     *
     * @param newFarClipping La nuova distanza del piano di clipping lontano (deve essere > nearClipping)
     */
    void setFarClipping(const float newFarClipping);

    /**
     * @brief Imposta le dimensioni della finestra di rendering della telecamera.
     *
     * Aggiorna sia la larghezza che l'altezza della finestra. Questi valori
     * sono utilizzati per calcolare l'aspect ratio nella matrice di proiezione.
     *
     * @param newWidth La nuova larghezza della finestra in pixel
     * @param newHeight La nuova altezza della finestra in pixel
     */
    void setWindowSize(const int newWidth, const int newHeight);

    /**
     * @brief Imposta il campo visivo (FOV) della telecamera.
     *
     * Modifica l'ampiezza dell'angolo di visione. Valori tipici sono
     * compresi tra 45 e 90 gradi per simulare la visione umana.
     *
     * @param newFov Il nuovo valore del campo visivo in gradi
     */
    void setFov(const float newFov);

    /**
     * @brief Imposta lo stato attivo della telecamera.
     *
     * Attiva o disattiva questa telecamera per il rendering.
     * È responsabilità dell'applicazione assicurarsi che solo una
     * telecamera sia attiva alla volta.
     *
     * @param newIsActive true per attivare la telecamera, false per disattivarla
     */
    void setActive(const bool newIsActive);

protected:
    float _fov;             ///< Campo visivo (Field of View) in gradi
    float _nearClipping;    ///< Distanza del piano di clipping vicino
    float _farClipping;     ///< Distanza del piano di clipping lontano
    int _windowWidth;       ///< Larghezza della finestra di rendering in pixel
    int _windowHeight;      ///< Altezza della finestra di rendering in pixel
    bool _isActive;         ///< Flag che indica se questa telecamera è attiva per il rendering
};