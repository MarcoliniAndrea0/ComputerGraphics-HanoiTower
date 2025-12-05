#include "PerspectiveCamera.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Costruttore della telecamera prospettica.
 *
 * Inizializza una nuova istanza di `PerspectiveCamera` con i parametri di default.
 * Questa telecamera applica una proiezione prospettica per il rendering della scena.
 */
PerspectiveCamera::PerspectiveCamera()
    : Camera{ "PerspectiveCamera" }
{}

///// Render PerspectiveCamera

/**
 * @brief Renderizza la scena utilizzando una proiezione prospettica.
 *
 * Configura una matrice di proiezione prospettica in OpenGL e applica il rendering
 * con l'effetto di prospettiva. Questa funzione viene chiamata automaticamente da MyEngine
 * e non deve essere invocata manualmente.
 *
 * @param viewMatrix La matrice di vista da utilizzare per il rendering di questo oggetto.
 */
void LIB_API PerspectiveCamera::render(const glm::mat4 viewMatrix) const
{
    // Verifica se la telecamera è attiva
    if (!this->_isActive)
        return;

    Node::render(viewMatrix);

    // Calcola il rapporto d'aspetto della finestra per mantenere proporzioni corrette
    const float aspectRatio = static_cast<float>(this->_windowWidth) / static_cast<float>(this->_windowHeight);

    // Configura la matrice di proiezione prospettica
    const glm::mat4 perspective_matrix = glm::perspective(glm::radians(this->_fov), aspectRatio, this->_nearClipping, this->_farClipping);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(perspective_matrix));
}
