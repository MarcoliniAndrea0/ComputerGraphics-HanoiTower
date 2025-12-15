#include "mesh.h"

#include <GL/freeglut.h>

bool Mesh::isColorPickingMode = false;

/**
 * @brief Costruttore della classe Mesh.
 *
 * Inizializza una mesh con un materiale di default e abilita le ombre.
 */
Mesh::Mesh()
    : Node{ "Mesh" }
{
    this->setMaterial(std::make_shared<Material>());
    this->setShadows(true);
}

/**
 * @brief Distruttore della classe Mesh.
 *
 * Libera le risorse associate alla mesh, incluso il materiale.
 */
Mesh::~Mesh()
{
    _material.reset(); // Libera il puntatore condiviso al materiale.
}

// Getter

bool LIB_API Mesh::getShadows() const
{
    return this->_castShadows;
}

std::shared_ptr<Material> LIB_API Mesh::getMaterial() const
{
    return this->_material;
}

const MeshData LIB_API &Mesh::getMeshData() const {
    return _meshData;
}


// Setter

void LIB_API Mesh::setMaterial(const std::shared_ptr<Material> newMaterial)
{
    this->_material = newMaterial;
}

void LIB_API Mesh::setShadows(const bool newShadows)
{
    this->_castShadows = newShadows;
}

void LIB_API Mesh::setMeshData(const MeshData& data)
{
    _meshData = data;
}

// Render Mesh

void LIB_API Mesh::render(const glm::mat4 viewMatrix) const
{
    Node::render(viewMatrix);

    if (Mesh::isColorPickingMode)
    {
        int id = this->getId();
        float idRange = id / 255.0f;

        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glColor4f(idRange, idRange, idRange, 1.0f);

        for (const auto& face : _meshData.getFaces())
        {
            glBegin(GL_TRIANGLES);

            const glm::vec3 vertex_0 = _meshData.getVertices()[std::get<0>(face)];
            const glm::vec3 vertex_1 = _meshData.getVertices()[std::get<1>(face)];
            const glm::vec3 vertex_2 = _meshData.getVertices()[std::get<2>(face)];

            glVertex3f(vertex_0.x, vertex_0.y, vertex_0.z);
            glVertex3f(vertex_1.x, vertex_1.y, vertex_1.z);
            glVertex3f(vertex_2.x, vertex_2.y, vertex_2.z);

            glEnd();
        }

        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
    }
    else
    {
        this->_material->render(viewMatrix);

        for (const auto& face : _meshData.getFaces())
        {
            glBegin(GL_TRIANGLES);

            const glm::vec3 vertex_0 = _meshData.getVertices()[std::get<0>(face)];
            const glm::vec3 vertex_1 = _meshData.getVertices()[std::get<1>(face)];
            const glm::vec3 vertex_2 = _meshData.getVertices()[std::get<2>(face)];

            const glm::vec3 normal_0 = _meshData.getNormals()[std::get<0>(face)];
            const glm::vec3 normal_1 = _meshData.getNormals()[std::get<1>(face)];
            const glm::vec3 normal_2 = _meshData.getNormals()[std::get<2>(face)];

            const glm::vec2 uv_0 = _meshData.getUVs()[std::get<0>(face)];
            const glm::vec2 uv_1 = _meshData.getUVs()[std::get<1>(face)];
            const glm::vec2 uv_2 = _meshData.getUVs()[std::get<2>(face)];

            glTexCoord2f(uv_0.x, uv_0.y);
            glNormal3f(normal_0.x, normal_0.y, normal_0.z);
            glVertex3f(vertex_0.x, vertex_0.y, vertex_0.z);

            glTexCoord2f(uv_1.x, uv_1.y);
            glNormal3f(normal_1.x, normal_1.y, normal_1.z);
            glVertex3f(vertex_1.x, vertex_1.y, vertex_1.z);

            glTexCoord2f(uv_2.x, uv_2.y);
            glNormal3f(normal_2.x, normal_2.y, normal_2.z);
            glVertex3f(vertex_2.x, vertex_2.y, vertex_2.z);

            glEnd();
        }
        // Logica ombre per i dischi della Torre di Hanoi (Disk_1 ... Disk_7)

        if (getName().find("Disk_") != std::string::npos)
        {
            // Ottieni l'altezza corrente del disco (assumendo che getPosition() restituisca la pos locale/globale corretta)
            float currentY = this->getPosition().y;

            // Altezza approssimativa della base 'Fillet8' o del piano di gioco.
            // Regola questo valore se l'ombra appare troppo alta o penetra nel pavimento.
            // Se Disk_1 è appoggiato a Y=0, metti un valore appena superiore (es. 0.1f) per evitare z-fighting.
            float floorY = 5520.0f;

            // Trasla l'ombra verso il basso fino a raggiungere il livello della base.
            // La traslazione avviene nel sistema di riferimento dell'oggetto, quindi spostiamo
            // l'ombra di una quantità opposta all'altezza del disco rispetto al pavimento.
            glTranslatef(0.0f, -(currentY - floorY), 0.0f);

            // Schiaccia la mesh sull'asse Y per renderla piatta
            glScalef(1.0f, 0.0f, 1.0f);  
        }

        // Duplica la mesh per creare l'effetto ombra.
        glPushMatrix();

        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        // Imposta il colore nero per l'ombra.

        glDisable(GL_LIGHTING);

        // Renderizza la mesh come ombra.
        for (const auto& face : _meshData.getFaces())
        {
            glBegin(GL_TRIANGLES);

            const glm::vec3 vertex_0 = _meshData.getVertices()[std::get<0>(face)];
            const glm::vec3 vertex_1 = _meshData.getVertices()[std::get<1>(face)];
            const glm::vec3 vertex_2 = _meshData.getVertices()[std::get<2>(face)];

            glVertex3f(vertex_0.x, vertex_0.y, vertex_0.z);
            glVertex3f(vertex_1.x, vertex_1.y, vertex_1.z);
            glVertex3f(vertex_2.x, vertex_2.y, vertex_2.z);

            glEnd();
        }

        // Ripristina lo stato grafico.
        glEnable(GL_LIGHTING); // Riabilita l'illuminazione dopo aver renderizzato l'ombra.

        glPopMatrix();



    }
}