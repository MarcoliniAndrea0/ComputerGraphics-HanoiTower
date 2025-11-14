/**
 * @file object.h
 * @brief Dichiarazione della classe base Object che rappresenta un elemento generico della scena.
 *
 * Questa classe fornisce generazione automatica di ID univoci, gestione di nome e tipo,
 * e un metodo virtuale puro render da implementare nelle classi derivate.
 */
#pragma one

#include <string>

 /**
  * @class Object
  * @brief Classe base per tutti gli oggetti nel grafo della scena.
  *
  * Funzionalità fornite:
  * - Identificatore univoco per ogni istanza
  * - Proprietà di nome e tipo
  * - Funzione render (virtuale pura) da implementare nelle sottoclassi
  *
  * La classe gestisce l'assegnazione automatica dell'ID tramite un contatore statico privato.
  */
class Object
{
public:
    /**
     * @brief Costruttore dell'Object con nome e tipo opzionali.
     * @param name Nome descrittivo (di default stringa vuota).
     * @param type Identificatore del tipo (di default stringa vuota).
     */
    Object(const std::string& name = "", const std::string& type = "");
    
    /**
     * @brief Distruttore virtuale.
     */
    virtual ~Object() = default;

    /**
     * @brief Metodo virtuale puro per renderizzare l'oggetto.
     * Deve essere implementato dalle classi derivate.
     */
    virtual void render() = 0;

        ////////////////
        ///  GETTER  ///
        ////////////////

    /**
     * @brief Restituisce l'ID univoco di questa istanza di Object.
     * @return Identificatore intero univoco.
     */
    int getID() const;
    
    /**
     * @brief Restituisce il nome dell'oggetto.
     * @return Riferimento alla stringa del nome.
     */
    const std::string& getName() const;
    
    /**
     * @brief Restituisce il tipo dell'oggetto.
     * @return Riferimento alla stringa del tipo.
     */
    const std::string& getType() const;


        ////////////////
        ///  SETTER  ///
        ////////////////

    /**
     * @brief Imposta un nuovo nome per l'oggetto.
     * @param newName Stringa da assegnare come nuovo nome.
     */
    void setName(const std::string& newName);
    
    /**
     * @brief Imposta l'identificatore di tipo per l'oggetto.
     * @param newType Stringa che definisce il nuovo tipo.
     */
    void setType(const std::string& newType);

protected:
    int id;             ///< ID univoco per questa istanza
    std::string name;   ///< Nome dell'oggetto
    std::string type;   ///< Tipo identificatore dell'oggetto

private:
    static int id_counter;  ///< Contatore statico per assegnare ID univoci
};
