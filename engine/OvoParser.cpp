#include "OvoParser.h"

#include <stack>
#include <glm/ext.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#pragma warning(disable:4996) // Disable Visual Studio warning

// Memorizza materiali durante il parsing del file.
std::unordered_map<std::string, std::shared_ptr<Material>> OVOParser::materials;

/**
 * @brief Converte i dati byte in una stringa C++.
 *
 * Questo metodo analizza un buffer di dati byte e lo converte in una stringa C++ terminata con il carattere null (0x00).
 * La funzione scorre i byte fino a trovare il terminatore null e costruisce la stringa a partire dai byte letti.
 *
 * @param data Puntatore ai dati byte da analizzare.
 *
 * @return La stringa convertita dai dati byte.
 */
std::string LIB_API OVOParser::parseString(const uint8_t* data)
{
    std::string string;

    uint32_t pointer = 0;

    // Fino a che non trova il terminatore null (0x00).
    while (data[pointer] != 0x00)
    {
        // Converte il byte corrente in un carattere.
        const char character = (char)data[pointer];

        // Aggiunge il carattere alla stringa.
        string += character;

        // Incrementa il puntatore per passare al byte successivo.
        ++pointer;
    }
    return string;
}

/**
 * @brief Carica una scena da un file .ovo e restituisce il nodo root della scena.
 *
 * Questo metodo apre un file .ovo specificato dal percorso `filePath`, elabora i dati contenuti nel file
 * e costruisce una gerarchia di nodi che rappresenta la scena. I dati vengono letti in chunk, ognuno dei quali
 * rappresenta un diverso tipo di oggetto nella scena (nodi, materiali, luci e mesh). La gerarchia dei nodi
 * viene costruita utilizzando uno stack per gestire i nodi e il loro conteggio di figli rimanenti.
 *
 * @param filePath Il percorso del file .ovo da cui caricare la scena.
 *
 * @return Il nodo root della scena costruita.
 *
 * @note La funzione gestisce diversi tipi di chunk:
 * - Tipo 0: Versione del file
 * - Tipo 1: Node della scena
 * - Tipo 9: Materiale
 * - Tipo 16: Luce
 * - Tipo 18: Mesh
 */
