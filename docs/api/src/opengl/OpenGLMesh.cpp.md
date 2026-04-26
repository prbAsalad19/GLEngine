# OpenGLMesh.cpp

## Brief Overview
`OpenGLMesh.cpp` contains the implementation for a class that manages an OpenGL mesh. The `OpenGLMesh` class encapsulates the creation and management of vertex array objects (VAOs), vertex buffer objects (VBOs), and element buffer objects (EBOs) to render meshes in an OpenGL application.

## Table of Classes/Structs/Enums
- **Class**: 
  - `OpenGLMesh`

## Detailed Methods Section

### Method: OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
- **Full Signature**:
  ```cpp
  OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices,
                          const std::vector<unsigned int>& indices);
  ```
- **Return Type Explanation**: 
  This method does not return any value.
- **Internal Logic/Purpose**: 
  Initializes a new instance of the `OpenGLMesh` class with the given vertex and index data. It sets up the necessary OpenGL buffers (VBO, EBO) and attributes to store and render the mesh.

### Method: void OpenGLMesh::draw() const
- **Full Signature**:
  ```cpp
  void OpenGLMesh::draw() const;
  ```
- **Return Type Explanation**: 
  This method does not return any value.
- **Internal Logic/Purpose**: 
  Binds the VAO and draws the mesh using `glDrawElements`, rendering the mesh as triangles.

### Method: void OpenGLMesh::drawInstanced(uint32_t instanceCount) const
- **Full Signature**:
  ```cpp
  void OpenGLMesh::drawInstanced(uint32_t instanceCount) const;
  ```
- **Return Type Explanation**: 
  This method does not return any value.
- **Internal Logic/Purpose**: 
  Binds the VAO and draws the mesh using `glDrawElementsInstanced`, allowing for instanced rendering with the specified number of instances.

### Method: OpenGLMesh::~OpenGLMesh()
- **Full Signature**:
  ```cpp
  OpenGLMesh::~OpenGLMesh();
  ```
- **Return Type Explanation**: 
  This method does not return any value.
- **Internal Logic/Purpose**: 
  Cleans up by deleting the VAO, VBO, and EBO associated with this mesh to free up GPU resources.