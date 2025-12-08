#pragma once

#include <unordered_map>
#include <filesystem>
#include "Common.h"
#include "mesh.h"
#include "node.h"
#include "light.h"
#include "material.h"
#include "MeshData.h"
#include "texture.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "spotLight.h"

/**
 * @class OVOParser
 * @brief Parser per il caricamento di grafi di scena da file OVO.
 *
 * La classe `OVOParser` fornisce funzionalit� per caricare e analizzare i dati delle scene dal formato di file OVO.
 * Converte i dati di questi file in una struttura di grafo di scena composta da nodi, mesh, materiali e luci.
 *
 * ### Funzionalit� principali:
 * - Caricamento di un grafo di scena completo da un file OVO.
 * - Analisi dei chunk di dati corrispondenti a nodi, mesh, materiali e luci.
 * - Estrazione di stringhe dai dati binari.
 * - Gestione di una mappa per evitare duplicazioni dei materiali.
 */
class LIB_API OVOParser
{
public:
    /**
     * @brief Carica un grafo di scena da un file OVO.
     *
     * Questa funzione legge un file `.ovo` specificato dal percorso `filePath`, analizza i dati in esso contenuti e
     * costruisce una struttura di grafo di scena composta da nodi, mesh, luci e materiali.
     *
     * @param filePath Il percorso del file `.ovo` da caricare.
     * @return Un puntatore condiviso al nodo radice della scena caricata.
     *
     * @throws std::runtime_error Se il file non pu� essere aperto o se i dati del file sono corrotti.
     */
    static std::shared_ptr<Node> fromFile(const std::string filePath);

private:
    /**
     * @brief Analizza un chunk di dati per creare un nodo della scena.
     *
     * Questo metodo estrae i dati di un nodo dal chunk fornito e costruisce un oggetto `Node`.
     * Include informazioni come il nome del nodo, la matrice di trasformazione e il numero di figli.
     *
     * @param chunkData I dati del chunk in formato binario.
     * @param chunkSize La dimensione del chunk in byte.
     * @return Una coppia contenente un puntatore condiviso al nodo e il numero di figli associati.
     */
    static std::pair<std::shared_ptr<Node>, uint32_t> parseNodeChunk(const uint8_t* chunkData, const uint32_t chunkSize);

    /**
     * @brief Analizza un chunk di dati per creare una mesh.
     *
     * Questo metodo estrae i dati di una mesh dal chunk fornito e costruisce un oggetto `Mesh` popolato
     * con informazioni sui vertici, le facce, le normali e le coordinate UV.
     *
     * @param chunkData I dati del chunk in formato binario.
     * @param chunkSize La dimensione del chunk in byte.
     * @return Una coppia contenente un puntatore condiviso alla mesh e il numero di figli associati.
     */
    static std::pair<std::shared_ptr<Mesh>, uint32_t> parseMeshChunk(const uint8_t* chunkData, const uint32_t chunkSize);

    /**
     * @brief Analizza un chunk di dati per creare un materiale.
     *
     * Questo metodo estrae i dati di un materiale dal chunk fornito e costruisce un oggetto `Material`
     * con informazioni come il nome, i colori albedo e di emissione, la rugosit�, la trasparenza e le texture.
     *
     * @param chunkData I dati del chunk in formato binario.
     * @param chunkSize La dimensione del chunk in byte.
     * @return Una coppia contenente un puntatore condiviso al materiale e il nome del materiale.
     */
    static std::pair<std::shared_ptr<Material>, std::string> parseMaterialChunk(const uint8_t* chunkData, const uint32_t chunkSize);

    /**
     * @brief Analizza un chunk di dati per creare una luce.
     *
     * Questo metodo estrae i dati di una luce dal chunk fornito e costruisce un oggetto `Light` di tipo
     * specifico (PointLight, DirectionalLight o SpotLight), popolato con informazioni come il colore,
     * la direzione, il raggio di influenza e il tipo di luce.
     *
     * @param chunkData I dati del chunk in formato binario.
     * @param chunkSize La dimensione del chunk in byte.
     * @return Una coppia contenente un puntatore condiviso alla luce e il numero di figli associati.
     */
    static std::pair<std::shared_ptr<Light>, uint32_t> parseLightChunk(const uint8_t* chunkData, const uint32_t chunkSize);

    /**
     * @brief Converte un array di byte in una stringa terminata con carattere null.
     *
     * Questo metodo estrae una stringa da un buffer di byte, terminata da un carattere null (`\0`).
     *
     * @param data Puntatore al buffer di dati binari.
     * @return La stringa estratta dai dati forniti.
     */
    static std::string parseString(const uint8_t* data);

    /**
     * @brief Mappa contenente i materiali caricati durante il parsing.
     *
     * Questa mappa viene utilizzata per evitare la duplicazione dei materiali durante il caricamento della scena.
     * La chiave � il nome del materiale, mentre il valore � un puntatore condiviso al materiale stesso.
     */
    static std::unordered_map<std::string, std::shared_ptr<Material>> materials;
};
