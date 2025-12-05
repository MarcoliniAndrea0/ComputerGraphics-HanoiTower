#pragma once

#include "Common.h"
#include "Light.h"

/**
 * @file PointLight.h
 * @brief Dichiarazione della classe PointLight.
 *
 * La classe `PointLight` rappresenta una sorgente di luce che emette luce in tutte le direzioni
 * da una posizione specifica, con un effetto visibile fino a una distanza definita dal raggio.
 */

 /**
  * @class PointLight
  * @brief Rappresenta una luce puntiforme.
  *
  * Una `PointLight` emette luce in tutte le direzioni da una posizione specifica
  * ed è utilizzata per simulare sorgenti di luce come una lampadina o una candela.
  *
  * La luce puntiforme ha un effetto visibile fino a una certa distanza, definita dal raggio.
  * Oltre questa distanza, la luce si attenua gradualmente.
  */
class LIB_API PointLight : public Light
{
public:
    /**
     * @brief Costruttore di default.
     *
     * Inizializza una `PointLight` con un raggio predefinito di `1.0f`.
     */
    PointLight();

    virtual ~PointLight() = default;

    /**
     * @brief Imposta il raggio della luce puntiforme.
     *
     * @param newRadius La distanza massima di influenza della luce.
     */
    void setRadius(const float newRadius);

    /**
     * @brief Renderizza la luce puntiforme.
     *
     * Configura i parametri della luce puntiforme in OpenGL per la scena corrente.
     *
     * @param viewMatrix La matrice di visualizzazione da utilizzare.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:
    float _radius; ///< Distanza massima di influenza della luce.
};
