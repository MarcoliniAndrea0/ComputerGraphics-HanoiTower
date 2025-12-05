#include "MeshData.h"

// Getter

/**
 * @brief Restituisce la lista dei vertici per questa mesh.
 * @return Un riferimento costante al vettore dei vertici.
 */
LIB_API const std::vector<glm::vec3>& MeshData::getVertices() const {
    return _vertices;
}

/**
 * @brief Restituisce la lista delle facce per questa mesh.
 *
 * Le facce sono rappresentate come tuple di indici, ciascuna contenente tre indici che formano un triangolo.
 * @return Un riferimento costante al vettore delle facce.
 */
LIB_API const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& MeshData::getFaces() const {
    return _faces;
}

/**
 * @brief Restituisce la lista delle normali per questa mesh.
 * @return Un riferimento costante al vettore delle normali.
 */
LIB_API const std::vector<glm::vec3>& MeshData::getNormals() const {
    return _normals;
}

/**
 * @brief Restituisce la lista delle coordinate UV per questa mesh.
 * @return Un riferimento costante al vettore delle coordinate UV.
 */
LIB_API const std::vector<glm::vec2>& MeshData::getUVs() const {
    return _uvs;
}

// Setter

/**
 * @brief Imposta i dati della mesh, inclusi vertici, facce, normali e coordinate UV.
 *
 * @param newVertices I nuovi vertici per la mesh.
 * @param newFaces Le nuove facce per la mesh, rappresentate come tuple di indici dei vertici.
 * @param newNormals Le nuove normali per la mesh.
 * @param newUvs Le nuove coordinate UV per la mesh.
 */
void LIB_API MeshData::set_mesh_data(
    const std::vector<glm::vec3> newVertices,
    const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> newFaces,
    const std::vector<glm::vec3> newNormals,
    const std::vector<glm::vec2> newUvs)
{
    // Assegna i nuovi dati ai membri privati della classe
    _vertices = newVertices;
    _faces = newFaces;
    _normals = newNormals;
    _uvs = newUvs;
}
