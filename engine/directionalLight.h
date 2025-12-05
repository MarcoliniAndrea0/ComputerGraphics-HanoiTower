#pragma once

#include "Light.h"
#include "Common.h"

/**
 * @class DirectionalLight
 * @brief Rappresenta una luce direzionale.
 *
 * Una luce direzionale proviene da una distanza infinita con una direzione specifica.
 * Questa luce simula fonti di illuminazione come il sole, emettendo raggi paralleli
 * che illuminano la scena da una direzione specifica.
 */
class LIB_API DirectionalLight : public Light
{

public:

    /**
     * @brief Costruttore di default della classe `DirectionalLight`.
     *
     * Inizializza la luce direzionale con una direzione di default
     * orientata verso l'alto (0.0f, 1.0f, 0.0f).
     */
    DirectionalLight();
    virtual ~DirectionalLight() = default;

    /**
     * @brief Modifica la direzione della luce direzionale.
     * @param newDirection Il nuovo vettore di direzione per la luce.
     */
    void setDirection(const glm::vec3 newDirection);

    /**
     * @brief Renderizza la luce direzionale.
     *
     * @param viewMatrix La matrice di vista utilizzata per il rendering.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:

    glm::vec3 _direction; ///< Direzione della luce.
};
