/**
 * @file OvoParser.cpp
 * @brief Implementazione del parser OVO (Corretta con BBox).
 */
#define _CRT_SECURE_NO_WARNINGS // Zittisce warning strcpy

#include "ovoParser.h"
#include <iostream>
#include <fstream>
#include <cstring> 
#include <glm/gtc/type_ptr.hpp>

 // Includiamo le classi dell'engine
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "prospectiveCamera.h" 
// #include "Texture.h" 

OvoParser::OvoParser() {}
OvoParser::~OvoParser() {}

Node* OvoParser::loadFile(const std::string& filename)
{
    std::cout << "[OvoParser] Loading file: " << filename << "..." << std::endl;

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return nullptr;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0) {
        std::cerr << "[ERROR] Empty file." << std::endl;
        return nullptr;
    }

    char* buffer = new char[size];
    if (!file.read(buffer, size)) {
        std::cerr << "[ERROR] Failed to read file content." << std::endl;
        delete[] buffer;
        return nullptr;
    }
    file.close();

    unsigned int position = 0;
    Node* root = new Node("Root_Ovo");

    while (position < (unsigned int)size) {
        unsigned int chunkId;
        memcpy(&chunkId, buffer + position, sizeof(unsigned int));

        if (chunkId == OVO_CHUNK_HEADER) {
            unsigned int chunkSize;
            memcpy(&chunkSize, buffer + position + 4, sizeof(unsigned int));
            unsigned int version;
            memcpy(&version, buffer + position + 8, sizeof(unsigned int));
            std::cout << "[OvoParser] Found Header. Version: " << version << std::endl;
            position += 8 + chunkSize;
            continue;
        }

        Node* child = parseChunk(buffer, position, (unsigned int)size);
        if (child) {
            root->addChild(child);
        }
        else {
            if (position >= (unsigned int)size) break;
        }
    }

    delete[] buffer;
    return root;
}

