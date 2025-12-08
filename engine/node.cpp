#include "node.h"

#include "GL/freeglut.h"
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Costruttore della classe Node.
 *
 * Il costruttore inizializza un oggetto `Node` impostando un ID univoco e un nome.
 * Inoltre, configura la matrice di base, la posizione,
 * la rotazione e lo scale dell'oggetto:
 *
 * - La matrice di base (`baseMatrix`) viene impostata come matrice di identity.
 * - La posizione (`position`) viene impostata a (0.0f, 0.0f, 0.0f).
 * - La rotazione (`rotation`) viene impostata a (0.0f, 0.0f, 0.0f).
 * - La scala (`scale`) viene impostata a (1.0f, 1.0f, 1.0f), -> normale
 */
Node::Node()
    :Object("Node")
{
    this->setBaseMatrix(glm::mat4(1.0f));
    this->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    this->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    this->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    this->setPriority(0);
}

/**
 * @brief Costruttore della classe Node con tipo personalizzato.
 *
 * Il costruttore inizializza un oggetto `Node` impostando un ID univoco e un nome.
 * Gli specifici un tipo e imposta:
 *
 * - La matrice di base (`baseMatrix`) viene impostata come matrice di identity.
 * - La posizione (`position`) viene impostata a (0.0f, 0.0f, 0.0f).
 * - La rotazione (`rotation`) viene impostata a (0.0f, 0.0f, 0.0f).
 * - La scala (`scale`) viene impostata a (1.0f, 1.0f, 1.0f), -> normale
 *
 * @param type Il tipo dell'oggetto `Node`. Questo valore viene passato al costruttore della classe base `Object`.
 */
Node::Node(std::string type)
    : Object(type)
{
    this->setBaseMatrix(glm::mat4(1.0f));
    this->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    this->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    this->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    this->setPriority(0);
}


///// Getter

/**
 * @brief Restituisce l'offset di posizione del nodo.
 *
 * Questo metodo restituisce il valore dell'offset di posizione del nodo,
 * che rappresenta la posizione del nodo rispetto alla matrice di base.
 *
 * @return Il valore dell'offset di posizione come vettore `glm::vec3`.
 */
glm::vec3 LIB_API Node::getPosition() const
{
    return this->_position;
}

/**
 * @brief Restituisce l'offset di rotazione del nodo.
 *
 * Questo metodo restituisce il valore dell'offset di rotazione del nodo,
 * che rappresenta la rotazione del nodo rispetto alla matrice di base.
 *
 * @return Il valore dell'offset di rotazione come vettore `glm::vec3` (in radianti).
 */
glm::vec3 LIB_API Node::getRotation() const
{
    return this->_rotation;
}

/**
 * @brief Restituisce l'offset di scala del nodo.
 *
 * Questo metodo restituisce il valore dell'offset di scala del nodo,
 * che rappresenta la scala del nodo rispetto alla matrice di base.
 *
 * @return Il valore dell'offset di scala come vettore `glm::vec3`.
 */
glm::vec3 LIB_API Node::getScale() const
{
    return this->_scale;
}

/**
 * @brief Restituisce la matrice del modello locale per questo nodo.
 *
 * La matrice del modello locale viene calcolata combinando la matrice di base con
 * traslazione, rotazione e scalatura applicate al nodo. Questo metodo viene utilizzato
 * per determinare la posizione finale e l'orientamento del nodo nello
 * spazio 3D.
 *
 * La localMatrix viene calcolata nel seguente ordine:
 *
 *  1. Base matrix
 *  2. Scale matrix
 *  3. X rotation matrix
 *  4. Y rotation matrix
 *  5. Z rotation matrix
 *  6. Translation matrix
 *
 * La matrice di trasformazione finale viene ottenuta combinando la matrice di traslazione,
 * la matrice di rotazione e la matrice di scalatura, e poi moltiplicando il risultato
 * con la matrice di base.
 *
 * @return La matrice locale per questo nodo come `glm::mat4`.
 */
