 ```cpp
// CPUMesh.cpp
// Author: John Doe
// Date: October 15, 2023

#include "CPUMesh.h"

namespace Core::Assets {

    // Class to represent a CPU-based mesh data structure.

    class CPUMesh {
    public:
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 uv;
        };

        struct Index {
            uint16_t index;
        };

        enum class MeshType {
            Static,
            Dynamic
        };

        // Constructor to initialize the mesh with a specified type.
        CPUMesh(MeshType type) : m_Type(type), m_Vertices(), m_Indices() {}

        // Destructor to clean up resources.
        ~CPUMesh() = default;

        // Method to add a vertex to the mesh.
        void AddVertex(const Vertex& vertex) {
            m_Vertices.push_back(vertex);
        }

        // Method to add an index to the mesh.
        void AddIndex(const Index& index) {
            m_Indices.push_back(index);
        }

        // Method to retrieve the type of the mesh.
        MeshType GetMeshType() const {
            return m_Type;
        }

        // Method to retrieve the vertices of the mesh.
        const std::vector<Vertex>& GetVertices() const {
            return m_Vertices;
        }

        // Method to retrieve the indices of the mesh.
        const std::vector<Index>& GetIndices() const {
            return m_Indices;
        }

    private:
        MeshType m_Type; // Type of the mesh (Static or Dynamic).
        std::vector<Vertex> m_Vertices; // List of vertices in the mesh.
        std::vector<Index> m_Indices; // List of indices defining the mesh's topology.
    };

} // namespace Core::Assets
```