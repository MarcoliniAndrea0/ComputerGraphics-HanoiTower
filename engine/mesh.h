/**
 * @file mesh.h
 * @brief Definizione della classe Mesh per la rappresentazione di oggetti 3D.
 *
 * Questo file contiene la dichiarazione della classe Mesh, che rappresenta
 * un oggetto tridimensionale renderizzabile composto da geometria e materiale.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include "material.h"
#include "node.h"
#include "MeshData.h"
#include "Common.h"

 /**
  * @class Mesh
  * @brief Rappresenta una mesh 3D renderizzabile nella scena.
  *
  * Una Mesh è la rappresentazione di un oggetto tridimensionale composto da:
  * - **Geometria** (MeshData): Vertici, facce, normali e coordinate UV
  * - **Materiale**: Aspetto visivo della superficie (colori, texture, proprietà)
  * - **Proprietà di rendering**: Come la capacità di proiettare ombre
  *
  * La classe Mesh eredita da Node, quindi supporta:
  * - Trasformazioni gerarchiche (posizione, rotazione, scala)
  * - Relazioni parent-child nel grafo della scena
  * - Priorità di rendering
  *
  * Caratteristiche principali:
  * - Gestione di geometrie complesse attraverso MeshData
  * - Supporto per materiali con texture
  * - Capacità di proiettare ombre (shadow casting)
  * - Modalità di rendering speciale per il color picking
  *
  * @see Node
  * @see MeshData
  * @see Material
  */
class LIB_API Mesh : public Node
{
public:
    /**
     * @brief Costruttore della classe Mesh.
     *
     * Inizializza una nuova mesh con:
     * - Un materiale di default (grigio opaco)
     * - Shadow casting abilitato (proietta ombre)
     * - Geometria vuota (da impostare successivamente)
     *
     * @note Dopo la costruzione, è necessario impostare i dati geometrici
     *       con setMeshData() e opzionalmente un materiale personalizzato
     *       con setMaterial().
     */
    Mesh();

    /**
     * @brief Distruttore della classe Mesh.
     *
     * Libera tutte le risorse associate alla mesh, inclusi:
     * - I dati geometrici (MeshData)
     * - Il materiale associato (tramite shared_ptr)
     *
     * @note La pulizia delle risorse OpenGL (VBO, texture) viene gestita
     *       automaticamente dai rispettivi distruttori.
     */
    ~Mesh();

    // ========================================================================
    // GETTER METHODS
    // ========================================================================

    /**
     * @brief Verifica se la mesh proietta ombre.
     *
     * Determina se questa mesh contribuisce alla generazione di ombre
     * nella scena quando è presente una sorgente di luce che proietta ombre.
     *
     * @return true se la mesh proietta ombre, false altrimenti
     */
    bool getShadows() const;

    /**
     * @brief Restituisce il materiale associato alla mesh.
     *
     * Il materiale definisce l'aspetto visivo della superficie della mesh,
     * inclusi colori, texture e proprietà di illuminazione.
     *
     * @return Puntatore condiviso al materiale corrente
     *
     * @note Il puntatore non sarà mai nullptr - ogni mesh ha sempre
     *       almeno un materiale di default.
     */
    std::shared_ptr<Material> getMaterial() const;

    /**
     * @brief Restituisce i dati geometrici della mesh.
     *
     * Fornisce accesso in sola lettura alla geometria della mesh
     * (vertici, facce, normali, coordinate UV).
     *
     * @return Riferimento costante ai dati geometrici (MeshData)
     *
     * @see MeshData
     */
    const MeshData& getMeshData() const;

    // ========================================================================
    // SETTER METHODS
    // ========================================================================

    /**
     * @brief Imposta un nuovo materiale per la mesh.
     *
     * Sostituisce il materiale corrente con uno nuovo, cambiando così
     * l'aspetto visivo della mesh quando viene renderizzata.
     *
     * @param newMaterial Puntatore condiviso al nuovo materiale da applicare
     *
     * @note Il materiale precedente viene automaticamente rilasciato
     *       se non ci sono altri riferimenti ad esso.
     *
     * Esempio di utilizzo:
     * @code
     * auto redMaterial = std::make_shared<Material>();
     * redMaterial->setDiffuseColor(glm::vec3(1.0f, 0.0f, 0.0f));
     * mesh->setMaterial(redMaterial);
     * @endcode
     */
    void setMaterial(const std::shared_ptr<Material> newMaterial);

    /**
     * @brief Imposta se la mesh deve proiettare ombre.
     *
     * Abilita o disabilita la proiezione di ombre da parte di questa mesh.
     * Disabilitare le ombre può migliorare le performance per oggetti
     * che non necessitano di proiettare ombre (es. oggetti distanti).
     *
     * @param newCastShadows true per abilitare le ombre, false per disabilitarle
     *
     * Esempio di utilizzo:
     * @code
     * mesh->setShadows(false); // Disabilita le ombre per questa mesh
     * @endcode
     */
    void setShadows(const bool newCastShadows);

    /**
     * @brief Imposta i dati geometrici della mesh.
     *
     * Carica nella mesh una nuova geometria definita da vertici, facce,
     * normali e coordinate UV. Questo metodo sostituisce completamente
     * la geometria precedente.
     *
     * @param data Struttura MeshData contenente:
     *             - Vertici (posizioni 3D)
     *             - Facce (triangoli definiti da indici di vertici)
     *             - Normali (per l'illuminazione)
     *             - Coordinate UV (per il texture mapping)
     *
     * Esempio di utilizzo:
     * @code
     * MeshData cubeData;
     * // ... popolare cubeData con dati del cubo ...
     * mesh->setMeshData(cubeData);
     * @endcode
     *
     * @see MeshData
     */
    void setMeshData(const MeshData& data);

    // ========================================================================
    // RENDERING
    // ========================================================================

    /**
     * @brief Renderizza la mesh nella scena.
     *
     * Questo metodo esegue il rendering completo della mesh:
     * 1. Applica il materiale (colori, texture)
     * 2. Applica le trasformazioni (model matrix)
     * 3. Disegna la geometria (vertici e facce)
     *
     * Il comportamento cambia in base alla modalità di rendering:
     * - **Modalità normale**: Rendering completo con materiale e illuminazione
     * - **Modalità color picking**: Rendering con colori solidi per la selezione
     *
     * @param viewMatrix La matrice di vista della camera corrente,
     *                   utilizzata per trasformare la mesh nello spazio della vista
     *
     * @note Questo metodo è chiamato automaticamente dal sistema di rendering.
     *       Non dovrebbe essere necessario chiamarlo manualmente.
     *
     * @see isColorPickingMode
     */
    void render(const glm::mat4 viewMatrix) const override;

    /**
     * @brief Flag statico per la modalità color picking.
     *
     * Quando true, le mesh vengono renderizzate con colori solidi univoci
     * basati sul loro ID, permettendo la selezione degli oggetti tramite
     * il colore del pixel. Utilizzato dal sistema di picking con il mouse.
     *
     * @note Questo è un flag globale che influenza tutte le mesh.
     *       Viene impostato temporaneamente a true durante il color picking,
     *       poi ripristinato a false per il rendering normale.
     *
     * @see Engine::getNodeByClick()
     */
    static bool isColorPickingMode;

private:
    MeshData _meshData;                     ///< Dati geometrici della mesh (vertici, facce, normali, UV)
    std::shared_ptr<Material> _material;    ///< Materiale applicato alla mesh (colori, texture, proprietà)
    bool _castShadows;                      ///< Flag che indica se questa mesh proietta ombre
};
