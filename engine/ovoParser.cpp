/**
 * @file OvoParser.cpp
 * @brief Implementazione OVO Parser che supporta Object3D e Debugging avanzato.
 */
#define _CRT_SECURE_NO_WARNINGS

#include "ovoParser.h"
#include <iostream>
#include <fstream>
#include <cstring> 
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/packing.hpp>

#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "prospectiveCamera.h" 

 // Struttura per saltare i dati fisici
struct PhysProps {
    unsigned char type;
    unsigned char contCollisionDetection;
    unsigned char collideWithRBodies;
    unsigned char hullType;
    glm::vec3 massCenter;
    float mass;
    float staticFriction;
    float dynamicFriction;
    float bounciness;
    float linearDamping;
    float angularDamping;
    unsigned int nrOfHulls;
    unsigned int _pad;
    void* physObj;
    void* hull;
};

OvoParser::OvoParser() {}
OvoParser::~OvoParser() {}

Node* OvoParser::loadFile(const std::string& filename)
{
    std::cout << "[OvoParser] Opening file: " << filename << std::endl;

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return nullptr;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0) {
        std::cerr << "[ERROR] File empty." << std::endl;
        return nullptr;
    }

    char* buffer = new char[size];
    if (!file.read(buffer, size)) {
        std::cerr << "[ERROR] Failed to read file." << std::endl;
        delete[] buffer;
        return nullptr;
    }
    file.close();

    unsigned int position = 0;
    Node* root = new Node("Root_Ovo");

    // Loop sui chunk di primo livello
    while (position < (unsigned int)size) {
        // Passiamo position per riferimento, verrà aggiornata alla fine del chunk
        Node* child = parseChunk(buffer, position, (unsigned int)size);

        // Se il parser ritorna un nodo valido, lo aggiungiamo
        // Se ritorna nullptr (es. Header o Material), continuiamo col prossimo
        if (child) {
            root->addChild(child);
        }
    }

    delete[] buffer;
    return root;
}

