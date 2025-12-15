/**
 * @file material.h
 * @brief Definizione della classe Material per la gestione dei materiali delle mesh.
 *
 * Questo file contiene la dichiarazione della classe Material, che definisce
 * l'aspetto visivo delle superfici 3D attraverso colori e texture.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include <memory>
#include "object.h"
#include "texture.h"
#include "Common.h"

 /**
  * @class Material
  * @brief Rappresenta un materiale che definisce l'aspetto visivo di una mesh.
  *
  * La classe Material gestisce tutti gli attributi che determinano come una
  * superficie 3D appare quando viene illuminata e renderizzata. Implementa
  * il modello di illuminazione di Phong con supporto per texture.
  *
  * Componenti del materiale (Modello di Phong):
  * - **Emissione**: Luce emessa direttamente dalla superficie (auto-luminosità)
  * - **Ambiente**: Come la superficie riflette la luce ambientale diffusa
  * - **Diffuso**: Come la superficie riflette la luce diretta (colore principale)
  * - **Speculare**: Come la superficie riflette i punti di luce brillanti
  * - **Shininess**: Quanto concentrati sono i riflessi speculari (lucidità)
  * - **Alpha**: Trasparenza del materiale (0.0 = trasparente, 1.0 = opaco)
  * - **Texture**: Immagine applicata sulla superficie (opzionale)
  *
  * Il materiale viene applicato alle mesh per determinare:
  * - Il colore della superficie sotto diverse condizioni di illuminazione
  * - La risposta ai vari tipi di luce nella scena
  * - L'eventuale mappatura di texture
  *
  * @see Mesh
  * @see Texture
  * @see Light
  */
class LIB_API Material : public Object
{
public:
    /**
     * @brief Costruttore di default per la classe Material.
     *
     * Inizializza un materiale con valori predefiniti appropriati:
     * - Emissione: nero (0.0, 0.0, 0.0) - nessuna auto-luminosità
     * - Ambiente: grigio scuro (0.2, 0.2, 0.2)
     * - Diffuso: grigio medio (0.8, 0.8, 0.8) - colore principale
     * - Speculare: bianco (1.0, 1.0, 1.0) - riflessi bianchi
     * - Shininess: 32.0 - moderatamente lucido
     * - Alpha: 1.0 - completamente opaco
     * - Texture: nullptr - nessuna texture
     */
    Material();

    /**
     * @brief Distruttore virtuale di default.
     *
     * Libera automaticamente la texture associata se presente
     * (tramite std::shared_ptr).
     */
    virtual ~Material() = default;

    // ========================================================================
    // GETTER METHODS
    // ========================================================================

    /**
     * @brief Restituisce il colore di emissione del materiale.
     *
     * Il colore di emissione rappresenta la luce che la superficie emette
     * autonomamente, indipendentemente dall'illuminazione esterna.
     * Utile per simulare oggetti che emettono luce come LED, schermi, ecc.
     *
     * @return Vettore RGB del colore di emissione (0.0-1.0 per canale)
     *
     * @note Un valore di (0, 0, 0) significa nessuna emissione (default).
     */
    glm::vec3 getEmissionColor() const;

    // ========================================================================
    // SETTER METHODS - COLORI
    // ========================================================================

    /**
     * @brief Imposta il colore di emissione del materiale.
     *
     * Modifica la quantità di luce che la superficie emette autonomamente.
     * Valori non-zero renderanno l'oggetto visibile anche senza illuminazione.
     *
     * @param newColor Nuovo colore di emissione come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * material->setEmissionColor(glm::vec3(1.0f, 0.0f, 0.0f)); // Rosso brillante
     * @endcode
     */
    void setEmissionColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore ambientale del materiale.
     *
     * Definisce come la superficie riflette la luce ambientale diffusa
     * presente nella scena. Tipicamente un colore a bassa intensità.
     *
     * @param newColor Nuovo colore ambientale come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * material->setAmbientColor(glm::vec3(0.1f, 0.1f, 0.2f)); // Tinta bluastra
     * @endcode
     */
    void setAmbientColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore diffuso del materiale.
     *
     * Il colore diffuso è il colore principale della superficie,
     * quello che percepiamo sotto illuminazione normale.
     * Determina come la superficie riflette la luce diretta.
     *
     * @param newColor Nuovo colore diffuso come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * material->setDiffuseColor(glm::vec3(0.8f, 0.2f, 0.2f)); // Rosso
     * @endcode
     */
    void setDiffuseColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore speculare del materiale.
     *
     * Definisce il colore dei riflessi brillanti sulla superficie.
     * Solitamente bianco o simile al colore della luce per materiali realistici.
     *
     * @param newColor Nuovo colore speculare come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * material->setSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Riflessi bianchi
     * @endcode
     */
    void setSpecularColor(const glm::vec3 newColor);

