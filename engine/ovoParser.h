/**
 * @file OvoParser.h
 * @brief Definizione della classe OvoParser (Fixed v3).
 */
#pragma once

#include "engine.h"
#include "node.h"
#include "mesh.h"
#include "material.h"
#include <string>
#include <vector>

 // --- OVO CHUNK IDS ---
#define OVO_CHUNK_OBJECT    0   // Header
#define OVO_CHUNK_NODE      1
#define OVO_CHUNK_OBJECT2D  2
#define OVO_CHUNK_OBJECT3D  3   // Spesso usato come root
#define OVO_CHUNK_LIST      4
#define OVO_CHUNK_MATERIAL  9
#define OVO_CHUNK_CAMERA    15
#define OVO_CHUNK_LIGHT     16
#define OVO_CHUNK_BONE      17
#define OVO_CHUNK_MESH      18
#define OVO_CHUNK_SKINNED   19

// Subtypes
#define OVO_MESH_DEFAULT        0
#define OVO_MESH_NORMALMAPPED   1
#define OVO_MESH_TESSELLATED    2

#define OVO_LIGHT_OMNI          0
#define OVO_LIGHT_DIRECTIONAL   1
#define OVO_LIGHT_SPOT          2

class ENG_API OvoParser
{
public:
    OvoParser();
    ~OvoParser();

    Node* loadFile(const std::string& filename);

private:
    Node* parseChunk(char* data, unsigned int& position, unsigned int size);
};