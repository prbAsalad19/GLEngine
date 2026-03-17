#pragma once
#include "config.h"
#include "algebricOp.h"

// ─────────────────────────────────────────────
//  Mesh  — GPU-side geometry (VAO / VBO / EBO)
// ─────────────────────────────────────────────



// ─────────────────────────────────────────────
//  CPUMesh  — raw parsed data, lives only on CPU
// ─────────────────────────────────────────────



// ─────────────────────────────────────────────
//  MeshLoader  — loads OBJ files and converts
//               CPUMesh to GPU-ready data
// ─────────────────────────────────────────────

class MeshLoader
{
public:
    // Parses an OBJ file into a CPUMesh. Returns false on failure.
    static bool loadOBJ(const std::string& filepath, CPUMesh& out);

    // Converts a CPUMesh to a deduplicated vertex array + index array.
    static std::vector<Vertex> toVertexArray(const CPUMesh& mesh,
                                             std::vector<unsigned int>& outIndices);
};

