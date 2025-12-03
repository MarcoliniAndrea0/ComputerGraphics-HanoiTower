/**
 * @file OvoParser.h
 * @brief Parser per file formato .ovo (OverVision Object).
 */
#pragma once

#include "engine.h"
#include "node.h"
#include "mesh.h"
#include "material.h"
#include <string>
#include <vector>

 // --- OVO FORMAT DEFINITIONS ---
 // Chunk IDs
#define OVO_CHUNK_HEADER            0x4F564F20  // "OVO "
#define OVO_CHUNK_OBJECT            0x4F424A20  // "OBJ "
#define OVO_CHUNK_MESH              0x4D455348  // "MESH"
#define OVO_CHUNK_LIGHT             0x4C494754  // "LIGT"
#define OVO_CHUNK_MATERIAL          0x4D41544C  // "MATL"
#define OVO_CHUNK_TEXTURE           0x54455854  // "TEXT"

// Sub-chunks for Mesh
#define OVO_CHUNK_VERTICES          0x56455254  // "VERT"
#define OVO_CHUNK_NORMALS           0x4E4F524D  // "NORM"
#define OVO_CHUNK_FACES             0x46414345  // "FACE"
#define OVO_CHUNK_TEXCOORDS         0x54455843  // "TEXC"

// Version
#define OVO_VERSION                 1

class ENG_API OvoParser
{
public:
    OvoParser();
    ~OvoParser();

    /**
     * @brief Carica una scena da un file .ovo
     * @param filename Percorso del file
     * @return Puntatore al nodo radice della scena caricata (o nullptr in caso di errore)
     */
    Node* loadFile(const std::string& filename);

private:
    // Metodo ricorsivo per processare i chunk
    Node* parseChunk(char* data, unsigned int& position, unsigned int size);
};