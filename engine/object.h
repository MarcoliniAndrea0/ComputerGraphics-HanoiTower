#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Common.h"

/**
 * @class Object
 * @brief Rappresenta un oggetto di base all'interno del sistema.
 *
 * La classe Object fornisce una base comune a tutti gli oggetti nel sistema.
 * Ogni oggetto ha un identificatore univoco, un tipo e un nome associato.
 * La classe serve come base per altre classi.
 */
class LIB_API Object
{
public:
    /**
     * @brief Costruttore di default.
     */
    Object() = default;

    /**
     * @brief Costruttore parametrico.
     * @param type Il tipo dell'oggetto.
     */
    Object(const std::string type);

    /**
     * @brief Distruttore della classe `Object`.
     *
     * Rimuove la luce decrementando il contatore globale degli ID degli oggetti.
     */
    virtual ~Object();

    // Getter

    /**
     * @brief Restituisce l'ID dell'oggetto.
     * @return L'ID univoco dell'oggetto.
     */
    int getId() const;

    /**
     * @brief Restituisce il nome dell'oggetto.
     * @return Il nome dell'oggetto.
     */
    std::string getName() const;

    /**
     * @brief Restituisce il tipo dell'oggetto.
     * @return Il tipo dell'oggetto.
     */
    const std::string getType() const;

    // Setter

    /**
     * @brief Imposta il nome dell'oggetto.
     * @param newName Il nuovo nome dell'oggetto.
     */
    void setName(const std::string newName);

    /**
     * @brief Imposta il tipo dell'oggetto.
     * @param type Il nuovo tipo dell'oggetto.
     */
    void setType(const std::string& type);

    /**
     * @brief Metodo virtuale puro per il rendering.
     * @param viewMatrix La matrice di visualizzazione (camera inversa moltiplicata per la matrice del mondo).
     */
    virtual void render(const glm::mat4 viewMatrix) const = 0;

    /**
     * @brief Resetta il generatore di ID a zero.
     */
    static void resetIdGenerator();

private:
    int _id;  ///< Identificatore univoco dell'oggetto.
    std::string _name; ///< Nome dell'oggetto.
    std::string _type; ///< Tipo dell'oggetto.

    static int nextId; ///< Prossimo ID da assegnare agli oggetti.
};