std::shared_ptr<Node> LIB_API OVOParser::fromFile(const std::string filePath)
{
    // Pulisce la mappa dei materiali.
    OVOParser::materials.clear();

    // Apre il file in modalit   binaria.
    FILE* file = fopen(filePath.c_str(), "rb");

    if (file == nullptr)
        ERROR("Failed to read file \"" + filePath + "\".");

    DEBUG("Loading file \"" << filePath.c_str() << "\" ...");

    // Inizializza uno stack per gestire la gerarchia dei nodi --> Node, Number of Children
    std::stack<std::pair<std::shared_ptr<Node>, uint32_t>> hierarchy;

    // Crea il nodo root della scena.
    std::shared_ptr<Node> sceneRoot = std::make_shared<Node>();
    sceneRoot->setName("Scene Root");
    hierarchy.push(std::make_pair(sceneRoot, 1));

    // Inizia a leggere e processare i chunk del file.
    while (true)
    {
        uint32_t chunkType;
        uint32_t chunkSize;

        // fread: legge dati da un file e li memorizza in una variabile.
        // Legge il tipo e la dimensione del chunk.
        fread(&chunkType, sizeof(uint32_t), 1, file);
        if (feof(file))
            break;
        fread(&chunkSize, sizeof(uint32_t), 1, file);

        // Alloca memoria per i dati del chunk e legge i dati.
        uint8_t* chunkData = new uint8_t[chunkSize];
        fread(chunkData, sizeof(uint8_t), chunkSize, file);

        DEBUG("Reading chunk: Type=" << chunkType << ", Size=" << chunkSize);


        // Gestisce i chunk in base al loro tipo.
        if (chunkType == 0) // Version
        {
            uint32_t version;

            // memcpy: copia un blocco di memoria da una sorgente a una destinazione.
            // copia da chunkData a version
            memcpy(&version, chunkData, sizeof(uint32_t));
            DEBUG("Version: " << version)
        }
        else if (chunkType == 1) // Node
        {
            // Parsa i dati del chunk per ottenere un nuovo nodo e il numero di figli.
            const std::pair<std::shared_ptr<Node>, uint32_t> ret = OVOParser::parseNodeChunk(chunkData, chunkSize);
            // Ottiene il nodo corrente dalla cima della gerarchia.
            auto& top = hierarchy.top();
            // Aggiunge il nuovo nodo come figlio del nodo corrente nella gerarchia.
            top.first->addChild(ret.first);
            // Decrementa il contatore dei figli rimasti da aggiungere per il nodo corrente.
            --top.second;
            // Verifica che il contatore dei figli rimasti non sia negativo.
            assert(top.second >= 0);
            // Aggiunge il nuovo nodo e il suo contatore dei figli alla gerarchia.
            hierarchy.push(ret);
        }
        else if (chunkType == 9) // Material
        {
            const std::pair<std::shared_ptr<Material>, std::string> ret = OVOParser::parseMaterialChunk(chunkData, chunkSize);
            const std::shared_ptr<Material> material = ret.first;
            const std::string materialName = ret.second;
            OVOParser::materials[materialName] = material;
        }
        else if (chunkType == 16) // Light
        {
            const std::pair<std::shared_ptr<Light>, uint32_t> ret = OVOParser::parseLightChunk(chunkData, chunkSize);
            auto& top = hierarchy.top();
            top.first->addChild(ret.first);
            --top.second;
            assert(top.second >= 0);
            hierarchy.push(ret);
        }
        else if (chunkType == 18) // Mesh
        {
            const std::pair<std::shared_ptr<Mesh>, uint32_t> ret = OVOParser::parseMeshChunk(chunkData, chunkSize);
            auto& top = hierarchy.top();
            top.first->addChild(ret.first);
            --top.second;
            assert(top.second >= 0);
            hierarchy.push(ret);
        }
        else
        {
            WARNING("Unsupported chunk ID " << chunkType);
        }

        // Rimuove i nodi dallo stack una volta che tutti i figli sono stati processati.
        while (hierarchy.size() > 0 && hierarchy.top().second == 0)
        {
            hierarchy.pop();
        }

        // Dealloca la memoria allocata per chunk_data.
        delete[] chunkData;
    }

    // Chiude il file.
    fclose(file);

    return sceneRoot;
}

/**
 * @brief Analizza un chunk di byte e lo converte in un oggetto Node.
 *
 * Questa funzione estrae i dati dal chunk fornito e li utilizza per creare un oggetto `Node`
 * popolato con le informazioni lette. Il chunk di byte contiene il nome del nodo, una matrice
 * di trasformazione e il numero di figli che il nodo ha.
 *
 * @param chunkData I dati del chunk da analizzare.
 * @param chunkSize La dimensione del chunk.
 *
 * @return Una coppia (`std::pair`) contenente:
 * - `std::shared_ptr<Node>`: Puntatore al nodo creato e configurato con i dati estratti.
 * - `uint32_t`: Numero di figli rimanenti che devono essere elaborati.
 *
 */
