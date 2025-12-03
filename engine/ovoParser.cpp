#define _CRT_SECURE_NO_WARNINGS
/**
 * @file OvoParser.cpp
 * @brief Implementazione del parser OVO.
 */
#include "ovoParser.h"
#include <iostream>
#include <fstream>
#include <cstring> // per memcpy
#include <glm/gtc/type_ptr.hpp>

 // Includiamo le luci per poterle creare
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
// #include "Texture.h" // Commentato finché non riattiviamo le texture

OvoParser::OvoParser() {}
OvoParser::~OvoParser() {}

Node* OvoParser::loadFile(const std::string& filename)
{
    std::cout << "[OvoParser] Loading file: " << filename << "..." << std::endl;

    // 1. Apri il file in modalità binaria
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << filename << std::endl;
        return nullptr;
    }

    // 2. Leggi dimensione e dati
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

    // 3. Parsing
    unsigned int position = 0;

    // Crea un nodo radice fittizio per contenere tutto
    Node* root = new Node("Root_Ovo");

    // Loop principale sui chunk di primo livello
    while (position < (unsigned int)size) {
        // Qui ci aspettiamo una sequenza di oggetti top-level o il chunk HEADER iniziale

        // Peek al chunk ID senza avanzare troppo se non è gestito
        unsigned int chunkId;
        memcpy(&chunkId, buffer + position, sizeof(unsigned int));

        // Gestione speciale Header (non è un Node)
        if (chunkId == OVO_CHUNK_HEADER) {
            unsigned int chunkSize;
            memcpy(&chunkSize, buffer + position + 4, sizeof(unsigned int));
            unsigned int version;
            memcpy(&version, buffer + position + 8, sizeof(unsigned int));
            std::cout << "[OvoParser] Found Header. Version: " << version << std::endl;
            position += 8 + chunkSize; // Salta header + dati
            continue;
        }

        // Se è un oggetto, mesh, luce o materiale, lo parsiamo e lo attacchiamo alla radice
        Node* child = parseChunk(buffer, position, (unsigned int)size);
        if (child) {
            root->addChild(child);
        }
        else {
            // Se ritorna nullptr ma non siamo alla fine, c'è un problema o un chunk ignoto
            // Per evitare loop infiniti in caso di errore, incrementiamo manualmente o usciamo
            if (position >= (unsigned int)size) break;

            // Try to skip unknown chunk manually if possible, but parseChunk usually handles pos update.
            // Se parseChunk fallisce senza aggiornare la pos, siamo nei guai. 
            // Assumiamo che parseChunk aggiorni SEMPRE position.
        }
    }

    delete[] buffer;
    return root;
}

