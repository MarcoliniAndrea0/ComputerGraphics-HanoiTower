#pragma once

#include "Common.h"
#include "Light.h"

/**
 * @file SpotLight.h
 * @brief Dichiarazione della classe SpotLight per rappresentare una luce spot.
 *
 * Una `SpotLight` emette luce a forma di cono con un angolo e una distanza specifici.
 */

 /**
  * @class SpotLight
  * @brief Rappresenta una luce spot in una scena 3D.
  *
  * Una `SpotLight` emette luce a forma di cono e viene utilizzata per simulare
  * effetti come una torcia o un riflettore. La luce è direzionale e ha un'attenuazione
  * in base al raggio e all'angolo di cutoff.
  */
class LIB_API SpotLight : public Light
{
public:
    /**
     * @brief Costruttore di default.
     *
     * Inizializza una `SpotLight` con parametri di default:
     * - Direzione: `(0.0f, 1.0f, 0.0f)`.
     * - Cutoff: `45.0f`.
     * - Esponente: `8.0f`.
     * - Raggio: `1.0f`.
     */
    SpotLight();

    virtual ~SpotLight() = default;

    /**
     * @brief Imposta l'angolo di cutoff per questa `SpotLight`.
     *
     * @param newCutoff L'angolo di cutoff (in gradi).
     */
    void setCutoff(const float newCutoff);

    /**
     * @brief Imposta il raggio della luce.
     *
     * @param newRadius La distanza massima di influenza della luce.
     */
    void setRadius(const float newRadius);

    /**
     * @brief Imposta l'esponente della luce.
     *
     * @param newExponent Il livello di concentrazione della luce nel cono.
     */
    void setExponent(const float newExponent);

    /**
     * @brief Imposta la direzione della luce.
     *
     * @param newDirection Il vettore direzionale della luce.
     */
    void setDirection(const glm::vec3 newDirection);

    /**
     * @brief Renderizza la luce spot nella scena.
     *
     * @param viewMatrix La matrice di vista da utilizzare per il rendering.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:
    float _cutoff; ///< Angolo del cono di luce (in gradi).
    float _radius; ///< Distanza massima di influenza della luce.
    float _exponent; ///< Concentrazione della luce nel cono.
    glm::vec3 _direction; ///< Direzione della luce.
};
