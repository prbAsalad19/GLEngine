#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include "core/assets/CPUMesh.h"

class MeshLoader
{
public:
    // Parses an OBJ file into a CPUMesh. Returns false on failure.
    static bool loadOBJ(const std::string& filepath, CPUMesh& out);

    // Converts a CPUMesh to a deduplicated vertex array + index array.
    static std::vector<Vertex> toVertexArray(const CPUMesh& mesh,
        std::vector<unsigned int>& outIndices);
};