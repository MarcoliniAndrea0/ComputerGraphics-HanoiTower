#pragma once

#include "material.h"
#include "node.h"
#include "MeshData.h"
#include "Common.h"

/**
 * @class Mesh
 * @brief Rappresenta una mesh che viene renderizzata all'interno di una scena.
 *
 * Una mesh � una rappresentazione tridimensionale di una forma. Include dati geometrici,
 * come vertici e facce, ed � associata a un materiale per definire l'aspetto visivo
 * della superficie. Le mesh possono anche proiettare ombre.
 */
class LIB_API Mesh : public Node
{
public:
    /**
     * @brief Costruttore della classe Mesh.
     *
     * Inizializza una mesh con un materiale di default e abilita le ombre.
     */
    Mesh();

    /**
     * @brief Distruttore della classe Mesh.
     *
     * Libera le risorse associate alla mesh, incluso il materiale.
     */
    ~Mesh();

    // Getter

    /**
     * @brief Verifica se la mesh proietta ombre.
     * @return `true` se la mesh proietta ombre, `false` altrimenti.
     */
    bool getShadows() const;

    /**
     * @brief Restituisce il materiale associato alla mesh.
     * @return Un puntatore condiviso al materiale della mesh.
     */
    std::shared_ptr<Material> getMaterial() const;

    // Setter

    /**
     * @brief Imposta un nuovo materiale per la mesh.
     * @param newMaterial Un puntatore condiviso al nuovo materiale.
     */
    void setMaterial(const std::shared_ptr<Material> newMaterial);

    /**
     * @brief Imposta se la mesh deve proiettare ombre.
     * @param newCastShadows `true` per abilitare le ombre, `false` per disabilitarle.
     */
    void setShadows(const bool newCastShadows);

    /**
     * @brief Imposta i dati della mesh.
     * @param data I dati geometrici della mesh (vertici, facce, normali e coordinate UV).
     */
    void setMeshData(const MeshData& data);

    /**
     * @brief Renderizza la mesh.
     * @param viewMatrix La matrice di vista da utilizzare per il rendering.
     */
    void render(const glm::mat4 viewMatrix) const override;
    const MeshData& getMeshData() const;

    /**
     * @brief Modalit� per il rendering della mesh solo con colori (senza illuminazione).
     */
    static bool isColorPickingMode;

private:
    MeshData _meshData; ///< Dati della mesh, inclusi vertici, facce, normali e coordinate UV.
    std::shared_ptr<Material> _material; ///< Materiale associato alla mesh.
    bool _castShadows; ///< Indica se la mesh deve proiettare ombre.
};
