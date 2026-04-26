### MeshLoader.cpp

#### Brief Overview:
`MeshLoader.cpp` is a source file for loading 3D mesh data from OBJ files into an internal `CPUMesh` format. This allows for efficient rendering and manipulation of mesh data within the application.

#### Table of Classes/Structs/Enums:
- **Classes/Structs:**
  - `MeshLoader`
    - Contains methods to load mesh data from OBJ files.
  - `Vertex`
    - Represents a single vertex with position, texture coordinates, and normal.

- **Enums:**
  - None defined in this file.

#### Detailed Methods Section:

**1. MeshLoader::loadOBJ**

- **Full Signature:**
  ```cpp
  bool MeshLoader::loadOBJ(const std::string& filepath, CPUMesh& out)
  ```

- **Return Type Explanation:**
  - `bool`: Returns `true` if the file is successfully loaded and parsed; otherwise, returns `false`.

- **Internal Logic/Purpose:**
  This function reads an OBJ file line by line and populates a `CPUMesh` object with vertex positions, texture coordinates, and normals. It handles parsing of vertices (`v`), texture coordinates (`vt`), and normals (`vn`). For faces (`f`), it performs fan triangulation to ensure all polygons are converted into triangles.

**2. MeshLoader::toVertexArray**

- **Full Signature:**
  ```cpp
  std::vector<Vertex> MeshLoader::toVertexArray(const CPUMesh& mesh, std::vector<unsigned int>& outIndices)
  ```

- **Return Type Explanation:**
  - `std::vector<Vertex>`: Returns a vector of `Vertex` objects representing the vertices of the mesh, with each vertex normalized and shared appropriately to minimize memory usage.

- **Internal Logic/Purpose:**
  This function converts a `CPUMesh` into an array of `Vertex` objects. It handles different types of faces (`v/vt`, `v//vn`, `v/vt/vn`) and uses a cache to avoid duplicate vertices, thus optimizing the mesh data for rendering.

### Example Usage:

```cpp
#include "MeshLoader.h"
#include "CPUMesh.h"

int main() {
    CPUMesh mesh;
    MeshLoader loader;

    if (loader.loadOBJ("path/to/your/model.obj", mesh)) {
        std::vector<Vertex> vertices = loader.toVertexArray(mesh, mesh.indices);
        // Render the vertices using your graphics API
    } else {
        std::cerr << "Failed to load OBJ file" << std::endl;
    }

    return 0;
}
```

### Notes:
- The `MeshLoader` class is designed to be simple and efficient, handling basic OBJ file formats.
- The `toVertexArray` function optimizes the mesh data by reducing duplicate vertices, which is crucial for performance in rendering applications.