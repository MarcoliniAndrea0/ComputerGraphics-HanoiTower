#include "object.h"

#include <sstream>

int Object::nextId = 0;

/**
 * @brief Costruttore della classe Object.
 * @param type Il tipo dell'oggetto.
 *
 * Il costruttore assegna automaticamente un ID univoco all'oggetto e imposta
 * il nome dell'oggetto nel formato "[<id>]".
 */
Object::Object(const std::string type)
    : _type(type)
{
    this->_id = Object::nextId++;

    std::stringstream stream;
    stream << '[' << this->getId() << ']';
    this->_name = stream.str();
}

/**
 * @brief Distruttore della classe `Object`.
 */
Object::~Object()
{
    nextId--;
}


///// Getter

/**
 * @brief Restituisce l'ID univoco assegnato a questo oggetto.
 *
 * Questo metodo restituisce l'ID che viene auto-generato per l'oggetto
 * al momento della sua creazione. L'ID identifica
 * in modo univoco ogni istanza della classe `Object`.
 *
 * @return L'ID dell'oggetto.
 */
int LIB_API Object::getId() const
{
    return this->_id;
}

/**
 * @brief Restituisce il nome di questo oggetto.
 *
 * @return Il nome dell'oggetto come stringa.
 */
std::string LIB_API Object::getName() const
{
    return this->_name;
}

/**
 * @brief Restituisce il tipo di questo oggetto.
 *
 * @return Il tipo dell'oggetto come stringa.
 */
const std::string LIB_API Object::getType() const {
    return _type;
}

////// Setter

/**
 * @brief Imposta un nuovo nome per questo oggetto.
 *
 * @param newName Il nuovo nome da assegnare all'oggetto.
 */
void LIB_API Object::setName(const std::string newName)
{
    this->_name = newName;
}

/**
 * @brief Imposta un nuovo tipo per questo oggetto.
 *
 * @param type Il nuovo tipo da assegnare all'oggetto.
 */
void LIB_API Object::setType(const std::string& type) {
    this->_type = type;
}

///// Other

/**
 * @brief Resetta il generatore di ID a zero.
 *
 * Questo metodo ripristina il contatore degli ID a zero.
 */
void LIB_API Object::resetIdGenerator() {
    nextId = 0;
}
