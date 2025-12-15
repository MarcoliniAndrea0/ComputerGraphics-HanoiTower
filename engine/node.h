/**
 * @file node.h
 * @brief Definizione della classe Node per la gestione del grafo della scena.
 *
 * Questo file contiene la dichiarazione della classe Node, che rappresenta
 * un nodo generico nella gerarchia della scena 3D. I nodi supportano
 * trasformazioni, relazioni parent-child e rendering.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include <memory>
#include "object.h"
#include "Common.h"

/**
* @class Node
* @brief Rappresenta un nodo nella gerarchia di una scena 3D (Scene Graph).
*
* La classe Node è l'elemento fondamentale del grafo della scena e fornisce
* la struttura gerarchica per organizzare gli oggetti 3D. Ogni nodo può avere:
* - Trasformazioni locali (posizione, rotazione, scala)
* - Un nodo padre (parent)
* - Multipli nodi figli (children)
* - Una priorità di rendering
*
* Le trasformazioni sono gestite tramite:
* - Una matrice di base (_baseMatrix)
* - Componenti individuali di trasformazione (posizione, rotazione, scala)
*
* Il sistema supporta trasformazioni gerarchiche: le trasformazioni di un nodo
* vengono moltiplicate con quelle dei suoi antenati per ottenere la posizione
* finale nello spazio world.
*
* @see Object
* @see Mesh
* @see Camera
* @see Light
*/
class LIB_API Node : public Object, public std::enable_shared_from_this<Node>
{
public:
    /**
     * @brief Costruttore di default.
     *
     * Inizializza un nodo con:
     * - Posizione: (0, 0, 0)
     * - Rotazione: (0, 0, 0)
     * - Scala: (1, 1, 1)
     * - Matrice di base: identità
     * - Priorità: 0
     * - Nessun parent o children
     */
    Node();

    /**
     * @brief Costruttore parametrico con tipo.
     *
     * Inizializza un nodo con un tipo specifico e le stesse proprietà
     * di default del costruttore senza parametri.
     *
     * @param type Stringa che identifica il tipo di nodo (es. "Mesh", "Light", "Camera")
     */
    Node(std::string type);

    /**
     * @brief Distruttore virtuale di default.
     *
     * Permette la corretta distruzione delle classi derivate.
     */
    virtual ~Node() = default;

    // ========================================================================
    // GETTER METHODS - TRASFORMAZIONI
    // ========================================================================

    /**
     * @brief Restituisce la posizione locale del nodo.
     *
     * La posizione è relativa al sistema di coordinate del nodo padre.
     * Se il nodo non ha un padre, la posizione è nello spazio world.
     *
     * @return Vettore 3D contenente le coordinate (x, y, z) della posizione locale
     */
    glm::vec3 getPosition() const;

    /**
     * @brief Restituisce la rotazione locale del nodo.
     *
     * La rotazione è espressa come angoli di Eulero (in radianti) applicati
     * nell'ordine X-Y-Z e è relativa al sistema di coordinate del padre.
     *
     * @return Vettore 3D contenente gli angoli di rotazione (pitch, yaw, roll) in radianti
     */
    glm::vec3 getRotation() const;

    /**
     * @brief Restituisce la scala locale del nodo.
     *
     * La scala è applicata lungo gli assi locali del nodo prima delle rotazioni
     * e delle traslazioni. Valori di 1.0 rappresentano la dimensione originale.
     *
     * @return Vettore 3D contenente i fattori di scala (scaleX, scaleY, scaleZ)
     */
    glm::vec3 getScale() const;

    /**
     * @brief Restituisce la priorità di rendering del nodo.
     *
     * La priorità determina l'ordine in cui i nodi vengono processati durante
     * il rendering. Valori più alti indicano priorità maggiore.
     * Utilizzato per controllare l'ordine di rendering (es. telecamere prima degli oggetti).
     *
     * @return Il valore intero della priorità
     */
    int getPriority() const;

    /**
     * @brief Calcola e restituisce la matrice di trasformazione locale del nodo.
     *
     * La matrice locale combina la matrice di base con le trasformazioni
     * di scala, rotazione e posizione del nodo. Questa matrice trasforma
     * le coordinate dallo spazio del modello allo spazio del parent.
     *
     * @return Matrice 4x4 di trasformazione locale
     */
    glm::mat4 getLocalMatrix() const;

    /**
     * @brief Alias di getLocalMatrix().
     *
     * Restituisce la matrice di trasformazione locale del nodo.
     *
     * @return Matrice 4x4 di trasformazione locale
     * @see getLocalMatrix()
     */
    glm::mat4 getTransform() const;

    /**
     * @brief Calcola la matrice di trasformazione globale (world matrix) del nodo.
     *
     * Combina la matrice di trasformazione locale con quella del parent
     * (se presente) per ottenere la trasformazione finale nello spazio world.
     *
     * @param parentMatrix La matrice di trasformazione del nodo padre.
     *                     Se non specificata, viene usata la matrice identità.
     * @return Matrice 4x4 che trasforma dallo spazio del modello allo spazio world
     *
     * @note Questa funzione è ricorsiva: risale la gerarchia dei parent
     *       moltiplicando le matrici fino alla radice.
     */
    glm::mat4 getWorldMatrix(const glm::mat4& parentMatrix = glm::mat4(1.0f)) const;

