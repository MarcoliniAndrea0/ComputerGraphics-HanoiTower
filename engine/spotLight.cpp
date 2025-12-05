#include "SpotLight.h"

#include <GL/freeglut.h>
#include "glm/ext.hpp"

/**
 * @brief Costruttore della classe `SpotLight`.
 *
 * Crea una nuova istanza di `SpotLight` con i seguenti parametri di default:
 *
 * - Direzione: `(0.0f, 1.0f, 0.0f)` (verso l'alto).
 * - Angolo di cutoff: `45.0f` gradi, definisce la larghezza del cono.
 * - Esponente: `8.0f`, influenza la concentrazione della luce nel cono.
 * - Raggio: `1.0f`, determina la distanza alla quale la luce ha effetto.
 */
SpotLight::SpotLight()
    : Light{ "SpotLight" }
{
    this->setDirection(glm::vec3(0.0f, 1.0f, 0.0f));
    this->setCutoff(45.0f);
    this->setExponent(8.0f);
    this->setRadius(1.0f);
}

///// Setter

/**
 * @brief Imposta l'angolo di cutoff per questa `SpotLight`.
 *
 * L'angolo di cutoff determina la larghezza del cono di luce.
 *
 * @param newCutoff Il nuovo valore di cutoff (in gradi) per questa luce spot.
 */
void LIB_API SpotLight::setCutoff(const float newCutoff)
{
    this->_cutoff = newCutoff;
}

/**
 * @brief Imposta il raggio della `SpotLight`.
 *
 * Questo valore definisce la distanza alla quale la luce ha effetto.
 *
 * @param newRadius Il nuovo raggio per questa luce spot.
 */
void LIB_API SpotLight::setRadius(const float newRadius)
{
    this->_radius = newRadius;
}

/**
 * @brief Imposta l'esponente della `SpotLight`.
 *
 * L'esponente controlla la concentrazione della luce nel cono.
 * Un valore più alto rende il cono di luce più stretto e concentrato.
 *
 * @param newExponent Il nuovo esponente per questa `SpotLight`.
 */
void LIB_API SpotLight::setExponent(const float newExponent)
{
    this->_exponent = newExponent;
}

/**
 * @brief Cambia la direzione verso cui la luce viene puntata.
 *
 * La direzione specifica il vettore verso cui è orientato il cono di luce.
 *
 * @param newDirection La nuova direzione della luce.
 */
void LIB_API SpotLight::setDirection(const glm::vec3 newDirection)
{
    this->_direction = newDirection;
}

///// Render spotLight

/**
 * @brief Renderizza la luce spot.
 *
 * Questa funzione configura i parametri della luce spot in OpenGL,
 * incluse la posizione, la direzione e i colori della luce.
 *
 * @param viewMatrix La matrice di vista da utilizzare per il rendering di questo oggetto.
 */
void LIB_API SpotLight::render(const glm::mat4 viewMatrix) const
{
    Node::render(viewMatrix);

    // Abilita la sorgente di luce specificata dall'ID corrente.
    glEnable(GL_LIGHT0 + this->_lightId);

    // Posizione della luce nel sistema di coordinate della scena.
    const glm::vec4 lightPosition(this->_direction, 1.0f);

    // Direzione del cono di luce.
    const glm::vec3 lightDirection(0.0f, -0.1f, 0.0f);

    // Colori della luce.
    const glm::vec4 ambient(this->_ambientColor, 1.0f);
    const glm::vec4 diffuse(this->_diffuseColor, 1.0f);
    const glm::vec4 specular(this->_specularColor, 1.0f);

    // Calcolo dell'attenuazione in base al raggio.
    const float constantAttenuation = 1.0f / (this->_radius / 100);

    // Ottiene l'identificatore della luce corrente.
    const int currentLight = Light::getCurrentLight(this->_lightId);

    // Configura i parametri della luce in OpenGL.
    glLightfv(currentLight, GL_POSITION, glm::value_ptr(lightPosition));
    glLightfv(currentLight, GL_SPOT_DIRECTION, glm::value_ptr(lightDirection));
    glLightfv(currentLight, GL_AMBIENT, glm::value_ptr(ambient));
    glLightfv(currentLight, GL_DIFFUSE, glm::value_ptr(diffuse));
    glLightfv(currentLight, GL_SPECULAR, glm::value_ptr(specular));
    glLightfv(currentLight, GL_SPOT_CUTOFF, &this->_cutoff);
    glLightf(currentLight, GL_CONSTANT_ATTENUATION, constantAttenuation);
    glLightf(currentLight, GL_SPOT_EXPONENT, this->_exponent);
}
