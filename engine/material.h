#pragma once

#include <memory>
#include "Object.h"
#include "Texture.h"
#include "Common.h"

/**
 * @class Material
 * @brief Rappresenta un materiale che conferisce colore e texture alle mesh.
 *
 * La classe `Material` gestisce gli attributi visivi di un materiale, come il colore di emissione,
 * il colore ambientale, il colore diffuso e il colore speculare. Inoltre, supporta la definizione
 * della lucentezza, del valore alpha e dell'applicazione di una texture.
 *
 * Un materiale viene applicato a una mesh per determinare come essa interagisce con la luce
 * e come viene visualizzata nella scena.
 */
class LIB_API Material : public Object
{
public:
    /**
     * @brief Costruttore di default per la classe `Material`.
     *
     * Inizializza un materiale con valori predefiniti per emissione, colore ambientale,
     * colore diffuso, colore speculare, lucentezza e trasparenza.
     */
    Material();
    virtual ~Material() = default;

    // Getter

    /**
     * @brief Restituisce il colore di emissione del materiale.
     * @return Il colore di emissione del materiale.
     */
    glm::vec3 getEmissionColor() const;

    // Setter

    /**
     * @brief Cambia il colore di emissione del materiale.
     * @param newColor Il nuovo colore di emissione.
     */
    void setEmissionColor(const glm::vec3 newColor);

    /**
     * @brief Cambia il colore ambientale del materiale.
     * @param newColor Il nuovo colore ambientale.
     */
    void setAmbientColor(const glm::vec3 newColor);

    /**
     * @brief Cambia il colore diffuso del materiale.
     * @param newColor Il nuovo colore diffuso.
     */
    void setDiffuseColor(const glm::vec3 newColor);

    /**
     * @brief Cambia il colore speculare del materiale.
     * @param newColor Il nuovo colore speculare.
     */
    void setSpecularColor(const glm::vec3 newColor);

    /**
     * @brief Cambia il valore di lucentezza del materiale.
     * @param newShininess Il nuovo valore di lucentezza.
     */
    void setShininess(const float newShininess);

    /**
     * @brief Cambia il valore alpha del materiale.
     * @param newAlpha Il nuovo valore alpha.
     */
    void setAlpha(const float newAlpha);

    /**
     * @brief Imposta la texture associata al materiale.
     * @param newTexture La nuova texture da applicare.
     */
    void setTexture(const std::shared_ptr<Texture> newTexture);

    /**
     * @brief Renderizza il materiale utilizzando le impostazioni correnti.
     * @param viewMatrix La matrice di vista da utilizzare.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:
    glm::vec3 _emissionColor; ///< Colore di emissione del materiale.
    glm::vec3 _ambientColor; ///< Colore ambientale del materiale.
    glm::vec3 _diffuseColor; ///< Colore diffuso del materiale.
    glm::vec3 _specularColor; ///< Colore speculare del materiale.
    float _shininess; ///< Lucentezza del materiale.
    float _alpha; ///< Valore alpha del materiale.
    std::shared_ptr<Texture> _texture; ///< Texture applicata al materiale.
};
