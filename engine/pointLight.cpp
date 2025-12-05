#include "PointLight.h"

#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Costruttore della classe PointLight.
 *
 * Crea una nuova istanza di PointLight con i seguenti parametri di default:
 * - Raggio: `1.0f`.
 *
 * La luce puntiforme emette luce in tutte le direzioni da una posizione specifica con
 * un effetto visibile fino a una distanza definita dal raggio.
 */
PointLight::PointLight()
    : Light{ "PointLight" }
{
    this->setRadius(5.0f);
}

///// Setter

/**
 * @brief Imposta il raggio della luce `PointLight`.
 *
 * Il raggio determina la distanza massima fino alla quale la luce ha effetto.
 * Un raggio maggiore aumenta l'area illuminata dalla luce.
 *
 * @param newRadius Il nuovo raggio della luce `PointLight`.
 */
void LIB_API PointLight::setRadius(const float newRadius)
{
    this->_radius = newRadius;
}

///// Render PointLight

/**
 * @brief Renderizza la luce puntiforme.
 *
 * Questa funzione configura i parametri della luce puntiforme in OpenGL,
 * inclusa la posizione e i colori della luce. Viene chiamata automaticamente da MyEngine.
 *
 * @param viewMatrix La matrice di visualizzazione da utilizzare per renderizzare questo oggetto.
 */
void LIB_API PointLight::render(const glm::mat4 viewMatrix) const
{
    Node::render(viewMatrix);

    // Abilita la sorgente di luce specificata dall'ID corrente.
    glEnable(GL_LIGHT0 + this->_lightId);

    // Definisce la posizione della luce come (0.0f, 0.0f, 0.0f, 1.0f),
    // che indica una luce puntiforme.
    const glm::vec4 lightPosition(0.0f, 0.0f, 0.0f, 1.0f);

    const glm::vec4 ambient(this->_ambientColor, 1.0f);
    const glm::vec4 diffuse(this->_diffuseColor, 1.0f);
    const glm::vec4 specular(this->_specularColor, 1.0f);

    // Valore speciale per indicare una luce puntiforme.
    float cutoff = 180.0f;

    // Calcola l'attenuazione costante della luce in base al raggio.
    const float constantAttenuation = 1.0f / this->_radius;

    // Ottiene l'identificatore della luce corrente.
    const int currentLight = Light::getCurrentLight(this->_lightId);

    // Configura i parametri della luce in OpenGL.
    glLightfv(currentLight, GL_POSITION, glm::value_ptr(lightPosition));
    glLightfv(currentLight, GL_AMBIENT, glm::value_ptr(ambient));
    glLightfv(currentLight, GL_DIFFUSE, glm::value_ptr(diffuse));
    glLightfv(currentLight, GL_SPECULAR, glm::value_ptr(specular));
    glLightfv(currentLight, GL_SPOT_CUTOFF, &cutoff);
    glLightf(currentLight, GL_CONSTANT_ATTENUATION, constantAttenuation);
}
