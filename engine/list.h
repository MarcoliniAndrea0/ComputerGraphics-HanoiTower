#pragma once

#include "Node.h"
#include "Common.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

/**
 * @class List
 * @brief Gestisce una lista di nodi e le loro matrici di trasformazione per il rendering.
 *
 * La classe `List` mantiene una lista di nodi e le relative matrici di trasformazione
 * che sono utilizzate per il rendering. La lista viene utilizzata per ordinare e
 * gestire il rendering degli oggetti nella scena.
 */
class LIB_API List : public Object {

public:

    /**
     * @brief Costruttore di default per la classe `List`.
     */
    List() = default;

    // Getter

    /**
     * @brief Restituisce la lista di nodi da renderizzare.
     * @return Una referenza a `std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>>` contenente
     *         i nodi e le loro matrici di trasformazione globale.
     */
    std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>>& getListRendering();

    // Setter

    /**
     * @brief Imposta la lista di nodi da renderizzare.
     * @param newListRendering Il nuovo vettore contenente nodi e le loro matrici di trasformazione.
     */
    void setListRendering(std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> newListRendering);

    /**
     * @brief Riordina la lista degli oggetti da renderizzare in base alla priorità.
     *
     * L'ordine di importanza è:
     * - Camera
     * - Light
     * - Resto (es. Mesh).
     */
    void sortListRendering();

    /**
     * @brief Genera una lista di nodi e le relative matrici di trasformazione globale per il rendering.
     * @param sceneRoot Il nodo radice della scena.
     * @param parentWorldMatrix La matrice di trasformazione globale del nodo padre.
     * @return Un vettore di coppie contenente nodi e matrici di trasformazione globale.
     */
    static std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> pass(const std::shared_ptr<Node> sceneRoot, const glm::mat4 parentWorldMatrix);

    /**
     * @brief Renderizza tutti gli oggetti nella lista.
     * @param inversaCamera La matrice inversa della camera, necessaria per calcolare la matrice di vista.
     */
    void render(const glm::mat4 inversaCamera) const override;

private:

    ///< Lista dei nodi e delle loro matrici di trasformazione per il rendering.
    std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> _listRendering;
};