std::pair<std::shared_ptr<Node>, uint32_t> LIB_API OVOParser::parseNodeChunk(const uint8_t* chunkData, const uint32_t chunkSize)
{
    // Verr   popolato con i dati dal chunk.
    std::shared_ptr<Node> node = std::make_shared<Node>();

    // Tiene traccia della posizione corrente nel chunk.
    uint32_t chunkPointer = 0;

    // Parse name
    {
        // Estrae una stringa dal chunk, che rappresenta il nome del nodo.
        const std::string nodeName = OVOParser::parseString(chunkData + chunkPointer);
        // Avanza oltre la stringa del nome del nodo.
        // Aggiunge 1 per considerare il carattere di terminazione null (0x00).
        chunkPointer += static_cast<uint32_t>(nodeName.length() + 1);
        node->setName(nodeName);
    }

    // Parse matrix
    {
        glm::mat4 matrix;
        // Copiare sizeof(glm::mat4) byte.
        memcpy(&matrix, chunkData + chunkPointer, sizeof(glm::mat4));
        chunkPointer += sizeof(glm::mat4);
        // Imposta la matrice di base del nodo con la matrice estratta dal chunk di dati.
        node->setBaseMatrix(matrix);
    }

    // Parse number of children
    uint32_t numberOfChildren;
    {
        memcpy(&numberOfChildren, chunkData + chunkPointer, sizeof(uint32_t));
        chunkPointer += sizeof(uint32_t);
    }

    return std::make_pair(node, numberOfChildren);
}

/**
 * @brief Analizza un chunk di byte e lo converte in un oggetto Mesh.
 *
 * Questa funzione estrae i dati dal chunk fornito e li utilizza per creare un oggetto `Mesh`
 * popolato con le informazioni lette. Il chunk di byte contiene il nome della mesh, una matrice
 * di trasformazione, il numero di figli, il nome del nodo di destinazione, il sottotipo della mesh,
 * il nome del materiale, e diversi dati relativi alla geometria e alla fisica della mesh.
 *
 * @param chunkData I dati del chunk da analizzare.
 * @param chunkSize La dimensione del chunk.
 *
 * @return Una coppia (`std::pair`) contenente:
 * - `std::shared_ptr<Mesh>`: Puntatore alla mesh creata e configurata con i dati estratti.
 * - `uint32_t`: Numero di figli rimanenti che devono essere elaborati.
 */
