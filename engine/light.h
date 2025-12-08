#pragma once

#include "node.h"
#include "Common.h"

/**
 * @class Light
 * @brief Funge da base per l'implementazione delle luci nella scena.
 *
 * La classe `Light` rappresenta una luce generica all'interno della scena e serve come classe
 * base per derivare altri tipi specifici di luci, come luci puntiformi, direzionali o spot.
 * Include attributi per i colori ambientale, diffuso e speculare, oltre a un ID univoco per
 * identificare ogni luce. La gestione degli ID garantisce che il numero massimo di luci supportato
 * da OpenGL non venga superato.
 */
class LIB_API Light : public Node
{
public:

    /**
     * @brief Costruttore della classe `Light`.
     * @param type Il tipo di luce (es. "PointLight", "SpotLight").
     */
    Light(const std::string& type);

    /**
     * @brief Distruttore della classe `Light`.
     *
     * Rimuove la luce decrementando il contatore globale degli ID delle luci.
     */
    ~Light();

    // Getter

    /**
     * @brief Restituisce il numero della luce OpenGL associato all'ID.
     * @param lightId L'ID della luce.
     * @return Il numero della luce OpenGL.
     */
    int getCurrentLight(const int lightId) const;

    /**
     * @brief Restituisce il colore ambientale della luce.
     * @return Il colore ambientale della luce come `glm::vec3`.
     */
    glm::vec3 getAmbientColor() const;

    /**
     * @brief Restituisce il colore diffuso della luce.
     * @return Il colore diffuso della luce come `glm::vec3`.
     */
    glm::vec3 getDiffuseColor() const;

    /**
     * @brief Restituisce il colore speculare della luce.
     * @return Il colore speculare della luce come `glm::vec3`.
     */
    glm::vec3 getSpecularColor() const;

    // Setter

    /**
     * @brief Imposta il colore ambientale della luce.
     * @param newColor Il nuovo colore ambientale come `glm::vec3`.
     */
    void setAmbientColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore diffuso della luce.
     * @param newColor Il nuovo colore diffuso come `glm::vec3`.
     */
    void setDiffuseColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore speculare della luce.
     * @param newColor Il nuovo colore speculare come `glm::vec3`.
     */
    void setSpecularColor(const glm::vec3 newColor);

    /**
     * @brief Resetta il contatore degli ID delle luci.
     *
     * Questo metodo pu� essere utilizzato per inizializzare il sistema di luci,
     * riportando il contatore degli ID delle luci al valore iniziale.
     */
    static void resetNextLightId();

protected:

    static int nextLightId;  ///< ID della prossima luce disponibile.

    glm::vec3 _ambientColor;   ///< Colore ambientale della luce.
    glm::vec3 _diffuseColor;   ///< Colore diffuso della luce.
    glm::vec3 _specularColor;  ///< Colore speculare della luce.

    int _lightId;              ///< ID della luce corrente.
};