Node* OvoParser::parseChunk(char* data, unsigned int& position, unsigned int size)
{
    unsigned int chunkId;
    unsigned int chunkSize;

    if (position + 8 > size) return nullptr;

    memcpy(&chunkId, data + position, sizeof(unsigned int));
    memcpy(&chunkSize, data + position + 4, sizeof(unsigned int));

    unsigned int startDataPos = position + 8;
    unsigned int endPos = position + 8 + chunkSize; // Salviamo la fine del chunk
    position = endPos; // Aggiorniamo subito la posizione globale per il prossimo giro

    unsigned int current = startDataPos;
    Node* newNode = nullptr;

    // --- MATERIALI (Gestiti a parte) ---
    if (chunkId == OVO_CHUNK_MATERIAL) {
        char name[256];
        strcpy(name, data + current);
        current += (unsigned int)strlen(name) + 1;

        Material* mat = new Material(name);

        glm::vec3 emission, albedo;
        float roughness, metalness, alpha;

        memcpy(glm::value_ptr(emission), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);
        memcpy(glm::value_ptr(albedo), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);
        memcpy(&roughness, data + current, sizeof(float)); current += sizeof(float);
        memcpy(&metalness, data + current, sizeof(float)); current += sizeof(float);
        memcpy(&alpha, data + current, sizeof(float)); current += sizeof(float);

        mat->setEmission(glm::vec4(emission, 1.0f));
        mat->setDiffuse(glm::vec4(albedo, alpha));
        mat->setSpecular(glm::vec4(metalness, metalness, metalness, 1.0f));
        mat->setShininess((1.0f - roughness) * 128.0f);

        std::cout << "[OvoParser] Loaded Material: " << name << std::endl;

        // In un engine vero salveremmo il materiale in una lista globale.
        // Qui lo "perdiamo" (leak) o lo ritorniamo come nodo finto solo per debug, 
        // ma Node* non è compatibile con Material*. 
        // Per ora ritorniamo nullptr (ma il materiale è creato).
        // TODO: Gestire Material Manager.
        return nullptr;
    }

    // --- NODI (Object, Mesh, Light, Camera) ---

    // 1. Dati Comuni a tutti i nodi
    unsigned int version;
    memcpy(&version, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);

    char name[256];
    strcpy(name, data + current);
    current += (unsigned int)strlen(name) + 1;

    glm::mat4 matrix;
    memcpy(glm::value_ptr(matrix), data + current, sizeof(glm::mat4));
    current += sizeof(glm::mat4);

    unsigned int nrChildren;
    memcpy(&nrChildren, data + current, sizeof(unsigned int));
    current += sizeof(unsigned int);

    char targetName[256];
    strcpy(targetName, data + current);
    current += (unsigned int)strlen(targetName) + 1;

    // --- [FIX CRITICO] LETTURA BOUNDING BOX ---
    // Questo mancava e faceva sfasare tutto!
    glm::vec3 bBoxMin, bBoxMax;
    memcpy(glm::value_ptr(bBoxMin), data + current, sizeof(glm::vec3));
    current += sizeof(glm::vec3);
    memcpy(glm::value_ptr(bBoxMax), data + current, sizeof(glm::vec3));
    current += sizeof(glm::vec3);
    // ------------------------------------------


    // 2. Creazione Istanza Specifica
    if (chunkId == OVO_CHUNK_OBJECT) {
        newNode = new Node(name);
    }
    else if (chunkId == OVO_CHUNK_MESH) {
        // Dati specifici Mesh
        unsigned char hasMaterial; // (in realtà è subtype nei vecchi ovo, ma spesso è bool)
        unsigned char castShadows;

        memcpy(&hasMaterial, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);
        memcpy(&castShadows, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);

        // Culling radius (float)
        // Attenzione: Ovoreader saltava direttamente ai subchunks? Controlliamo.
        // Solitamente c'è un float cullingRadius. Se non c'è, il while sotto lo gestisce.
        // Proviamo a leggere il raggio.
        float cullingRadius;
        memcpy(&cullingRadius, data + current, sizeof(float)); current += sizeof(float);

        Mesh* mesh = new Mesh(name);
        newNode = mesh;

        // Parsing Sub-Chunks (Vertici, Facce...)
        // Continuiamo finché non finisce il chunk corrente
        unsigned int subTagsStart = current;

        // Loop sui sub-chunks finché non raggiungiamo la fine del chunk MESH
        while (current < endPos) {
            unsigned int subId, subSize;
            memcpy(&subId, data + current, sizeof(unsigned int));
            memcpy(&subSize, data + current + 4, sizeof(unsigned int));

            unsigned int subDataPtr = current + 8;

            if (subId == OVO_CHUNK_VERTICES) {
                unsigned int nrVertices;
                memcpy(&nrVertices, data + subDataPtr, sizeof(unsigned int));
                const float* vData = (const float*)(data + subDataPtr + sizeof(unsigned int));
                for (unsigned int i = 0; i < nrVertices; i++)
                    mesh->addVertex(glm::vec3(vData[i * 3], vData[i * 3 + 1], vData[i * 3 + 2]));
            }
            else if (subId == OVO_CHUNK_NORMALS) {
                unsigned int nrNormals;
                memcpy(&nrNormals, data + subDataPtr, sizeof(unsigned int));
                const float* nData = (const float*)(data + subDataPtr + sizeof(unsigned int));
                for (unsigned int i = 0; i < nrNormals; i++)
                    mesh->addNormal(glm::vec3(nData[i * 3], nData[i * 3 + 1], nData[i * 3 + 2]));
            }
            else if (subId == OVO_CHUNK_FACES) {
                unsigned int nrFaces;
                memcpy(&nrFaces, data + subDataPtr, sizeof(unsigned int));
                const unsigned int* fData = (const unsigned int*)(data + subDataPtr + sizeof(unsigned int));
                for (unsigned int i = 0; i < nrFaces; i++)
                    mesh->addFace(fData[i * 3], fData[i * 3 + 1], fData[i * 3 + 2]);
            }
            // Texture coords? (OVO_CHUNK_TEXCOORDS)
            else if (subId == OVO_CHUNK_TEXCOORDS) {
                unsigned int nrTex;
                memcpy(&nrTex, data + subDataPtr, sizeof(unsigned int));
                const float* tData = (const float*)(data + subDataPtr + sizeof(unsigned int));
                for (unsigned int i = 0; i < nrTex; i++)
                    mesh->addTexCoord(glm::vec2(tData[i * 2], tData[i * 2 + 1]));
            }

            current += 8 + subSize;
        }
    }
    else if (chunkId == OVO_CHUNK_LIGHT) {
        // Luci
        // Leggiamo il sottotipo (enum)
        // In OVOReader standard: lightType (short), color (vec3), radius (float)...

        // Per ora creiamo una Omni generica per non bloccarci
        newNode = new PointLight(name);

        // TODO: Leggere i dati specifici della luce (tipo, colore, ecc)
        // current += ... 
    }

    // Se abbiamo creato il nodo, impostiamo la matrice
    if (newNode) {
        newNode->setMatrix(matrix);
    }

    // Gestione Figli
    // In OVO, i figli sono chunk successivi. 
    // Il metodo loadFile principale gestisce la sequenza piatta,
    // ma dobbiamo sapere che i prossimi 'nrChildren' chunk sono figli di questo.
    // Questa logica ricorsiva complessa va gestita meglio nel main loop,
    // ma per una scena piatta (senza troppa gerarchia) questo approccio "flat" nel loadFile va bene
    // perché parseChunk ritorna il nodo e noi lo attacchiamo alla radice.

    // NOTA: Se la scena ha gerarchia vera, dovremmo chiamare parseChunk ricorsivamente qui.
    for (unsigned int i = 0; i < nrChildren; i++) {
        Node* child = parseChunk(data, position, size); // position viene aggiornato da parseChunk
        if (newNode && child) newNode->addChild(child);
    }

    return newNode;
}