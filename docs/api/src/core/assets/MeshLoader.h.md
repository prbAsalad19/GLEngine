# MeshLoader.h

## Brief Overview
The `MeshLoader` class provides functionality for loading 3D models from OBJ files and converting them into a format that can be easily processed by rendering engines. This includes parsing the OBJ file to extract mesh data and deduplicating vertices to optimize memory usage.

## Table of Classes/Structs/Enums

### MeshLoader
- **Description**: A utility class for loading 3D models from OBJ files.
- **Methods**:
  - `loadOBJ`
  - `toVertexArray`

## Detailed Methods Section

### Method: loadOBJ
- **Full Signature**:
  ```cpp
  static bool loadOBJ(const std::string& filepath, CPUMesh& out);
  ```
- **Return Type Explanation**: 
  - `bool`: Returns `true` if the OBJ file was successfully loaded and parsed into a `CPUMesh`, otherwise returns `false`.
- **Internal Logic/Purpose**:
  - The `loadOBJ` method reads an OBJ file from the specified `filepath` and parses its contents.
  - It populates the provided `out` parameter with a `CPUMesh` containing the vertex, normal, and texture coordinates extracted from the OBJ file.
  - Error handling is included to manage cases where the file cannot be opened or parsed correctly.

### Method: toVertexArray
- **Full Signature**:
  ```cpp
  static std::vector<Vertex> toVertexArray(const CPUMesh& mesh, std::vector<unsigned int>& outIndices);
  ```
- **Return Type Explanation**: 
  - `std::vector<Vertex>`: Returns a vector of `Vertex` objects representing the deduplicated vertices from the input `mesh`.
- **Internal Logic/Purpose**:
  - The `toVertexArray` method takes a `CPUMesh` and converts it into a deduplicated vertex array.
  - It also generates an index array (`outIndices`) that maps these deduplicated vertices back to their original indices in the `CPUMesh`.
  - This process ensures efficient memory usage by removing duplicate vertices, which are common in many 3D models.