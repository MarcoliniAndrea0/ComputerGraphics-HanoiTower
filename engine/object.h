/**
 * @file object.h
 * @brief Definizione della classe base astratta Object per tutte le entità del motore.
 *
 * Questo file contiene la dichiarazione della classe Object, che funge da classe
 * base per tutti gli oggetti gestiti dal motore grafico. Fornisce funzionalità
 * comuni come identificazione univoca, naming e interfaccia di rendering.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Common.h"

 /**
  * @class Object
  * @brief Classe base astratta per tutte le entità del motore grafico.
  *
  * La classe Object definisce l'interfaccia comune e le proprietà di base
  * per tutti gli oggetti gestiti dal sistema. È una classe astratta che non può
  * essere istanziata direttamente, ma deve essere derivata.
  *
  * Caratteristiche principali:
  * - **Identificazione univoca**: Ogni oggetto riceve automaticamente un ID unico
  *   generato tramite un contatore statico
  * - **Naming**: Ogni oggetto può avere un nome leggibile per facilitare il debugging
  *   e la ricerca nella scena
  * - **Tipizzazione**: Un campo tipo permette di identificare la categoria dell'oggetto
  * - **Interfaccia di rendering**: Definisce il metodo virtuale puro render() che
  *   tutte le classi derivate devono implementare
  *
  * La classe gestisce automaticamente la generazione di ID univoci attraverso
  * un contatore statico incrementale, garantendo che ogni istanza abbia un
  * identificatore unico all'interno della sessione.
  *
  * @see Node
  * @see Mesh
  * @see Light
  * @see Camera
  * @see Material
  * @see Texture
  *
  * @author Gruppo6
  * @version 1.0
  */
class LIB_API Object
{
public:
    // ========================================================================
    // COSTRUTTORI E DISTRUTTORE
    // ========================================================================

    /**
     * @brief Costruttore di default.
     *
     * Inizializza un nuovo oggetto con:
     * - ID univoco generato automaticamente dal contatore statico
     * - Nome vuoto
     * - Tipo vuoto
     *
     * @note L'ID viene assegnato automaticamente incrementando il contatore statico nextId.
     */
    Object() = default;

    /**
     * @brief Costruttore parametrico con tipo.
     *
     * Inizializza un nuovo oggetto specificando il suo tipo.
     * L'ID viene assegnato automaticamente.
     *
     * @param type Stringa che rappresenta il tipo/categoria dell'oggetto
     *             (es. "MESH", "LIGHT", "CAMERA", "MATERIAL")
     *
     * Esempio di utilizzo:
     * @code
     * class Mesh : public Object {
     *     Mesh() : Object("MESH") { }
     * };
     * @endcode
     */
    Object(const std::string type);

    /**
     * @brief Distruttore virtuale.
     *
     * Garantisce la corretta distruzione degli oggetti derivati quando vengono
     * eliminati attraverso un puntatore alla classe base.
     *
     * @note Il distruttore è virtuale per permettere il polimorfismo corretto
     *       e assicurare che i distruttori delle classi derivate vengano chiamati.
     */
    virtual ~Object();

    // ========================================================================
    // GETTERS
    // ========================================================================

    /**
     * @brief Restituisce l'identificativo univoco dell'oggetto.
     *
     * L'ID è un numero intero generato automaticamente alla creazione dell'oggetto
     * e rimane costante per tutta la sua vita. Gli ID sono sequenziali e univoci
     * all'interno di una sessione del programma.
     *
     * @return L'ID intero univoco dell'oggetto
     *
     * @note Gli ID iniziano da 0 e vengono incrementati per ogni nuovo oggetto creato.
     */
    int getId() const;

    /**
     * @brief Restituisce il nome assegnato all'oggetto.
     *
     * Il nome è una stringa leggibile dall'utente che può essere utilizzata per:
     * - Identificare l'oggetto durante il debugging
     * - Cercare oggetti specifici nel grafo della scena
     * - Visualizzare informazioni all'utente
     *
     * @return Stringa contenente il nome dell'oggetto (può essere vuota)
     *
     * @see setName()
     */
    std::string getName() const;

    /**
     * @brief Restituisce il tipo dell'oggetto.
     *
     * Il tipo identifica la categoria o classe dell'oggetto (es. "MESH", "LIGHT").
     * Utile per il casting sicuro, il filtering e l'organizzazione degli oggetti.
     *
     * @return Stringa costante che identifica la categoria dell'oggetto
     *
     * @see setType()
     */
    const std::string getType() const;