std::pair<std::shared_ptr<Mesh>, uint32_t> LIB_API OVOParser::parseMeshChunk(const uint8_t* chunkData, const uint32_t chunkSize)
{
    // Sar  popolata con i dati dal chunk.
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    MeshData meshData;

    // Tiene traccia della posizione corrente nel chunk.
    uint32_t chunkPointer = 0;

    // Name
    {
        // Estrae una stringa dal chunk, che    il nome del mesh.
        const std::string meshName = OVOParser::parseString(chunkData + chunkPointer);
        chunkPointer += static_cast<uint32_t>(meshName.length() + 1);
        mesh->setName(meshName);
    }

    // Matrix
    {
        glm::mat4 matrix;
        // Copia esattamente sizeof(glm::mat4) byte.
        memcpy(&matrix, chunkData + chunkPointer, sizeof(glm::mat4));
        chunkPointer += sizeof(glm::mat4);
        mesh->setBaseMatrix(matrix);

    }

    // Number of children
    uint32_t numberOfChildren;
    {
        // Copia il numero di figli dalla posizione corrente del chunk nella variabile number_of_children.
        memcpy(&numberOfChildren, chunkData + chunkPointer, sizeof(uint32_t));
        // Aggiorna chunk_pointer per avanzare oltre il numero di figli nel chunk di dati.
        chunkPointer += sizeof(uint32_t);
    }

    // Target node
    // La stringa viene ignorata, ma il puntatore viene aggiornato.
    chunkPointer += static_cast<uint32_t>(OVOParser::parseString(chunkData + chunkPointer).length() + 1);

    // Mesh sub-type // Ignorato
    chunkPointer += sizeof(uint8_t);

    // Parse material name
    {
        // Analizza il nome del materiale
        const std::string materialName = OVOParser::parseString(chunkData + chunkPointer);
        chunkPointer += static_cast<uint32_t>(materialName.length() + 1);

        // Se non c'  
        if (materialName == "[none]")
        {
            // Leave the default material.
        }
        // Verifica se il materiale non    stato trovato nella mappa di materiali
        else if (OVOParser::materials.find(materialName) == OVOParser::materials.end())
        {
            WARNING("Out-of-order material loading is not supported.");
        }
        else
        {
            // Imposta il materiale della mesh con il materiale trovato nella mappa.
            mesh->setMaterial(OVOParser::materials[materialName]);
        }
    }

    // Mesh radius size // Ignorato
    chunkPointer += sizeof(float);

    // Mesh bounding box minimum corner // Ignorato
    chunkPointer += sizeof(glm::vec3);

    // Mesh bounding box maximum corner // Ignorato
    chunkPointer += sizeof(glm::vec3);

    // Physics
    {
        // Determina se la mesh ha dati di fisica associati.
        uint8_t hasPhysics;
        // Copia il dato di fisica dalla posizione corrente del chunk nella variabile has_physics.
        memcpy(&hasPhysics, chunkData + chunkPointer, sizeof(uint8_t));
        chunkPointer += sizeof(uint8_t);

        // Verifica se la mesh ha dati di fisica associati.
        if (hasPhysics)
        {
            // Avanza il chunk_pointer per saltare 40 byte di dati (relativi alla fisica)
            chunkPointer += 40;

            // Number of hulls (volumi convessi)
            uint32_t numberOfHulls;
            memcpy(&numberOfHulls, chunkData + chunkPointer, sizeof(uint32_t));
            chunkPointer += sizeof(uint32_t);

            // Avanza il chunk_pointer per saltare 20 byte di dati
            chunkPointer += 20;

            // Ciclo per elaborare ogni hull.
            for (uint32_t i = 0; i < numberOfHulls; ++i)
            {
                // Numero di vertici di un hull.
                uint32_t numberOfHullVertices;
                memcpy(&numberOfHullVertices, chunkData + chunkPointer, sizeof(uint32_t));
                chunkPointer += sizeof(uint32_t);

                // Memorizza il numero di facce del hull.
                uint32_t numberOfHullFaces;
                memcpy(&numberOfHullFaces, chunkData + chunkPointer, sizeof(uint32_t));
                chunkPointer += sizeof(uint32_t);

                // Avanza chunkPointer per saltare i dati dei vertici e delle facce dell'hull.
                chunkPointer += sizeof(glm::vec3) + numberOfHullVertices * sizeof(glm::vec3) + numberOfHullFaces * sizeof(uint32_t) * 3;
            }
        }
    }

    // Number of LODs -->  numero di livelli di dettaglio (LODs) della mesh.
    uint32_t numberOfLods;
    // Copia il numero di LODs dalla posizione corrente del chunk nella variabile number_of_lods.
    memcpy(&numberOfLods, chunkData + chunkPointer, sizeof(uint32_t));
    // Aggiorna chunk_pointer per avanzare oltre il numero di LODs.
    chunkPointer += sizeof(uint32_t);

    //  Viene gestito solo il primo LOD.
    if (numberOfLods > 1)
    {
        WARNING("Only one LOD is supported (current mesh has " << numberOfLods << "). Using the first one.");
    }

    // Ciclo per elaborare ciascun livello di dettaglio (LOD) del mesh. -> si interrompe dopo la prima iterazione.
    for (uint32_t i = 0; i < numberOfLods; ++i)
    {
        // Numero di vertici.
        uint32_t numberOfVertices;
        memcpy(&numberOfVertices, chunkData + chunkPointer, sizeof(uint32_t));
        chunkPointer += sizeof(uint32_t);

        // Numero di facce.
        uint32_t numberOfFaces;
        memcpy(&numberOfFaces, chunkData + chunkPointer, sizeof(uint32_t));
        chunkPointer += sizeof(uint32_t);

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        // Ciclo per elaborare ciascun vertice della mesh.
        for (uint32_t j = 0; j < numberOfVertices; ++j)
        {
            glm::vec3 vertex;
            // Copia i dati del vertice dalla posizione corrente del chunk nella variabile vertex.
            memcpy(&vertex, chunkData + chunkPointer, sizeof(glm::vec3));
            // Aggiorna chunk_pointer per avanzare oltre il dato del vertice.
            chunkPointer += sizeof(glm::vec3);
            vertices.push_back(vertex);


            uint32_t normalRaw;
            // Copia i dati della normale dalla posizione corrente del chunk nella variabile normal_raw.
            memcpy(&normalRaw, chunkData + chunkPointer, sizeof(uint32_t));
            chunkPointer += sizeof(uint32_t);
            // Converte i dati della normale in un oggetto glm::vec3.
            const glm::vec3 normal = glm::vec3(glm::unpackSnorm3x10_1x2(normalRaw));
            normals.push_back(normal);

            uint32_t uvRaw;
            // Copia i dati delle coordinate UV dalla posizione corrente del chunk nella variabile uv_raw.
            memcpy(&uvRaw, chunkData + chunkPointer, sizeof(uint32_t));
            chunkPointer += sizeof(uint32_t);
            // Converte i dati delle coordinate UV in un oggetto glm::vec2.
            const glm::vec2 uv = glm::unpackHalf2x16(uvRaw);
            uvs.push_back(uv);

            chunkPointer += sizeof(uint32_t);
        }

        // Ogni faccia    rappresentata come una tupla di tre indici di vertici.
        std::vector<std::tuple<uint32_t, uint32_t, uint32_t>> faces;

        // Ciclo per elaborare ciascuna faccia della mesh.
        for (uint32_t j = 0; j < numberOfFaces; ++j)
        {
            // Memorizza il primo vertice della faccia.
            uint32_t face0;
            // Copia il primo vertice della faccia dalla posizione corrente del chunk nella variabile face_0.
            memcpy(&face0, chunkData + chunkPointer, sizeof(uint32_t));
            // Aggiorna chunk_pointer per avanzare oltre il dato del primo vertice della faccia.
            chunkPointer += sizeof(uint32_t);

            // Secondo vertice
            uint32_t face1;
            memcpy(&face1, chunkData + chunkPointer, sizeof(uint32_t));
            chunkPointer += sizeof(uint32_t);

            // Terzo vertice
            uint32_t face2;
            memcpy(&face2, chunkData + chunkPointer, sizeof(uint32_t));
            chunkPointer += sizeof(uint32_t);

            // Crea una tupla che rappresenta la faccia con i tre vertici.
            const auto face = std::make_tuple(face0, face1, face2);
            // Aggiunge la faccia al vettore faces.
            faces.push_back(face);
        }

        meshData.set_mesh_data(vertices, faces, normals, uvs);

        // We only consider the first LOD.
        break;
    }
    mesh->setMeshData(meshData);

    return std::make_pair(mesh, numberOfChildren);
}