Node* OvoParser::parseChunk(char* data, unsigned int& position, unsigned int size)
{
    unsigned int chunkId;
    unsigned int chunkSize;

    if (position + 8 > size) return nullptr; // Safety check

    memcpy(&chunkId, data + position, sizeof(unsigned int));
    memcpy(&chunkSize, data + position + 4, sizeof(unsigned int));

    // Avanziamo l'header del chunk
    unsigned int startDataPos = position + 8;
    position += 8 + chunkSize; // La position finale dopo questo blocco

    // Cursore locale per i dati dentro il chunk
    unsigned int current = startDataPos;

    Node* newNode = nullptr;

    switch (chunkId)
    {
    case OVO_CHUNK_OBJECT:
    case OVO_CHUNK_MESH:
    case OVO_CHUNK_LIGHT:
    {
        // --- LEGGIAMO I DATI COMUNI DEI NODI ---
        unsigned int version;
        memcpy(&version, data + current, sizeof(unsigned int)); current += sizeof(unsigned int);

        // Nome
        char name[256]; // Assumiamo max 256 chars
        strcpy(name, data + current);
        current += (unsigned int)strlen(name) + 1;

        // Matrice (16 float)
        glm::mat4 matrix;
        memcpy(glm::value_ptr(matrix), data + current, sizeof(glm::mat4));
        current += sizeof(glm::mat4);

        // Nr Figli
        unsigned int nrChildren;
        memcpy(&nrChildren, data + current, sizeof(unsigned int));
        current += sizeof(unsigned int);

        // Target Node Name (opzionale, lo saltiamo per ora)
        char targetName[256];
        strcpy(targetName, data + current);
        current += (unsigned int)strlen(targetName) + 1;

        // Istanziamo la classe corretta
        if (chunkId == OVO_CHUNK_OBJECT) {
            newNode = new Node(name);
        }
        else if (chunkId == OVO_CHUNK_MESH) {
            // Leggiamo dati extra per Mesh: BBox Min/Max
            // (Li saltiamo perché Mesh li ricalcola o non li usa ancora)
            current += sizeof(glm::vec3) * 2;

            // Sub-chunks: Materiale e raggio
            unsigned char hasMaterial;
            memcpy(&hasMaterial, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);

            // Casting ombra
            unsigned char castShadows;
            memcpy(&castShadows, data + current, sizeof(unsigned char)); current += sizeof(unsigned char);

            // Raggio sfera avvolgente
            current += sizeof(float); // cullingRadius

            // Creiamo la mesh
            newNode = new Mesh(name);

            // Parsing Sub-Chunks della Mesh (Vertici, Facce, ecc.)
            // Questi sub-chunks sono DENTRO il payload del chunk MESH
            while (current < position) {
                unsigned int subId, subSize;
                memcpy(&subId, data + current, sizeof(unsigned int));
                memcpy(&subSize, data + current + 4, sizeof(unsigned int));

                unsigned int subDataStart = current + 8;

                Mesh* mesh = dynamic_cast<Mesh*>(newNode);

                if (subId == OVO_CHUNK_VERTICES) {
                    unsigned int nrVertices;
                    memcpy(&nrVertices, data + subDataStart, sizeof(unsigned int));

                    // I vertici iniziano dopo il contatore
                    const float* vData = (const float*)(data + subDataStart + sizeof(unsigned int));
                    for (unsigned int i = 0; i < nrVertices; i++) {
                        mesh->addVertex(glm::vec3(vData[i * 3], vData[i * 3 + 1], vData[i * 3 + 2]));
                    }
                }
                else if (subId == OVO_CHUNK_NORMALS) {
                    unsigned int nrNormals;
                    memcpy(&nrNormals, data + subDataStart, sizeof(unsigned int));
                    const float* nData = (const float*)(data + subDataStart + sizeof(unsigned int));
                    for (unsigned int i = 0; i < nrNormals; i++) {
                        mesh->addNormal(glm::vec3(nData[i * 3], nData[i * 3 + 1], nData[i * 3 + 2]));
                    }
                }
                else if (subId == OVO_CHUNK_FACES) {
                    unsigned int nrFaces;
                    memcpy(&nrFaces, data + subDataStart, sizeof(unsigned int));
                    const unsigned int* fData = (const unsigned int*)(data + subDataStart + sizeof(unsigned int));
                    for (unsigned int i = 0; i < nrFaces; i++) {
                        // OVO salva 3 indici per faccia
                        mesh->addFace(fData[i * 3], fData[i * 3 + 1], fData[i * 3 + 2]);
                    }
                }
                else if (subId == OVO_CHUNK_MATERIAL) {
                    // Materiale embedded nella mesh (caso semplice) o link
                    // Per ora gestiamo materiali separati
                }

                current += 8 + subSize;
            }
        }
        else if (chunkId == OVO_CHUNK_LIGHT) {
            // Parsing parametri luce...
            // Per semplicità creiamo una PointLight generica, 
            // in futuro leggi il sottotipo (Directional/Spot) dai dati
            newNode = new PointLight(name);
            // Qui dovresti leggere colore, raggio, ecc. saltiamo per brevità
        }

        // Imposta la matrice letta
        if (newNode) newNode->setMatrix(matrix);

        // --- RICORSIONE PER I FIGLI ---
        // I figli sono scritti SEQUENZIALMENTE nel file DOPO questo chunk.
        // Ma OvoReader classico li annida?
        // Se 'nrChildren' > 0, dobbiamo chiamare parseChunk N volte.
        // ATTENZIONE: In molti formati OVO flat, i figli sono chunk successivi.
        // Nel loop while del metodo loadFile, stiamo parsando linearmente.

        // Se il formato prevede che i figli siano "dopo" il padre ma allo stesso livello logico di file,
        // dobbiamo attaccarli.
        // Tuttavia, l'implementazione base di solito crea l'albero.
        // Qui ci fermiamo: Node creato. I figli verranno aggiunti se il chiamante gestisce la gerarchia.

        // Fix logica ricorsiva:
        // Poiché 'position' è stato avanzato ALLA FINE di questo chunk, 
        // se i figli sono DENTRO questo chunk (nested binary), li avremmo già parsati nel while interno.
        // Se sono FUORI (dopo), dobbiamo sapere che i prossimi N chunk sono figli di questo.

        for (unsigned int i = 0; i < nrChildren; i++) {
            Node* childNode = parseChunk(data, position, size);
            if (newNode && childNode) {
                newNode->addChild(childNode);
            }
        }
    }
    break;

    case OVO_CHUNK_MATERIAL:
    {
        // Creazione materiale
        char name[256];
        strcpy(name, data + current);
        current += (unsigned int)strlen(name) + 1;

        Material* mat = new Material(name);

        // Leggi colori
        glm::vec3 emission, albedo;
        float roughness, metalness, alpha;

        memcpy(glm::value_ptr(emission), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);
        memcpy(glm::value_ptr(albedo), data + current, sizeof(glm::vec3)); current += sizeof(glm::vec3);
        memcpy(&roughness, data + current, sizeof(float)); current += sizeof(float);
        memcpy(&metalness, data + current, sizeof(float)); current += sizeof(float);
        memcpy(&alpha, data + current, sizeof(float)); current += sizeof(float);

        // Converti parametri PBR in Phong (approssimazione per il tuo engine)
        mat->setEmission(glm::vec4(emission, 1.0f));
        mat->setDiffuse(glm::vec4(albedo, alpha));
        mat->setSpecular(glm::vec4(metalness, metalness, metalness, 1.0f)); // Metallo = speculare alto
        mat->setShininess((1.0f - roughness) * 128.0f);

        // Texture Name
        char texName[256];
        strcpy(texName, data + current);

        // Se c'è una texture, stampiamo (per ora disabilitata)
        if (strlen(texName) > 0) {
            std::cout << "[OvoParser] Material '" << name << "' uses texture: " << texName << " (Loading disabled)" << std::endl;
            // Qui in futuro: mat->setTexture(new Texture(texName));
        }

        // In un sistema reale, dovremmo salvare questo materiale in una mappa globale 
        // per poi assegnarlo alle mesh che lo richiedono.
        // Per ora ritorniamo il materiale come se fosse un nodo (hack) o nullptr.
        // Poiché Material eredita da Object ma non da Node, non possiamo ritornarlo qui direttamente
        // se la firma è Node*.
        // Soluzione: Salviamo il materiale in una lista statica/globale o lo attacchiamo all'ultimo nodo creato.

        // PER ORA: Leakiamo il materiale (non lo usiamo) o lo stampiamo e basta.
        delete mat;
        return nullptr;
    }
    break;

    default:
        std::cout << "[OvoParser] Unknown chunk ID: " << std::hex << chunkId << std::dec << std::endl;
        return nullptr;
    }

    return newNode;
}