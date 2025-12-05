#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include "Node.h"
#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>

/**
 * @class Camera
 * @brief Classe base per rappresentare camere nella scena.
 *
 * La classe `Camera` rappresenta una camera nella scena 3D e fornisce funzionalità di base
 * come il campo visivo (FOV), i piani di clipping, le dimensioni della finestra e lo stato attivo.
 */
class LIB_API Camera : public Node
{
public:
    /**
     * @brief Costruttore della classe `Camera`.
     * @param type Il tipo di camera, passato alla classe base `Node`.
     */
    Camera(const std::string& type);
    virtual ~Camera() = default;

    // Getter
    /**
     * @brief Calcola e restituisce la matrice inversa della camera.
     * @return La matrice inversa come `glm::mat4`.
     */
    glm::mat4 getInverseMatrix();

    /**
     * @brief Restituisce il campo visivo (FOV) della camera.
     * @return Il valore del campo visivo.
     */
    float getFov() const;

    /**
     * @brief Restituisce la distanza del piano di clipping vicino.
     * @return La distanza del piano di clipping vicino.
     */
    float getNearClipping() const;

    /**
     * @brief Restituisce la distanza del piano di clipping lontano.
     * @return La distanza del piano di clipping lontano.
     */
    float getFarClipping() const;

    /**
     * @brief Restituisce la larghezza della finestra.
     * @return La larghezza della finestra.
     */
    int getWindowWidth() const;

    /**
     * @brief Restituisce l'altezza della finestra.
     * @return L'altezza della finestra.
     */
    int getWindowHeight() const;

    /**
     * @brief Verifica se la camera è attiva.
     * @return `true` se la camera è attiva, altrimenti `false`.
     */
    bool isActive() const;

    // Setter 
    /**
     * @brief Imposta la distanza del piano di clipping vicino.
     * @param newNearClipping La nuova distanza del piano di clipping vicino.
     */
    void setNearClipping(const float newNearClipping);

    /**
     * @brief Imposta la distanza del piano di clipping lontano.
     * @param newFarClipping La nuova distanza del piano di clipping lontano.
     */
    void setFarClipping(const float newFarClipping);

    /**
     * @brief Imposta le dimensioni della finestra della camera.
     * @param newWidth La nuova larghezza della finestra.
     * @param newHeight La nuova altezza della finestra.
     */
    void setWindowSize(const int newWidth, const int newHeight);

    /**
     * @brief Imposta il campo visivo (FOV) della camera.
     * @param newFov Il nuovo valore del campo visivo.
     */
    void setFov(const float newFov);

    /**
     * @brief Imposta lo stato attivo della camera.
     * @param newIsActive `true` per attivare la camera, `false` per disattivarla.
     */
    void setActive(const bool newIsActive);

protected:
    float _fov;             ///< Campo visivo della camera.
    float _nearClipping;    ///< Distanza del piano di clipping vicino.
    float _farClipping;     ///< Distanza del piano di clipping lontano.
    int _windowWidth;       ///< Larghezza della finestra della camera.
    int _windowHeight;      ///< Altezza della finestra della camera.
    bool _isActive;         ///< Indica se la camera è attiva.
};
