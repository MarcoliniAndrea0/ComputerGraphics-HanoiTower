/**
 * @file list.h
 * @brief Definizione della classe List per la gestione delle liste di rendering.
 *
 * Questo file contiene la dichiarazione della classe List, che gestisce
 * la raccolta e l'ordinamento degli oggetti da renderizzare nella scena.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include "node.h"
#include "Common.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>

 /**
  * @class List
  * @brief Gestisce una lista ordinata di nodi e le loro matrici di trasformazione per il rendering.
  *
  * La classe List è responsabile della creazione e gestione di una lista
  * "flat" (appiattita) di tutti i nodi da renderizzare nella scena,
  * insieme alle loro matrici di trasformazione globali precalcolate.
  *
  * Funzionalità principali:
  * - **Traversal del grafo**: Attraversa ricorsivamente il grafo della scena
  *   per raccogliere tutti i nodi renderizzabili
  * - **Calcolo matrici globali**: Precalcola le world matrices per ogni nodo
  *   moltiplicando le trasformazioni gerarchiche
  * - **Ordinamento per priorità**: Riordina i nodi secondo la priorità di rendering
  *   (Camere → Luci → Mesh)
  * - **Ottimizzazione**: Evita il ricalcolo delle trasformazioni durante il rendering
  *
  * Il sistema di priorità garantisce che:
  * 1. Le telecamere vengano processate per prime
  * 2. Le luci vengano configurate prima di renderizzare gli oggetti
  * 3. Le mesh vengano renderizzate per ultime
  *
  * @see Node
  * @see Camera
  * @see Light
  * @see Mesh
  */
class LIB_API List : public Object {

public:

    /**
     * @brief Costruttore di default per la classe List.
     *
     * Inizializza una lista vuota pronta per essere popolata
     * con i nodi della scena.
     */
    List() = default;

    // ========================================================================
    // GETTER METHODS
    // ========================================================================

    /**
     * @brief Restituisce la lista di nodi da renderizzare con le loro matrici globali.
     *
     * Fornisce accesso diretto alla lista interna di coppie (nodo, matrice).
     * Ogni coppia contiene:
     * - Un puntatore condiviso al nodo da renderizzare
     * - La matrice di trasformazione globale (world matrix) del nodo
     *
     * @return Riferimento modificabile al vettore di coppie (nodo, world matrix)
     *
     * @note La lista potrebbe non essere ordinata se sortListRendering()
     *       non è stato chiamato.
     */
    std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>>& getListRendering();

    // ========================================================================
    // SETTER METHODS
    // ========================================================================

    /**
     * @brief Sostituisce completamente la lista di rendering.
     *
     * Imposta una nuova lista di nodi e matrici, scartando la precedente.
     * Utile quando si vuole ricostruire completamente la lista da zero.
     *
     * @param newListRendering Nuovo vettore contenente coppie di nodi e
     *                         le loro matrici di trasformazione globali
     */
    void setListRendering(std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> newListRendering);

    // ========================================================================
    // METODI DI GESTIONE E ORDINAMENTO
    // ========================================================================

    /**
     * @brief Riordina la lista degli oggetti secondo la priorità di rendering.
     *
     * Ordina i nodi nella lista in base alla loro priorità per garantire
     * un ordine corretto di rendering. L'ordinamento segue questa gerarchia:
     *
     * 1. **Camera** (priorità più alta): Devono essere processate per prime
     *    per configurare la view matrix
     * 2. **Light** (priorità media): Devono essere configurate prima di
     *    renderizzare gli oggetti che verranno illuminati
     * 3. **Altri oggetti** come Mesh (priorità più bassa): Vengono
     *    renderizzati per ultimi usando camera e luci già configurate
     *
     * @note Questo metodo modifica l'ordine interno della lista.
     *       Deve essere chiamato dopo aver popolato la lista con pass()
     *       e prima di chiamare render().
     *
     * @see pass()
     * @see render()
     */
    void sortListRendering();

    /**
     * @brief Genera ricorsivamente una lista flat di tutti i nodi renderizzabili.
     *
     * Attraversa l'intero grafo della scena a partire dal nodo radice,
     * visitando ricorsivamente tutti i figli e calcolando le matrici di
     * trasformazione globali (world matrices) per ciascun nodo.
     *
     * Il traversal è di tipo depth-first (profondità prima) e:
     * - Raccoglie ogni nodo visitato
     * - Calcola la world matrix moltiplicando la local matrix con quella del parent
     * - Continua ricorsivamente per tutti i figli
     *
     * @param sceneRoot Puntatore condiviso al nodo radice della scena da attraversare
     * @param parentWorldMatrix Matrice di trasformazione globale del nodo padre.
     *                          Per il nodo radice, dovrebbe essere la matrice identità.
     *
     * @return Vettore di coppie (nodo, world matrix) contenente tutti i nodi
     *         renderizzabili e le loro trasformazioni globali
     *
     * Esempio di utilizzo:
     * @code
     * auto renderList = List::pass(sceneRoot, glm::mat4(1.0f));
     * @endcode
     *
     * @note Questo è un metodo statico che può essere chiamato senza
     *       un'istanza della classe List.
     *
     * @see sortListRendering()
     */
    static std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> pass(const std::shared_ptr<Node> sceneRoot, const glm::mat4 parentWorldMatrix);

    // ========================================================================
    // RENDERING
    // ========================================================================

    /**
     * @brief Renderizza tutti gli oggetti nella lista in ordine.
     *
     * Itera attraverso la lista ordinata di nodi e invoca il metodo render()
     * di ciascuno, passando la view matrix calcolata dalla camera.
     *
     * Il processo di rendering:
     * 1. Per ogni nodo nella lista (già ordinata per priorità)
     * 2. Calcola la view matrix moltiplicando inversaCamera con la world matrix
     * 3. Chiama il metodo render() del nodo con questa view matrix
     *
     * @param inversaCamera Matrice inversa della camera attiva, utilizzata per
     *                      trasformare gli oggetti dallo spazio world allo
     *                      spazio della vista (view space)
     *
     * @note Prima di chiamare questo metodo, assicurarsi che:
     *       - La lista sia stata popolata con pass()
     *       - La lista sia stata ordinata con sortListRendering()
     *       - OpenGL sia stato correttamente inizializzato
     *
     * @see pass()
     * @see sortListRendering()
     */
    void render(const glm::mat4 inversaCamera) const override;

private:

    /**
     * @brief Lista interna di coppie (nodo, world matrix) per il rendering.
     *
     * Ogni elemento della lista contiene:
     * - std::shared_ptr<Node>: Puntatore al nodo da renderizzare
     * - glm::mat4: Matrice di trasformazione globale (world matrix) precalcolata
     *
     * La lista viene popolata dal metodo pass() e ordinata da sortListRendering().
     */
    std::vector<std::pair<std::shared_ptr<Node>, glm::mat4>> _listRendering;
};