Node* OvoParser::parseChunk(char* data, unsigned int& position, unsigned int size)
{
    if (position >= size) return nullptr;

    unsigned int chunkId;
    unsigned int chunkSize;

    memcpy(&chunkId, data + position, sizeof(unsigned int));
    memcpy(&chunkSize, data + position + 4, sizeof(unsigned int));

    // Calcoliamo dove finisce questo chunk
    unsigned int endPos = position + 8 + chunkSize;
    unsigned int current = position + 8;

    // Aggiorniamo la position globale per il prossimo giro (il chiamante userà questo valore)
    position = endPos;

    // --- DEBUG LOG ---
    // Decommenta questa riga se vuoi vedere TUTTI i chunk passati
    std::cout << "   [DEBUG] Chunk Found. ID: " << chunkId << " | Size: " << chunkSize << std::endl;

    Node* newNode = nullptr;

    // 1. HEADER
    if (chunkId == OVO_CHUNK_OBJECT) {
        unsigned int version;
        memcpy(&version, data + current, sizeof(unsigned int));
        std::cout << "[OvoParser] Header. Version: " << version << std::endl;
        return nullptr;
    }

    // 2. MATERIAL
    if (chunkId == OVO_CHUNK_MATERIAL) {
        char name[FILENAME_MAX];
        strcpy(name, data + current);
        // std::cout << "[OvoParser] Skipping Material: " << name << std::endl;
        return nullptr;
    }

    // 3. NODI STANDARD E ESTESI (NODE, MESH, LIGHT, CAMERA, BONE, OBJECT3D)
    // In OVO, NODE (1), OBJECT3D (3) e le entità specifiche condividono spesso l'intestazione:
    // [Name][Matrix][NrChildren][TargetName]

    bool isStandardNode = (chunkId == OVO_CHUNK_NODE ||
        chunkId == OVO_CHUNK_OBJECT3D || // <-- Aggiunto ID 3
        chunkId == OVO_CHUNK_MESH ||
        chunkId == OVO_CHUNK_LIGHT ||
        chunkId == OVO_CHUNK_CAMERA ||
        chunkId == OVO_CHUNK_BONE ||
        chunkId == OVO_CHUNK_SKINNED);

    if (isStandardNode) {
        char name[FILENAME_MAX];
        glm::mat4 matrix;
        unsigned int nrChildren = 0;
        char targetName[FILENAME_MAX];

        // Lettura Dati Comuni
        strcpy(name, data + current); current += (unsigned int)strlen(name) + 1;
        memcpy(glm::value_ptr(matrix), data + current, sizeof(glm::mat4)); current += sizeof(glm::mat4);
        memcpy(&nrChildren, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
        strcpy(targetName, data + current); current += (unsigned int)strlen(targetName) + 1;

        // Dispatch per tipo
        if (chunkId == OVO_CHUNK_NODE || chunkId == OVO_CHUNK_OBJECT3D) {
            newNode = new Node(name);
            std::cout << "[OvoParser] Node/Obj3D loaded: " << name << " (Children: " << nrChildren << ")" << std::endl;
        }
        else if (chunkId == OVO_CHUNK_MESH || chunkId == OVO_CHUNK_SKINNED) {
            // ... [Dati Mesh uguali a prima] ...
            unsigned char subtype; memcpy(&subtype, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);
            char matName[FILENAME_MAX]; strcpy(matName, data + current); current += (unsigned int)strlen(matName) + 1;
            float radius; memcpy(&radius, data + current, sizeof(float)); current += sizeof(float);
            glm::vec3 bBoxMin, bBoxMax;
            memcpy(glm::value_ptr(bBoxMin), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);
            memcpy(glm::value_ptr(bBoxMax), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);

            unsigned char hasPhysics; memcpy(&hasPhysics, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);
            if (hasPhysics) {
                PhysProps mp; memcpy(&mp, data + current, sizeof(PhysProps)); current += sizeof(PhysProps);
                if (mp.nrOfHulls) {
                    for (unsigned int c = 0; c < mp.nrOfHulls; c++) {
                        unsigned int nV, nF;
                        memcpy(&nV, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
                        memcpy(&nF, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
                        current += sizeof(glm::vec3) + nV * sizeof(glm::vec3) + nF * sizeof(unsigned int) * 3;
                    }
                }
            }

            Mesh* mesh = new Mesh(name);
            newNode = mesh;
            //mesh->setMaterial(new Material(matName)); // Placeholder material (COMMENTATO PERCHé NON HO ANCORA I MATERIALI E TEXTURE
            Material* tempMat = new Material(matName);
            tempMat->setAmbient(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f));  // Grigio scuro
            tempMat->setDiffuse(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));  // Grigio chiaro
            tempMat->setSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)); // Poco speculare
            tempMat->setShininess(10.0f);

            mesh->setMaterial(tempMat);


            unsigned int LODs; memcpy(&LODs, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
            long totalVerts = 0;

            for (unsigned int l = 0; l < LODs; l++) {
                unsigned int nVerts, nFaces;
                memcpy(&nVerts, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
                memcpy(&nFaces, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);

                bool loadData = (l == 0);

                for (unsigned int v = 0; v < nVerts; v++) {
                    glm::vec3 pos;
                    unsigned int normData, texData, tanData;
                    memcpy(glm::value_ptr(pos), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);
                    memcpy(&normData, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
                    memcpy(&texData, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
                    memcpy(&tanData, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);

                    if (loadData) {
                        mesh->addVertex(pos);
                        glm::vec4 norm = glm::unpackSnorm3x10_1x2(normData);
                        mesh->addNormal(glm::vec3(norm));
                        // UV...
                    }
                }
                for (unsigned int f = 0; f < nFaces; f++) {
                    unsigned int face[3];
                    // Leggiamo i 3 indici dal file (v1, v2, v3)
                    memcpy(face, data + current, sizeof(unsigned int) * 3);
                    current += sizeof(unsigned int) * 3;

                    if (loadData) {
                        // FIX VISUALIZZAZIONE:
                        // Mesh::addFace(v, n, t) aggiunge UN SOLO vertice alla buffer list.
                        // Per disegnare un triangolo con glBegin(GL_TRIANGLES), dobbiamo inviare 3 vertici.
                        // Poiché OVO usa vertici "interleaved" (stesso indice per pos, norm, uv),
                        // usiamo lo stesso indice per tutti e tre i canali.

                        // 1° Vertice del triangolo
                        mesh->addFace(face[0], face[0], face[0]);

                        // 2° Vertice del triangolo
                        mesh->addFace(face[1], face[1], face[1]);

                        // 3° Vertice del triangolo
                        mesh->addFace(face[2], face[2], face[2]);
                    }
                }
                if (loadData) totalVerts = nVerts;
            }
            std::cout << "[OvoParser] MESH: " << name << " | Verts: " << totalVerts << std::endl;

            // Skip Skinned Data
            if (chunkId == OVO_CHUNK_SKINNED) {
                current += sizeof(glm::mat4); // Pose
                unsigned int nrBones; memcpy(&nrBones, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);
                for (unsigned int b = 0; b < nrBones; b++) {
                    char bName[FILENAME_MAX]; strcpy(bName, data + current); current += (unsigned int)strlen(bName) + 1;
                    current += sizeof(glm::mat4);
                }
                // Weights... difficile saltare senza logica complessa.
                // Se crasha qui, sappiamo perché.
            }
        }
        else if (chunkId == OVO_CHUNK_LIGHT) {
            // Skip light data...
            unsigned char subtype; memcpy(&subtype, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);
            current += sizeof(glm::vec3) + sizeof(float) + sizeof(glm::vec3) + sizeof(float) + sizeof(float) + 2;
            newNode = new PointLight(name);
            ((PointLight*)newNode)->setConstantAttenuation(0.5f); // Hack visibilità
            std::cout << "[OvoParser] LIGHT: " << name << std::endl;
        }
        else if (chunkId == OVO_CHUNK_CAMERA) {
            // FIX: PerspectiveCamera richiede 5 argomenti. 
            // Usiamo valori standard (60 gradi FOV, 16:9 aspect, 0.1 near, 1000 far)
            newNode = new PerspectiveCamera(name, 60.0f, 1.77f, 0.1f, 1000.0f);
            std::cout << "[OvoParser] CAMERA: " << name << std::endl;
        }
        else if (chunkId == OVO_CHUNK_BONE) {
            current += sizeof(glm::vec3) * 2; // BBox
            newNode = new Node(name);
        }

        if (newNode) newNode->setMatrix(matrix);

        // --- RICORSIONE FIGLI ---
        // I figli sono scritti nel file DOPO i dati del padre.
        // Essendo parseChunk che avanza 'position', basta chiamarlo N volte.
        for (unsigned int i = 0; i < nrChildren; i++) {
            Node* child = parseChunk(data, position, size);
            if (newNode && child) {
                newNode->addChild(child);
            }
        }
    }
    else {
        // Chunk ID sconosciuto o non gestito (es. LIST se ha struttura diversa)
        std::cerr << "[OvoParser] WARNING: Unhandled Chunk ID: " << chunkId << " (Size: " << chunkSize << "). Skipping." << std::endl;
        // La posizione è già stata aggiornata a endPos all'inizio, quindi lo saltiamo e basta.
    }

    return newNode;
}