    // ========================================================================
    // SETTERS
    // ========================================================================

    /**
     * @brief Assegna un nome leggibile all'oggetto.
     *
     * Il nome è utile per identificare l'oggetto nel grafo della scena,
     * durante il debugging o per implementare sistemi di ricerca.
     *
     * @param newName Il nuovo nome da assegnare all'oggetto
     *
     * Esempio di utilizzo:
     * @code
     * auto mesh = std::make_shared<Mesh>();
     * mesh->setName("PlayerCharacter");
     * @endcode
     *
     * @note Il nome non deve essere univoco - più oggetti possono avere lo stesso nome.
     */
    void setName(const std::string newName);

    /**
     * @brief Definisce il tipo/categoria dell'oggetto.
     *
     * Imposta una stringa che identifica la categoria dell'oggetto.
     * Normalmente viene chiamato dal costruttore delle classi derivate.
     *
     * @param type Il nuovo tipo da assegnare (es. "MESH", "POINT_LIGHT", "CAMERA")
     *
     * @note Convenzionalmente, i tipi sono espressi in MAIUSCOLO per convenzione,
     *       ma non è un requisito tecnico.
     */
    void setType(const std::string& type);

    // ========================================================================
    // METODI VIRTUALI
    // ========================================================================

    /**
     * @brief Metodo virtuale puro per il rendering dell'oggetto.
     *
     * Ogni classe derivata DEVE implementare questo metodo per definire
     * come l'oggetto viene disegnato sullo schermo. Questo è il cuore
     * del sistema di rendering polimorfic del motore.
     *
     * Il metodo riceve la view matrix corrente, che è necessaria per:
     * - Trasformare gli oggetti dallo spazio world allo spazio della vista
     * - Calcolare l'illuminazione
     * - Applicare le trasformazioni della camera
     *
     * @param viewMatrix La matrice di vista (View Matrix) corrente della camera,
     *                   che trasforma le coordinate dallo spazio world allo
     *                   spazio della camera
     *
     * Esempio di implementazione:
     * @code
     * void Mesh::render(const glm::mat4 viewMatrix) const override {
     *     // Applica materiale
     *     material->render(viewMatrix);
     *
     *     // Disegna la geometria
     *     glBegin(GL_TRIANGLES);
     *     // ... rendering code ...
     *     glEnd();
     * }
     * @endcode
     *
     * @note Questo metodo deve essere dichiarato const perché il rendering
     *       non dovrebbe modificare lo stato logico dell'oggetto.
     */
    virtual void render(const glm::mat4 viewMatrix) const = 0;

    // ========================================================================
    // METODI STATICI
    // ========================================================================

    /**
     * @brief Resetta il contatore statico degli ID a zero.
     *
     * Riporta il generatore di ID al suo stato iniziale. Questo significa
     * che i prossimi oggetti creati riceveranno ID a partire da 0.
     *
     * @warning Da utilizzare con estrema cautela! Resettare il contatore
     *          mentre esistono oggetti con ID già assegnati può causare
     *          conflitti di ID e comportamenti indefiniti.
     *
     * Casi d'uso appropriati:
     * - All'inizio del programma per inizializzazione pulita
     * - Dopo aver svuotato completamente la scena e distrutto tutti gli oggetti
     * - Durante il caricamento di un nuovo livello quando si vuole ripartire da zero
     *
     * @code
     * // Scenario sicuro:
     * scene->removeAllObjects();  // Rimuove tutti gli oggetti
     * scene.reset();              // Distrugge la scena
     * Object::resetIdGenerator(); // Ora è sicuro resettare
     * scene = std::make_shared<Node>(); // Nuova scena pulita
     * @endcode
     */
    static void resetIdGenerator();

private:
    int _id;            ///< Identificatore numerico univoco dell'oggetto
    std::string _name;  ///< Nome leggibile assegnato all'oggetto (opzionale)
    std::string _type;  ///< Categoria o tipo dell'oggetto (es. "MESH", "LIGHT")

    /**
     * @brief Contatore statico per generare ID univoci.
     *
     * Questo contatore viene incrementato ogni volta che un nuovo oggetto
     * viene creato, garantendo che ogni oggetto riceva un ID unico.
     */
    static int nextId;
};
