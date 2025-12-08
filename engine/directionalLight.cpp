#include "directionalLight.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Costruttore della classe DirectionalLight.
 *
 * Crea una nuova istanza di `DirectionalLight` con i seguenti parametri di default:
 * - Direzione: (0.0f, 1.0f, 0.0f) (Verso l'alto).
 *
 * Nota: La direzione predefinita indica che la luce � orientata verso l'alto lungo l'asse Y,
 * simulando l'effetto di una luce proveniente dall'alto, come il sole.
 */
DirectionalLight::DirectionalLight()
    : Light{ "DirectionalLight" }
{
    this->setDirection(glm::vec3(0.0f, 1.0f, 0.0f));
}

/**
 * @brief Modifica la direzione verso cui la luce viene puntata.
 *
 * @param newDirection Il nuovo vettore di direzione per la luce.
 */
void LIB_API DirectionalLight::setDirection(const glm::vec3 newDirection)
{
    this->_direction = newDirection;
}

/**
 * @brief Renderizza la luce direzionale.
 *
 * Questa funzione viene chiamata automaticamente da MyEngine e non deve essere invocata manualmente.
 *
 * @param viewMatrix La matrice di vista da utilizzare per il rendering.
 *
 * Dettagli:
 * - Abilita la sorgente di luce specificata dall'ID della luce.
 * - Imposta la posizione della luce come un vettore 4D, dove il valore `w = 0.0f` indica una luce direzionale.
 * - Imposta i colori ambientale, diffuso e speculare della luce.
 */
void LIB_API DirectionalLight::render(const glm::mat4 viewMatrix) const
{
    // Chiama il metodo della classe base `Node` per gestire il rendering di base.
    Node::render(viewMatrix);

    // Abilita la luce corrente in OpenGL.
    glEnable(GL_LIGHT0 + this->_lightId);

    // Posizione della luce: la componente w = 0.0f indica una luce direzionale.
    const glm::vec4 lightPosition(_direction, 0.0f);

    // Colori ambientale, diffuso e speculare della luce.
    const glm::vec4 ambient(this->_ambientColor, 1.0f);
    const glm::vec4 diffuse(this->_diffuseColor, 1.0f);
    const glm::vec4 specular(this->_specularColor, 1.0f);

    // Ottiene l'ID della luce corrente.
    const int currentLight = Light::getCurrentLight(this->_lightId);

    // Configura le propriet� della luce in OpenGL.
    glLightfv(currentLight, GL_POSITION, glm::value_ptr(lightPosition));
    glLightfv(currentLight, GL_AMBIENT, glm::value_ptr(ambient));
    glLightfv(currentLight, GL_DIFFUSE, glm::value_ptr(diffuse));
    glLightfv(currentLight, GL_SPECULAR, glm::value_ptr(specular));
}
