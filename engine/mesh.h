#pragma once

#include "node.h"
#include <vector>
#include <glm/glm.hpp>

class ENG_API Mesh : public Node
{
public:
    /**
     * @brief Costruttore.
     * @param name Nome della mesh.
     */
    Mesh(const std::string& name);

    virtual ~Mesh() = default;

    /**
     * @brief Renderizza la mesh usando i dati salvati.
     */
    void render() override;

    // --- Metodi per costruire la mesh (usati dal Parser o manualmente) ---

    void addVertex(const glm::vec3& vertex);
    void addNormal(const glm::vec3& normal);
    void addTexCoord(const glm::vec2& texCoord);

    /**
     * @brief Aggiunge una faccia (triangolo).
     * @param vIndex Indice del vertice
     * @param nIndex Indice della normale
     * @param tIndex Indice della coordinata texture
     */
    void addFace(unsigned int vIndex, unsigned int nIndex, unsigned int tIndex);

    // Getters (opzionali per debug)
    size_t getVertexCount() const 
    {
        return m_vertices.size(); 
    }

    size_t getFaceCount() const 
    { 
        return m_facesVertices.size() / 3; 
    } // 3 indici per faccia

private:
    // Dati geometrici grezzi
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_texCoords;

    // Indici per disegnare i triangoli (Topo-logia)
    // Salviamo gli indici separati per posizione, normale e UV (come fa OBJ/OVO)
    std::vector<unsigned int> m_facesVertices;
    std::vector<unsigned int> m_facesNormals;
    std::vector<unsigned int> m_facesTexCoords;
};