glm::mat4 LIB_API Node::getLocalMatrix() const
{
    // Crea una matrice di traslazione basata sulla posizione del nodo
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), this->_position);

    const glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), glm::radians(this->_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(this->_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(this->_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), this->_scale);

    // La scalatura viene applicata prima, seguita dalla rotazione e infine dalla traslazione
    const glm::mat4 offsetMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    // Moltiplica la matrice di base con la matrice di trasformazione calcolata
    return offsetMatrix * this->_baseMatrix;
}

/**
 * @brief Restituisce una copia del vettore contenente i puntatori agli oggetti figli.
 *
 * @return Una copia del vettore di puntatori condivisi (`std::shared_ptr`) agli oggetti figli.
 */
std::vector<std::shared_ptr<Node>> LIB_API Node::getChildren() const
{
    return this->children;
}

std::vector<std::shared_ptr<Node>>& Node::getChildren()
{
    return this->children; // Restituisce una referenza modificabile
}


/**
 * @brief Restituisce la priorita' del nodo.
 *
 * @return Il valore della priorita del nodo.
 */
int LIB_API Node::getPriority() const {
    return priority;
}

///// Setter

/**
 * @brief Imposta l'offset di scala del nodo.
 *
 * @param newScale Il nuovo offset di scala come vettore `glm::vec3`.
 */
void LIB_API Node::setScale(const glm::vec3 newScale)
{
    this->_scale = newScale;
}

/**
 * @brief Imposta la matrice di base per questo nodo.
 *
 * @param newBaseMatrix La nuova matrice di base come `glm::mat4`.
 */
void LIB_API Node::setBaseMatrix(const glm::mat4 newBaseMatrix)
{
    this->_baseMatrix = newBaseMatrix;
}

/**
 * @brief Imposta l'offset di posizione del nodo.
 *
 * @param newPosition Il nuovo offset di posizione come vettore `glm::vec3`.
 */
void LIB_API Node::setPosition(const glm::vec3 newPosition)
{
    this->_position = newPosition;
}

/**
 * @brief Imposta l'offset di rotazione del nodo.
 *
 * @param newRotation Il nuovo offset di rotazione come vettore `glm::vec3`.
 */
void LIB_API Node::setRotation(const glm::vec3 newRotation)
{
    this->_rotation = newRotation;
}

/**
 * @brief Imposta la priorita' del nodo.
 *
 * @param p Il nuovo valore di priorita' come intero.
 */
void LIB_API Node::setPriority(int p) {
    priority = p;
}

///// Other

/**
 * @brief Aggiunge un oggetto come figlio di questo nodo.
 *
 * Questo metodo aggiunge un puntatore condiviso (`std::shared_ptr`) a un oggetto nodo
 * come figlio del nodo corrente.
 *
 * @param newChild Il puntatore condiviso al nuovo nodo figlio da aggiungere.
 */
void LIB_API Node::addChild(const std::shared_ptr<Node> newChild)
{
    if (newChild) {
        newChild->parent = shared_from_this(); // Imposta il genitore del figlio
        this->children.push_back(newChild);
    }
}

void LIB_API Node::removeAllChildren()
{
    this->children.clear();
    std::cout << "All children nodes have been removed from: " << this->getName() << std::endl;
}

///// funzione di render per un Node

/**
 * @brief Esegue il rendering del nodo.
 *
 * Questa funzione viene chiamata automaticamente da MyEngine non bisogna chiamarla manualmente.
 *
 * @param viewMatrix La matrice di visualizzazione da utilizzare per il rendering di questo nodo.
 */
void LIB_API Node::render(const glm::mat4 viewMatrix) const
{
    // Imposta la modalita' della matrice su GL_MODELVIEW
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(viewMatrix));
}

glm::mat4 Node::getWorldMatrix(const glm::mat4& parentMatrix) const {
    return parentMatrix * getLocalMatrix();
}

glm::mat4 Node::getTransform() const {
    return getLocalMatrix();
}

std::shared_ptr<Node> Node::getParent() const {
    return this->parent.lock();
}
