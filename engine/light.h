/**
 * @file light.h
 * @brief Definizione della classe base Light per il sistema di illuminazione.
 *
 * Questo file contiene la dichiarazione della classe Light, che funge da classe
 * base per tutti i tipi di luci nel motore grafico (direzionali, puntiformi, spot).
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include "node.h"
#include "Common.h"

 /**
  * @class Light
  * @brief Classe base astratta per l'implementazione delle luci nella scena 3D.
  *
  * La classe Light rappresenta una sorgente di luce generica e serve come classe
  * base per derivare tipi specifici di illuminazione come:
  * - DirectionalLight: luce direzionale (es. sole)
  * - PointLight: luce puntiforme (es. lampadina)
  * - SpotLight: luce spot/conica (es. torcia)
  *
  * Caratteristiche principali:
  * - **Modello di illuminazione di Phong**: Supporta tre componenti di colore:
  *   - Ambientale: luce diffusa uniforme
  *   - Diffuso: luce che dipende dall'angolo della superficie
  *   - Speculare: riflessione brillante sui materiali lucidi
  *
  * - **Gestione automatica degli ID**: Ogni luce riceve un ID univoco OpenGL
  *   (GL_LIGHT0, GL_LIGHT1, ecc.) per essere utilizzata nel rendering.
  *   OpenGL tradizionalmente supporta un massimo di 8 luci simultanee.
  *
  * - **Gerarchia della scena**: Eredita da Node, quindi le luci possono essere
  *   posizionate e orientate nella scena come qualsiasi altro oggetto.
  *
  * @see Node
  * @see DirectionalLight
  * @see PointLight
  * @see SpotLight
  */
class LIB_API Light : public Node
{
public:
    /**
     * @brief Costruttore della classe Light.
     *
     * Inizializza una nuova luce con:
     * - ID univoco OpenGL assegnato automaticamente (se disponibile)
     * - Colori di default per ambientale, diffuso e speculare
     * - Incremento del contatore statico nextLightId
     *
     * @param type Stringa che identifica il tipo specifico di luce
     *             (es. "PointLight", "DirectionalLight", "SpotLight")
     *
     * @warning Se vengono create più di 8 luci (o il limite OpenGL della GPU),
     *          le luci successive non riceveranno un ID valido e non saranno
     *          renderizzate correttamente.
     */
    Light(const std::string& type);

    /**
     * @brief Distruttore della classe Light.
     *
     * Libera le risorse associate alla luce e decrementa il contatore
     * globale degli ID, permettendo il riutilizzo dello slot OpenGL.
     *
     * @note Il decremento del contatore permette di creare nuove luci
     *       dopo averne distrutte altre, rimanendo entro il limite OpenGL.
     */
    ~Light();

    // ========================================================================
    // GETTER METHODS
    // ========================================================================

    /**
     * @brief Restituisce l'identificatore OpenGL della luce.
     *
     * Converte l'ID interno della luce nell'enumerazione OpenGL corrispondente
     * (GL_LIGHT0, GL_LIGHT1, ..., GL_LIGHT7).
     *
     * @param lightId L'ID interno della luce (0-7)
     * @return Il valore dell'enumerazione OpenGL (GL_LIGHT0 + lightId)
     *
     * @note Questo metodo è utilizzato internamente dal sistema di rendering
     *       per attivare la luce corretta in OpenGL.
     */
    int getCurrentLight(const int lightId) const;

    /**
     * @brief Restituisce il colore ambientale della luce.
     *
     * La componente ambientale rappresenta la luce diffusa che illumina
     * uniformemente tutti gli oggetti, indipendentemente dalla loro orientazione.
     * Simula la luce riflessa dall'ambiente circostante.
     *
     * @return Vettore RGB (valori 0.0-1.0) del colore ambientale
     */
    glm::vec3 getAmbientColor() const;

    /**
     * @brief Restituisce il colore diffuso della luce.
     *
     * La componente diffusa rappresenta la luce diretta che colpisce le superfici.
     * L'intensità dipende dall'angolo tra la superficie e la direzione della luce
     * (legge del coseno di Lambert).
     *
     * @return Vettore RGB (valori 0.0-1.0) del colore diffuso
     */
    glm::vec3 getDiffuseColor() const;

    /**
     * @brief Restituisce il colore speculare della luce.
     *
     * La componente speculare crea i punti di luce brillanti sui materiali lucidi,
     * simulando i riflessi diretti della sorgente luminosa.
     *
     * @return Vettore RGB (valori 0.0-1.0) del colore speculare
     */
    glm::vec3 getSpecularColor() const;

    // ========================================================================
    // SETTER METHODS
    // ========================================================================

    /**
     * @brief Imposta il colore ambientale della luce.
     *
     * Modifica la componente di illuminazione ambientale.
     * Valori tipici sono colori a bassa intensità (es. 0.2, 0.2, 0.2).
     *
     * @param newColor Nuovo colore ambientale come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * light->setAmbientColor(glm::vec3(0.1f, 0.1f, 0.1f)); // Luce ambientale tenue
     * @endcode
     */
    void setAmbientColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore diffuso della luce.
     *
     * Modifica la componente di illuminazione diffusa principale.
     * Questo è tipicamente il colore principale e più intenso della luce.
     *
     * @param newColor Nuovo colore diffuso come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * light->setDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Luce bianca
     * light->setDiffuseColor(glm::vec3(1.0f, 0.8f, 0.6f)); // Luce calda
     * @endcode
     */
    void setDiffuseColor(const glm::vec3 newColor);

    /**
     * @brief Imposta il colore speculare della luce.
     *
     * Modifica la componente dei riflessi brillanti.
     * Solitamente è dello stesso colore della componente diffusa o bianco puro.
     *
     * @param newColor Nuovo colore speculare come vettore RGB (0.0-1.0)
     *
     * Esempio di utilizzo:
     * @code
     * light->setSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f)); // Riflessi bianchi
     * @endcode
     */
    void setSpecularColor(const glm::vec3 newColor);

    /**
     * @brief Resetta il contatore degli ID delle luci a zero.
     *
     * Riporta il generatore di ID al suo stato iniziale.
     * Utile quando si vuole ripartire da zero con la gestione delle luci,
     * ad esempio durante il caricamento di una nuova scena.
     *
     * @warning Chiamare questo metodo mentre esistono luci attive può causare
     *          conflitti di ID. Assicurarsi di rimuovere tutte le luci prima
     *          di resettare il contatore.
     *
     * @code
     * // Uso corretto:
     * scene->removeAllObjects(); // Rimuove tutte le luci
     * Light::resetNextLightId(); // Ora è sicuro resettare
     * @endcode
     */
    static void resetNextLightId();

protected:
    /**
     * @brief Contatore statico per il prossimo ID luce disponibile.
     *
     * Tiene traccia di quante luci sono state create per assegnare
     * ID univoci. Viene incrementato ad ogni creazione e decrementato
     * ad ogni distruzione.
     */
    static int nextLightId;

    glm::vec3 _ambientColor;   ///< Colore della componente ambientale (0.0-1.0 per canale)
    glm::vec3 _diffuseColor;   ///< Colore della componente diffusa (0.0-1.0 per canale)
    glm::vec3 _specularColor;  ///< Colore della componente speculare (0.0-1.0 per canale)

    int _lightId;              ///< ID interno della luce (0-7 per OpenGL tradizionale)
};
