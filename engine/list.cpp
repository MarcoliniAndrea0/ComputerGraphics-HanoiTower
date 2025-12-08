#include "list.h"
#include "node.h"
#include <algorithm>

// Getter

/**
 * @brief Restituisce una referenza alla lista degli oggetti da renderizzare.
 * @return Una referenza a `std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>>`.
 */
LIB_API std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>>& List::getListRendering() {
    return _listRendering;
}

// Setter

/**
 * @brief Imposta la lista degli oggetti da renderizzare.
 * @param newListRendering Il nuovo vettore contenente nodi e matrici di trasformazione.
 */
void LIB_API List::setListRendering(std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> newListRendering) {
    _listRendering = newListRendering;
}

/**
 * @brief Genera una lista di nodi e le loro matrici di trasformazione globale per il rendering.
 * @param sceneRoot Il nodo radice della scena.
 * @param parentWorldMatrix La matrice di trasformazione globale del nodo padre.
 * @return Un vettore di coppie contenente nodi e matrici di trasformazione globale.
 */
std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> LIB_API List::pass(const std::shared_ptr<Node> sceneRoot, const glm::mat4 parentWorldMatrix) {
    std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> renderListPass;

    // Aggiunge il nodo corrente con la sua matrice di trasformazione globale.
    renderListPass.push_back(std::make_pair(sceneRoot, parentWorldMatrix * sceneRoot->getLocalMatrix()));

    // Itera sui figli del nodo radice e costruisce ricorsivamente la lista.
    for (const auto& child : sceneRoot->getChildren()) {
        auto childRenderList = List::pass(child, parentWorldMatrix * sceneRoot->getLocalMatrix());
        renderListPass.insert(renderListPass.end(), childRenderList.begin(), childRenderList.end());
    }

    return renderListPass;
}

/**
 * @brief Riordina la lista degli oggetti da renderizzare in base alla priorit�.
 */
void List::sortListRendering() {
    std::sort(_listRendering.begin(), _listRendering.end(),
        [](const std::pair<std::shared_ptr<Node>, glm::mat4> a, const std::pair<std::shared_ptr<Node>, glm::mat4> b) {
            return a.first->getPriority() > b.first->getPriority();
        });
}

/**
 * @brief Renderizza tutti gli oggetti nella lista di rendering.
 * @param inversaCamera La matrice inversa della camera.
 */
void LIB_API List::render(const glm::mat4 inversaCamera) const {
    glm::mat4 viewMatrix(1.0f);

    // Per ogni elemento della lista, calcola la matrice di vista e renderizza il nodo.
    for (const auto& node : _listRendering) {
        viewMatrix = inversaCamera * node.second;
        node.first->render(viewMatrix);
    }
}
