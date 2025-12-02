/**
 * @file Mesh.cpp
 * @brief Implementazione della classe Mesh.
 */
#include "mesh.h"
#include "engine.h"
#include "material.h"
#include <iostream>

Mesh::Mesh(const std::string& name) : Node(name, "Mesh")
{
}

void Mesh::render()
{
    // 1. Applica il materiale (se presente nel nodo)
    if (this->getMaterial()) {
        this->getMaterial()->render();
    }
    else {
        // Materiale di default se nullo (bianco opaco)
        float gray[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
    }

    // 2. Disegna i triangoli
    glBegin(GL_TRIANGLES);

    // Iteriamo su tutti gli indici salvati (3 alla volta per ogni faccia)
    for (size_t i = 0; i < m_facesVertices.size(); i++) {

        // Recuperiamo gli indici per questo specifico vertice del triangolo
        unsigned int vIdx = m_facesVertices[i];
        unsigned int nIdx = m_facesNormals[i];
        unsigned int tIdx = (i < m_facesTexCoords.size()) ? m_facesTexCoords[i] : 0; // Check bounds

        // A. Normale (Importante per la luce!)
        if (nIdx < m_normals.size()) {
            glNormal3fv(glm::value_ptr(m_normals[nIdx]));
        }

        // B. Coordinate Texture (Se ci sono e sono valide)
        if (!m_texCoords.empty() && tIdx < m_texCoords.size()) {
            glTexCoord2fv(glm::value_ptr(m_texCoords[tIdx]));
        }

        // C. Posizione (Deve essere l'ultima chiamata)
        if (vIdx < m_vertices.size()) {
            glVertex3fv(glm::value_ptr(m_vertices[vIdx]));
        }
    }

    glEnd();

    // Renderizza eventuali figli (sistema gerarchico)
    Node::render();
}

void Mesh::addVertex(const glm::vec3& vertex) { m_vertices.push_back(vertex); }
void Mesh::addNormal(const glm::vec3& normal) { m_normals.push_back(normal); }
void Mesh::addTexCoord(const glm::vec2& texCoord) { m_texCoords.push_back(texCoord); }

void Mesh::addFace(unsigned int vIndex, unsigned int nIndex, unsigned int tIndex) {
    m_facesVertices.push_back(vIndex);
    m_facesNormals.push_back(nIndex);
    m_facesTexCoords.push_back(tIndex);
}