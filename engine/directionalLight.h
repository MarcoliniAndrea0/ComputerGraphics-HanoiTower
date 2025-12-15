/**
 * @file directionalLight.h
 * @brief Definizione della classe DirectionalLight per luci direzionali.
 *
 * Questo file contiene la dichiarazione della classe DirectionalLight,
 * che rappresenta una sorgente di luce direzionale nel motore grafico.
 * Le luci direzionali simulano sorgenti luminose molto distanti come il sole.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include "light.h"
#include "Common.h"

 /**
  * @class DirectionalLight
  * @brief Rappresenta una luce direzionale nella scena 3D.
  *
  * Una DirectionalLight è una sorgente di luce che emette raggi paralleli
  * provenienti da una distanza infinita in una direzione specifica.
  * Questo tipo di illuminazione simula fonti di luce molto distanti come il sole,
  * dove tutti i raggi luminosi possono essere considerati paralleli tra loro.
  *
  * Le caratteristiche principali di una luce direzionale sono:
  * - I raggi luminosi sono paralleli (non divergono)
  * - La posizione della luce è irrilevante (solo la direzione conta)
  * - L'intensità della luce non si attenua con la distanza
  * - Ideale per simulare il sole o altre sorgenti luminose distanti
  *
  * @see Light
  * @see PointLight
  * @see SpotLight
  */
class LIB_API DirectionalLight : public Light
{

public:
    /**
     * @brief Costruttore di default della classe DirectionalLight.
     *
     * Inizializza una nuova luce direzionale con parametri di default:
     * - Direzione orientata verso l'alto: (0.0f, 1.0f, 0.0f)
     * - Colori ambientale, diffuso e speculare ereditati dalla classe base Light
     *
     * @note La direzione di default punta verso l'alto (+Y), quindi illumina
     *       gli oggetti dal basso verso l'alto.
     */
    DirectionalLight();

    /**
     * @brief Distruttore virtuale di default.
     *
     * Permette la corretta distruzione e pulizia delle risorse.
     */
    virtual ~DirectionalLight() = default;

    // ========================================================================
    // SETTER METHODS
    // ========================================================================

    /**
     * @brief Imposta la direzione della luce direzionale.
     *
     * Modifica la direzione da cui provengono i raggi luminosi paralleli.
     * Il vettore di direzione verrà normalizzato internamente se necessario.
     *
     * @param newDirection Vettore 3D che rappresenta la nuova direzione della luce
     *                     (non è necessario che sia normalizzato)
     *
     * @note La direzione indica da dove proviene la luce, non dove punta.
     *       Ad esempio, una direzione (0, -1, 0) significa che la luce
     *       proviene dall'alto e illumina verso il basso.
     *
     * Esempio di utilizzo:
     * @code
     * auto sunLight = std::make_shared<DirectionalLight>();
     * // Luce che proviene dall'alto-sinistra-dietro
     * sunLight->setDirection(glm::vec3(-1.0f, -1.0f, -1.0f));
     * @endcode
     */
    void setDirection(const glm::vec3 newDirection);

    // ========================================================================
    // RENDERING
    // ========================================================================

    /**
     * @brief Renderizza la luce direzionale nella scena.
     *
     * Questo metodo configura i parametri OpenGL necessari per applicare
     * l'illuminazione direzionale alla scena. Imposta la direzione della luce
     * nello spazio della vista e configura i colori ambientale, diffuso e speculare.
     *
     * La direzione viene trasformata usando la view matrix per essere espressa
     * nel corretto sistema di coordinate (view space).
     *
     * @param viewMatrix La matrice di vista utilizzata per il rendering,
     *                   necessaria per trasformare la direzione della luce
     *                   nel sistema di coordinate della camera
     *
     * @note Questo metodo è chiamato automaticamente dal sistema di rendering
     *       durante il rendering della scena. Non dovrebbe essere chiamato
     *       manualmente dall'utente.
     */
    void render(const glm::mat4 viewMatrix) const override;

private:

    glm::vec3 _direction; ///< Direzione da cui proviene la luce (vettore nello spazio world)
};