    // ========================================================================
    // GETTER METHODS - GERARCHIA
    // ========================================================================

    /**
     * @brief Restituisce un puntatore al nodo padre.
     *
     * @return Shared pointer al nodo padre, o nullptr se questo è un nodo radice
     *
     * @note Viene restituito un weak_ptr per evitare cicli di riferimenti circolari.
     */
    std::shared_ptr<Node> getParent() const;

    /**
     * @brief Restituisce la lista dei nodi figli (versione const).
     *
     * @return Vettore contenente shared pointers a tutti i nodi figli
     */
    std::vector<std::shared_ptr<Node>> getChildren() const;

    /**
     * @brief Restituisce un riferimento modificabile alla lista dei nodi figli.
     *
     * @return Riferimento al vettore contenente shared pointers ai nodi figli
     *
     * @warning Modificare direttamente questo vettore può causare inconsistenze
     *          nella gerarchia. Preferire l'uso di addChild() e removeAllChildren().
     */
    std::vector<std::shared_ptr<Node>>& getChildren();

    // ========================================================================
    // SETTER METHODS - TRASFORMAZIONI
    // ========================================================================

    /**
     * @brief Imposta la posizione locale del nodo.
     *
     * Modifica la posizione del nodo rispetto al sistema di coordinate del padre.
     *
     * @param newPosition Nuovo vettore di posizione (x, y, z)
     */
    void setPosition(const glm::vec3 newPosition);

    /**
     * @brief Imposta la rotazione locale del nodo.
     *
     * Modifica la rotazione del nodo usando angoli di Eulero (in radianti).
     * Gli angoli vengono applicati nell'ordine X-Y-Z.
     *
     * @param newRotation Nuovo vettore di rotazione (pitch, yaw, roll) in radianti
     */
    void setRotation(const glm::vec3 newPosition);

    /**
     * @brief Imposta la scala locale del nodo.
     *
     * Modifica i fattori di scala lungo gli assi locali del nodo.
     *
     * @param newScale Nuovo vettore di scala (scaleX, scaleY, scaleZ)
     *
     * @note Valori di 0.0 o negativi possono causare comportamenti indefiniti.
     */
    void setScale(const glm::vec3 newPosition);

    /**
     * @brief Imposta la matrice di trasformazione di base del nodo.
     *
     * La matrice di base fornisce una trasformazione aggiuntiva che viene
     * applicata prima delle trasformazioni standard (scala, rotazione, posizione).
     * Utile per applicare trasformazioni complesse o precalcolate.
     *
     * @param newBaseMatrix Nuova matrice di base 4x4
     */
    void setBaseMatrix(const glm::mat4 newBaseMatrix);

    /**
     * @brief Imposta la priorità di rendering del nodo.
     *
     * La priorità determina l'ordine di processing durante il rendering.
     * Valori più alti = priorità maggiore.
     *
     * @param p Nuovo valore di priorità
     */
    void setPriority(int p);

    // ========================================================================
    // METODI DI GESTIONE GERARCHIA
    // ========================================================================

    /**
     * @brief Aggiunge un nodo figlio a questo nodo.
     *
     * Stabilisce una relazione parent-child tra questo nodo e il figlio.
     * Il nodo figlio erediterà le trasformazioni di questo nodo.
     *
     * @param newChild Shared pointer al nodo da aggiungere come figlio
     *
     * @note Se il figlio aveva già un parent, la relazione precedente
     *       viene mantenuta (il nodo può avere un solo parent).
     */
    void addChild(const std::shared_ptr<Node> newChild);

    /**
     * @brief Rimuove tutti i nodi figli da questo nodo.
     *
     * Elimina tutte le relazioni parent-child, ma non distrugge i nodi figli
     * se esistono altri riferimenti ad essi.
     */
    void removeAllChildren();

    // ========================================================================
    // RENDERING
    // ========================================================================

    /**
     * @brief Renderizza il nodo e tutti i suoi figli.
     *
     * Questo metodo è virtuale e può essere sovrascritto dalle classi derivate
     * per implementare comportamenti di rendering specifici.
     * La versione di base della classe Node richiama ricorsivamente il rendering
     * di tutti i nodi figli.
     *
     * @param viewMatrix La matrice di vista della camera corrente, utilizzata
     *                   per trasformare gli oggetti nello spazio della vista
     */
    void render(const glm::mat4 viewMatrix) const override;

private:
    int priority;                                   ///< Priorità del nodo per l'ordinamento del rendering
    std::vector<std::shared_ptr<Node>> children;   ///< Vettore di puntatori ai nodi figli
    glm::mat4 _baseMatrix;                          ///< Matrice di trasformazione di base (applicata prima delle altre)
    glm::vec3 _position;                            ///< Posizione locale del nodo (x, y, z)
    glm::vec3 _rotation;                            ///< Rotazione locale del nodo come angoli di Eulero (pitch, yaw, roll)
    glm::vec3 _scale;                               ///< Scala locale del nodo (scaleX, scaleY, scaleZ)
    std::weak_ptr<Node> parent;                     ///< Weak pointer al nodo genitore (evita riferimenti circolari)
};
