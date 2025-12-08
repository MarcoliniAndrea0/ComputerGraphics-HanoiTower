#include "camera.h"

/**
 * @brief Costruttore della classe Camera.
 *
 * Inizializza una nuova istanza di Camera con valori di default:
 * - Campo visivo (FOV): 90 gradi
 * - Piano di clipping vicino: 0.01
 * - Piano di clipping lontano: 1000.0
 * - Stato attivo: false
 * - Dimensioni della finestra: larghezza e altezza impostate a 0
 * - Priorit�: 2
 *
 * @param type Tipo della camera.
 */
Camera::Camera(const std::string& type)
    : Node{ type }
{
    this->setFov(90.0f);
    this->setNearClipping(0.01f);
    this->setFarClipping(1000.0f);
    this->_isActive = false;
    this->setWindowSize(0, 0);
    this->setPriority(2);
}

/**
 * @brief Restituisce il campo visivo (FOV) della camera.
 * @return Campo visivo (FOV) in gradi.
 */
float Camera::getFov() const {
    return this->_fov;
}

/**
 * @brief Restituisce la distanza del piano di clipping vicino.
 * @return Piano di clipping vicino.
 */
float Camera::getNearClipping() const {
    return this->_nearClipping;
}

/**
 * @brief Restituisce la distanza del piano di clipping lontano.
 * @return Piano di clipping lontano.
 */
float Camera::getFarClipping() const {
    return this->_farClipping;
}

/**
 * @brief Restituisce la larghezza della finestra.
 * @return Larghezza della finestra.
 */
int Camera::getWindowWidth() const {
    return this->_windowWidth;
}

/**
 * @brief Restituisce l'altezza della finestra.
 * @return Altezza della finestra.
 */
int Camera::getWindowHeight() const {
    return this->_windowHeight;
}

/**
 * @brief Verifica se la camera � attiva.
 * @return true se la camera � attiva, false altrimenti.
 */
bool Camera::isActive() const {
    return this->_isActive;
}

/**
 * @brief Calcola la matrice inversa della camera.
 * @return Matrice inversa della camera.
 */
glm::mat4 Camera::getInverseMatrix() {
    return glm::inverse(getLocalMatrix());
}

/**
 * @brief Imposta la distanza del piano di clipping vicino.
 * @param newNearClipping Nuova distanza del piano di clipping vicino.
 */
void Camera::setNearClipping(const float newNearClipping) {
    this->_nearClipping = newNearClipping;
}

/**
 * @brief Imposta la distanza del piano di clipping lontano.
 * @param newFarClipping Nuova distanza del piano di clipping lontano.
 */
void Camera::setFarClipping(const float newFarClipping) {
    this->_farClipping = newFarClipping;
}

/**
 * @brief Imposta le dimensioni della finestra.
 * @param newWidth Nuova larghezza della finestra.
 * @param newHeight Nuova altezza della finestra.
 */
void Camera::setWindowSize(const int newWidth, const int newHeight) {
    this->_windowWidth = newWidth;
    this->_windowHeight = newHeight;
}

/**
 * @brief Imposta il campo visivo (FOV) della camera.
 * @param newFov Nuovo campo visivo (FOV) in gradi.
 */
void Camera::setFov(const float newFov) {
    this->_fov = newFov;
}

/**
 * @brief Imposta lo stato attivo della camera.
 * @param newIsActive Nuovo stato attivo della camera.
 */
void Camera::setActive(const bool newIsActive) {
    this->_isActive = newIsActive;
}