    // ========================================================================
    // SETTER METHODS - PROPRIETÀ FISICHE
    // ========================================================================

    /**
     * @brief Imposta il valore di shininess (lucidità) del materiale.
     *
     * La shininess controlla quanto sono concentrati i riflessi speculari:
     * - Valori bassi (1-10): Riflessi ampi e diffusi (superfici opache)
     * - Valori medi (10-100): Riflessi moderatamente concentrati (plastica, legno)
     * - Valori alti (100+): Riflessi molto concentrati (metallo lucido, specchi)
     *
     * @param newShininess Nuovo valore di shininess (tipicamente 1-128)
     *
     * Esempio di utilizzo:
     * @code
     * material->setShininess(128.0f); // Superficie molto lucida (metallo)
     * material->setShininess(10.0f);  // Superficie opaca (gomma)
     * @endcode
     */
    void setShininess(const float newShininess);

    /**
     * @brief Imposta il valore alpha (trasparenza) del materiale.
     *
     * L'alpha determina l'opacità della superficie:
     * - 0.0: Completamente trasparente (invisibile)
     * - 1.0: Completamente opaco (default)
     * - Valori intermedi: Semi-trasparente
     *
     * @param newAlpha Nuovo valore alpha nel range [0.0, 1.0]
     *
     * Esempio di utilizzo:
     * @code
     * material->setAlpha(0.5f); // 50% trasparente (vetro)
     * material->setAlpha(0.2f); // Molto trasparente
     * @endcode
     *
     * @note Per rendering corretto della trasparenza, gli oggetti semi-trasparenti
     *       devono essere renderizzati in ordine back-to-front.
     */
    void setAlpha(const float newAlpha);

    /**
     * @brief Imposta la texture da applicare al materiale.
     *
     * Associa un'immagine texture che verrà mappata sulla superficie della mesh.
     * La texture sostituisce o modula il colore diffuso del materiale.
     *
     * @param newTexture Puntatore condiviso alla texture da applicare,
     *                   o nullptr per rimuovere la texture corrente
     *
     * Esempio di utilizzo:
     * @code
     * auto tex = std::make_shared<Texture>("textures/brick.jpg");
     * material->setTexture(tex);
     * @endcode
     *
     * @see Texture
     */
    void setTexture(const std::shared_ptr<Texture> newTexture);

    // ========================================================================
    // RENDERING
    // ========================================================================

    /**
     * @brief Applica il materiale al contesto OpenGL per il rendering.
     *
     * Configura tutti i parametri del materiale in OpenGL, inclusi:
     * - I colori di emissione, ambiente, diffuso e speculare
     * - Il valore di shininess
     * - Il valore alpha (trasparenza)
     * - La texture (se presente)
     *
     * Questo metodo deve essere chiamato prima di renderizzare la geometria
     * a cui si vuole applicare questo materiale.
     *
     * @param viewMatrix La matrice di vista corrente (non utilizzata direttamente
     *                   dal materiale ma richiesta dall'interfaccia Object)
     *
     * @note Questo metodo è chiamato automaticamente dal sistema di rendering.
     *       Non dovrebbe essere necessario chiamarlo manualmente.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:
    glm::vec3 _emissionColor;               ///< Colore emesso autonomamente dalla superficie (0.0-1.0)
    glm::vec3 _ambientColor;                ///< Colore di riflessione della luce ambientale (0.0-1.0)
    glm::vec3 _diffuseColor;                ///< Colore principale della superficie (0.0-1.0)
    glm::vec3 _specularColor;               ///< Colore dei riflessi brillanti (0.0-1.0)
    float _shininess;                        ///< Concentrazione dei riflessi speculari (1-128)
    float _alpha;                            ///< Trasparenza del materiale (0.0 = trasparente, 1.0 = opaco)
    std::shared_ptr<Texture> _texture;      ///< Texture applicata al materiale (opzionale)
};
