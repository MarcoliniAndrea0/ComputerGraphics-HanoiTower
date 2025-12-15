/**
 * @file MeshData.h
 * @brief Definizione della classe MeshData per la gestione dei dati geometrici.
 *
 * Questo file contiene la dichiarazione della classe MeshData, che incapsula
 * tutti i dati geometrici necessari per renderizzare una mesh 3D.
 *
 * @author Gruppo6
 * @date 2025
 */
#pragma once

#include "Common.h"

#include <tuple>
#include <vector>
#include <glm/glm.hpp>

 /**
  * @class MeshData
  * @brief Rappresenta i dati geometrici di una mesh 3D.
  *
  * La classe MeshData gestisce tutti i dati geometrici necessari per
  * definire e renderizzare una mesh tridimensionale. Fornisce una struttura
  * organizzata per memorizzare:
  *
  * - **Vertici**: Posizioni dei punti nello spazio 3D
  * - **Facce**: Definizione dei triangoli attraverso indici di vertici
  * - **Normali**: Vettori perpendicolari alle superfici per illuminazione e shading
  * - **Coordinate UV**: Mappatura 2D per l'applicazione delle texture
  *
  * Caratteristiche del sistema:
  * - Geometria basata su triangoli (ogni faccia ha esattamente 3 vertici)
  * - Supporto per normal mapping (una normale per vertice)
  * - Supporto per texture mapping (coordinate UV per vertice)
  * - Organizzazione efficiente dei dati per il rendering
  *
  * I dati sono organizzati in modo che:
  * - Ogni vertice abbia un indice univoco (0, 1, 2, ...)
  * - Ogni faccia faccia riferimento a 3 indici di vertici
  * - Ogni vertice possa avere una normale e coordinate UV associate
  *
  * @note La classe non gestisce direttamente VBO (Vertex Buffer Objects) OpenGL,
  *       ma fornisce i dati in un formato facilmente utilizzabile per crearli.
  *
  * @see Mesh
  */
class LIB_API MeshData
{
public:
    // ========================================================================
    // GETTER METHODS
    // ========================================================================

    /**
     * @brief Restituisce la lista dei vertici della mesh.
     *
     * I vertici sono i punti fondamentali che definiscono la forma della mesh
     * nello spazio 3D. Ogni vertice è rappresentato da un vettore glm::vec3
     * contenente le coordinate (x, y, z).
     *
     * @return Riferimento costante al vettore di vertici
     *
     * Esempio di accesso:
     * @code
     * const auto& vertices = meshData.getVertices();
     * for (const auto& vertex : vertices) {
     *     std::cout << "Vertex: " << vertex.x << ", " << vertex.y << ", " << vertex.z << std::endl;
     * }
     * @endcode
     */
    const std::vector<glm::vec3>& getVertices() const;

    /**
     * @brief Restituisce la lista delle facce della mesh.
     *
     * Le facce definiscono i triangoli che compongono la superficie della mesh.
     * Ogni faccia è rappresentata come una tupla di tre indici interi (uint32_t)
     * che fanno riferimento ai vertici nel vettore dei vertici.
     *
     * Esempio: La faccia (0, 1, 2) crea un triangolo che connette
     *          il vertice 0, il vertice 1 e il vertice 2.
     *
     * @return Riferimento costante al vettore di facce (tuple di 3 indici)
     *
     * Esempio di accesso:
     * @code
     * const auto& faces = meshData.getFaces();
     * for (const auto& [i0, i1, i2] : faces) {
     *     std::cout << "Triangle: " << i0 << ", " << i1 << ", " << i2 << std::endl;
     * }
     * @endcode
     *
     * @note Gli indici devono essere validi (< vertices.size()).
     *       Indici non validi causano comportamento indefinito.
     */
    const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>>& getFaces() const;

    /**
     * @brief Restituisce la lista delle normali della mesh.
     *
     * Le normali sono vettori perpendicolari alle superfici, utilizzati per:
     * - Calcolare l'illuminazione (Phong shading, Lambert shading)
     * - Determinare l'orientamento delle superfici
     * - Implementare effetti di smooth shading
     *
     * Tipicamente c'è una normale per ogni vertice, permettendo interpolazione
     * smooth delle normali tra i vertici di un triangolo.
     *
     * @return Riferimento costante al vettore di normali
     *
     * @note Le normali dovrebbero essere normalizzate (lunghezza = 1.0)
     *       per un calcolo corretto dell'illuminazione.
     */
    const std::vector<glm::vec3>& getNormals() const;

    /**
     * @brief Restituisce la lista delle coordinate UV della mesh.
     *
     * Le coordinate UV definiscono come una texture 2D viene mappata
     * sulla superficie 3D della mesh. Ogni coordinata UV è un vettore 2D:
     * - U (x): Coordinata orizzontale sulla texture (0.0 - 1.0)
     * - V (y): Coordinata verticale sulla texture (0.0 - 1.0)
     *
     * Tipicamente c'è una coppia di coordinate UV per ogni vertice,
     * permettendo al rendering engine di determinare quale pixel della
     * texture corrisponde a quale punto sulla superficie.
     *
     * @return Riferimento costante al vettore di coordinate UV
     *
     * @note Le coordinate UV sono tipicamente nel range [0.0, 1.0],
     *       ma valori fuori da questo range sono validi e causano
     *       il wrapping o clamping della texture.
     */
    const std::vector<glm::vec2>& getUVs() const;

    // ========================================================================
    // SETTER METHODS
    // ========================================================================

    /**
     * @brief Imposta tutti i dati geometrici della mesh in un'unica chiamata.
     * 
     * Questo metodo configura completamente la geometria della mesh fornendo
     * tutti i dati necessari: vertici, facce, normali e coordinate UV.
     * Sostituisce completamente eventuali dati precedenti.
     * 
     * @param new_vertices Vettore di posizioni 3D dei vertici
     * @param new_faces Vettore di tuple di 3 indici che definiscono i triangoli
     * @param new_normals Vettore di normali 3D (una per vertice tipicamente)
     * @param new_uvs Vettore di coordinate 2D per il texture mapping
     * 
     * Esempio di utilizzo:
     * @code
     * MeshData cubeData;
     * 
     * std::vector<glm::vec3> vertices = {
     *     {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
     *     {-1, -1, 1},  {1, -1, 1},  {1, 1, 1},  {-1, 1, 1}
     * };
     * 
     * std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> faces = {
     *     {0, 1, 2}, {0, 2, 3}, // Front face
     *     // ... altre facce ...
     * };
     * 
     * std::vector<glm::vec3> normals = {  ...  };
     * std::vector<glm::vec2> uvs = {  ...  };
     * 
     * cubeData.set_mesh_data(vertices, faces, normals, uvs);
     * @endcode
     * 
     * @note È importante che:
     *       - Le dimensioni dei vettori siano coerenti
     *       - Gli indici nelle facce siano validi (< vertices.size())
     *       - Le normali siano normalizzate per un'illuminazione corretta
     */
    void set_mesh_data(
        const std::vector<glm::vec3> new_vertices,
        const std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> new_faces,
        const std::vector<glm::vec3> new_normals,
        const std::vector<glm::vec2> new_uvs);

private:
    std::vector<glm::vec3> _vertices;                               ///< Posizioni 3D dei vertici (x, y, z)
    std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> _faces;  ///< Triangoli definiti da triple di indici
    std::vector<glm::vec3> _normals;                                ///< Vettori normali per l'illuminazione
    std::vector<glm::vec2> _uvs;                                    ///< Coordinate 2D per il texture mapping (u, v)
};
