/**
 * @file object.cpp
 * @brief Implementazione della classe base Object.
 *
 * Inizializza il contatore statico degli ID e implementa i metodi accessori
 * per nome, tipo e ID. Il costruttore inizializza l'ID e le variabili membro.
 * Il metodo render è virtuale puro e non è implementato qui.
 */
#include "object.h"

int Object::id_counter = 0;

/**
 * @brief Costruisce un oggetto Object con nome e tipo opzionali.
 * Assegna automaticamente un ID univoco.
 * @param name Nome descrittivo (di default stringa vuota).
 * @param type Identificatore del tipo (di default stringa vuota).
 */
Object::Object(const std::string& name_, const std::string& type_)
	: id(++id_counter), name(name_), type(type_)
{
}

	////////////////
	///  GETTER  ///
	////////////////

/**
 * @brief Restituisce l'ID univoco di questa istanza di Object.
 * @return Identificatore intero univoco.
 */
int Object::getID() const 
{
	return id;
}

/**
 * @brief Restituisce il nome dell'oggetto.
 * @return Riferimento alla stringa del nome.
 */
const std::string& Object::getName() const 
{
	return name;
}

/**
 * @brief Restituisce il tipo dell'oggetto.
 * @return Riferimento alla stringa del tipo.
 */
const std::string& Object::getType() const 
{ 
	return type; 
}

	////////////////
	///  SETTER  ///
	////////////////


/**
 * @brief Imposta un nuovo nome per l'oggetto.
 * @param newName Stringa da assegnare come nuovo nome.
 */
void Object::setName(const std::string& newName) {
	name = newName;
}

/**
 * @brief Imposta il tipo identificatore per l'oggetto.
 * @param newType Stringa che definisce il nuovo tipo.
 */
void Object::setType(const std::string& newType) 
{ 
	type = newType; 
}