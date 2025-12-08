#pragma once

#include "Common.h"
#include "camera.h"

/**
 * @file PerspectiveCamera.h
 * @brief Dichiarazione della classe `PerspectiveCamera`.
 *
 * La classe `PerspectiveCamera` rappresenta una telecamera che utilizza una proiezione prospettica
 * per simulare la percezione visiva degli oggetti in base alla distanza dalla telecamera.
 */

 /**
  * @class PerspectiveCamera
  * @brief Rappresenta una telecamera con proiezione prospettica.
  *
  * Questa classe estende la classe base `Camera` per implementare una proiezione prospettica.
  * A differenza della camera ortografica, la camera prospettica simula la percezione visiva degli oggetti,
  * dove gli oggetti lontani appaiono pi� piccoli rispetto a quelli vicini, creando un effetto di profondit�.
  */
class LIB_API PerspectiveCamera : public Camera
{
public:
    /**
     * @brief Costruttore di default per la `PerspectiveCamera`.
     *
     * Inizializza una telecamera prospettica con parametri predefiniti per il campo visivo,
     * il piano di clipping vicino e lontano, e il rapporto d'aspetto.
     */
    PerspectiveCamera();

    virtual ~PerspectiveCamera() = default;

    /**
     * @brief Renderizza la scena utilizzando una proiezione prospettica.
     *
     * Configura una matrice di proiezione prospettica in OpenGL e applica il rendering
     * con effetto di prospettiva.
     *
     * @param viewMatrix La matrice di vista da utilizzare per il rendering.
     */
    void render(const glm::mat4 viewMatrix) const override;
};