/**
 * @brief Analizza un chunk di byte e lo converte in un oggetto Material.
 *
 * Questa funzione estrae i dati dal chunk fornito e li utilizza per creare un oggetto `Material`
 * popolato con le informazioni lette. Il chunk di byte contiene il nome del materiale, il colore di
 * emissione, il colore albedo, la rugosit  , la metallicit  , la trasparenza, e i nomi delle texture e delle mappe.
 *
 * @param chunkData I dati del chunk da analizzare.
 * @param chunkSize La dimensione del chunk.
 *
 * @return Una coppia (`std::pair`) contenente:
 * - `std::shared_ptr<Material>`: Puntatore al materiale creato e configurato con i dati estratti.
 * - `std::string`: Nome del materiale.
 */
std::pair<std::shared_ptr<Material>, std::string> LIB_API OVOParser::parseMaterialChunk(const uint8_t* chunkData, const uint32_t chunkSize)
{
    //  Sar   popolato con i dati dal chunk.
    std::shared_ptr<Material> material = std::make_shared<Material>();

    // Usato per scorrere attraverso i dati del chunk.
    uint32_t chunkPointer = 0;

    // Material name
    {
        std::string materialName = OVOParser::parseString(chunkData + chunkPointer);
        chunkPointer += static_cast<uint32_t>(materialName.length() + 1);
        material->setName(materialName);
        if (materialName == "[none]") {
            WARNING("Material not found for mesh. Using default.");
        }
        else {
            DEBUG("Material loaded: " << materialName);
        }
    }

    // Emission
    {
        // Memorizzare il colore di emissione del materiale.
        glm::vec3 emission;
        memcpy(&emission, chunkData + chunkPointer, sizeof(glm::vec3));
        chunkPointer += sizeof(glm::vec3);
        material->setEmissionColor(emission);
    }

    // Albedo
    glm::vec3 albedo;
    {
        // Copia i dati del colore albedo dalla posizione corrente del chunk nella variabile albedo.
        memcpy(&albedo, chunkData + chunkPointer, sizeof(glm::vec3));
        chunkPointer += sizeof(glm::vec3);
    }

    // Roughness -> rugosit   del materiale.
    float roughness;
    {
        memcpy(&roughness, chunkData + chunkPointer, sizeof(float));
        chunkPointer += sizeof(float);
    }

    // Metalness -> metallicit   // Ignorato
    chunkPointer += sizeof(float);

    // Transparency --> alpha
    {
        float alpha;
        memcpy(&alpha, chunkData + chunkPointer, sizeof(float));
        chunkPointer += sizeof(float);
        material->setAlpha(alpha);
    }

    // Texture name
    {
        std::string textureName = OVOParser::parseString(chunkData + chunkPointer);
        chunkPointer += static_cast<uint32_t>(textureName.length() + 1);

        // Se ha una texture
        if (textureName != "[none]")
        {
            std::string fullTexturePath = textureName;

            // Crea un oggetto Texture e lo memorizza in un shared_ptr.
            std::shared_ptr<Texture> texture = std::make_shared<Texture>(fullTexturePath);

            // Assegna la texture al materiale.
            material->setTexture(texture);

        }
    }

    // Avanza il puntatore oltre il nome della mappa // Ignorati

    // Normal map name
    chunkPointer += static_cast<uint32_t>(OVOParser::parseString(chunkData + chunkPointer).length() + 1);

    // Height map name
    chunkPointer += static_cast<uint32_t>(OVOParser::parseString(chunkData + chunkPointer).length() + 1);

    // Roughness map name
    chunkPointer += static_cast<uint32_t>(OVOParser::parseString(chunkData + chunkPointer).length() + 1);

    // Metalness map name
    chunkPointer += static_cast<uint32_t>(OVOParser::parseString(chunkData + chunkPointer).length() + 1);

    // Imposta il colore ambientale del materiale con il colore albedo.
    material->setAmbientColor(albedo);
    material->setSpecularColor(albedo);
    material->setDiffuseColor(albedo);
    // Calcola e imposta la brillantezza del materiale basata sulla rugosit  .
    // Brillantezza massima = 128.0f
    material->setShininess((1.0f - std::sqrt(roughness)) * 128.0f);

    return std::make_pair(material, material->getName());
}

