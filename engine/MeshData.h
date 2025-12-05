#pragma once

#include "Common.h"

#include <tuple>
#include <vector>
#include <glm/glm.hpp>

/**
 * @class MeshData
 * @brief Rappresenta i dati di una mesh: vertici, facce, normali, coordinate UV.
 *
 * Questa classe gestisce i dati geometrici necessari per renderizzare una mesh nella scena.
 * Include informazioni su:
 * - Vertici (`_vertices`): Posizioni dei punti nello spazio 3D.
 * - Facce (`_faces`): Definizione dei triangoli attraverso indici di vertici.
 * - Normali (`_normals`): Vettori per illuminazione e shading.
 * - Coordinate UV (`_uvs`): Mappatura delle texture.
 */
class LIB_API MeshData
{
public:
    // Getter

    /**
     * @brief Restituisce la lista dei vertici per questa mesh.
     * @return Un riferimento costante al vettore dei vertici.
     */
    const std::vector<glm::vec3>& getVertices() const;

    /**
     * @brief Restituisce la lista delle facce per questa mesh.
     *
     * Le facce sono rappresentate come tuple di indici, ciascuna contenente tre indici che formano un triangolo.
     * @return Un riferimento costante al vettore delle facce.
     */
    const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& getFaces() const;

    /**
     * @brief Restituisce la lista delle normali per questa mesh.
     * @return Un riferimento costante al vettore delle normali.
     */
    const std::vector<glm::vec3>& getNormals() const;

    /**
     * @brief Restituisce la lista delle coordinate UV per questa mesh.
     * @return Un riferimento costante al vettore delle coordinate UV.
     */
    const std::vector<glm::vec2>& getUVs() const;

    // Setter

    /**
     * @brief Imposta i dati della mesh, inclusi vertici, facce, normali e coordinate UV.
     *
     * @param newVertices I nuovi vertici per la mesh.
     * @param newFaces Le nuove facce per la mesh, rappresentate come tuple di indici dei vertici.
     * @param newNormals Le nuove normali per la mesh.
     * @param newUvs Le nuove coordinate UV per la mesh.
     */
    void set_mesh_data(
        const std::vector<glm::vec3> new_vertices,
        const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> new_faces,
        const std::vector<glm::vec3> new_normals,
        const std::vector<glm::vec2> new_uvs);

private:
    std::vector<glm::vec3> _vertices; ///< Lista dei vertici della mesh.
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> _faces; ///< Lista delle facce della mesh.
    std::vector<glm::vec3> _normals; ///< Lista delle normali della mesh.
    std::vector<glm::vec2> _uvs; ///< Lista delle coordinate UV della mesh.
};