/**
 * @brief Analizza un chunk di byte e lo converte in un oggetto Light.
 *
 * Questa funzione estrae i dati dal chunk fornito e li utilizza per creare un oggetto `Light`
 * di tipo specifico (PointLight, DirectionalLight, o SpotLight) popolato con le informazioni lette.
 * Il chunk di byte contiene il nome della luce, la matrice di trasformazione, il numero di figli,
 * il tipo di luce, il colore, il raggio di influenza, la direzione, il cutoff e l'esponente.
 *
 * @param chunkData I dati del chunk da analizzare.
 * @param chunkSize La dimensione del chunk.
 *
 * @return Una coppia (`std::pair`) contenente:
 * - `std::shared_ptr<Light>`: Puntatore al `Light` creato e configurato con i dati estratti. Il tipo di
 *   luce    determinato dal valore del campo `subtype`.
 * - `uint32_t`: Numero di figli associati alla luce.
 */
std::pair<std::shared_ptr<Light>, uint32_t> LIB_API OVOParser::parseLightChunk(const uint8_t* chunkData, const uint32_t chunkSize)
{
    // Usato per scorrere i dati del chunk.
    uint32_t chunkPointer = 0;

    // Light name
    std::string lightName;
    {
        lightName = OVOParser::parseString(chunkData + chunkPointer);
        chunkPointer += static_cast<uint32_t>(lightName.length() + 1);
    }

    // Matrix
    glm::mat4 matrix;
    {
        memcpy(&matrix, chunkData + chunkPointer, sizeof(glm::mat4));
        chunkPointer += sizeof(glm::mat4);
    }

    // Number of children
    uint32_t numberOfChildren;
    {
        memcpy(&numberOfChildren, chunkData + chunkPointer, sizeof(uint32_t));
        chunkPointer += sizeof(uint32_t);
    }

    // Target node // Ignorato
    chunkPointer += static_cast<uint32_t>(OVOParser::parseString(chunkData + chunkPointer).length() + 1);

    // Tipo di luce:
    // 0: Point
    // 1: Directional
    // 2: Spot
    uint8_t subtype;
    {
        memcpy(&subtype, chunkData + chunkPointer, sizeof(uint8_t));
        chunkPointer += sizeof(uint8_t);
    }

    // Colore della luce
    glm::vec3 color;
    {
        memcpy(&color, chunkData + chunkPointer, sizeof(glm::vec3));
        chunkPointer += sizeof(glm::vec3);
    }

    // Influence radius --> raggio di influenza della luce
    float radius;
    {
        memcpy(&radius, chunkData + chunkPointer, sizeof(float));
        chunkPointer += sizeof(float);
    }

    // Direction
    glm::vec3 direction;
    {
        memcpy(&direction, chunkData + chunkPointer, sizeof(glm::vec3));
        chunkPointer += sizeof(glm::vec3);
    }

    // Cutoff
    float cutoff;
    {
        memcpy(&cutoff, chunkData + chunkPointer, sizeof(float));
        chunkPointer += sizeof(float);
    }

    // Exponent
    float spot_exponent;
    {
        memcpy(&spot_exponent, chunkData + chunkPointer, sizeof(float));
        chunkPointer += sizeof(float);
    }

    // Verifica se il tipo di luce    Point (tipo 0).
    if (subtype == 0) // Point
    {
        std::shared_ptr<PointLight> light = std::make_shared<PointLight>();

        // Imposta il nome della luce PointLight.
        light->setName(lightName);
        light->setBaseMatrix(matrix);
        light->setDiffuseColor(color);
        light->setSpecularColor(color);
        light->setRadius(radius / 250.0f); // Arbitrary

        return std::make_pair(light, numberOfChildren);

    }

    // Verifica se il tipo di luce    Directional (tipo 1).
    else if (subtype == 1) // Directional
    {
        std::shared_ptr<DirectionalLight> light = std::make_shared<DirectionalLight>();

        // Imposta il nome della luce DirectionalLight.
        light->setName(lightName);
        light->setBaseMatrix(matrix);
        light->setDiffuseColor(color);
        light->setDirection(direction);

       // std::cout << light->getName() << std::endl;
        // std::cout << light->getId() << std::endl;

        return std::make_pair(light, numberOfChildren);
    }
    // Verifica se il tipo di luce    Spot (tipo 2).
    else if (subtype == 2) // Spot
    {
        std::shared_ptr<SpotLight> light = std::make_shared<SpotLight>();

        // Imposta il nome della luce SpotLight.
        light->setName(lightName);
        light->setBaseMatrix(matrix);
        light->setDiffuseColor(color);
        light->setSpecularColor(color);
        light->setCutoff(cutoff);
        light->setRadius(radius);
        light->setExponent(spot_exponent);
        light->setDirection(direction);

        return std::make_pair(light, numberOfChildren);

    }
    else
    {
        // Se il tipo di luce non corrisponde a nessuno dei tipi noti.
        // Imposta un tipo di luce di default -> PointLight.
        WARNING("Unknown light subtype: " << (uint32_t)subtype << ". Defaulting to a point light.");

        return std::make_pair(std::make_shared<PointLight>(), 0);
    }
}

