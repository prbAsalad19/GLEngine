# OpenGLMesh.h

## Brief Overview

The `OpenGLMesh` class is designed to manage and render 3D mesh data in an OpenGL application. It encapsulates the vertex and index buffers, as well as the bounding volume hierarchy (AABB), providing methods to draw the mesh both individually and in a batched instance mode.

## Table of Classes/Structs/Enums

- `OpenGLMesh`
- `Vertex` (not defined in this file but assumed to be included from another header)
- `AABB` (Bounding Box, included from `core/bvh/aabb.h`)

## Detailed Methods Section

### Constructor
```cpp
OpenGLMesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           AABB aabb);
```
**Return Type Explanation:** N/A (No return value; initializes the mesh)
**Internal Logic/Purpose:** Constructs an `OpenGLMesh` object with given vertex data, index data, and bounding box. Initializes OpenGL buffers for vertices, indices, and sets up the Vertex Array Object (VAO).

### Destructor
```cpp
~OpenGLMesh();
```
**Return Type Explanation:** N/A (No return value; cleans up allocated resources)
**Internal Logic/Purpose:** Frees the OpenGL buffers and deletes vertex array objects when the `OpenGLMesh` object is destroyed.

### Draw Method
```cpp
void draw() const;
```
**Return Type Explanation:** N/A (No return value; renders the mesh)
**Internal Logic/Purpose:** Binds the VAO, sets up the vertex attributes, binds the index buffer, and draws the mesh using OpenGL primitives. This method is suitable for rendering a single instance of the mesh.

### Draw Instanced Method
```cpp
void drawInstanced(uint32_t instanceCount) const;
```
**Return Type Explanation:** N/A (No return value; renders multiple instances of the mesh)
**Internal Logic/Purpose:** Similar to `draw`, but allows for rendering multiple instances of the same mesh in a single draw call. This is useful for optimizing rendering of objects with identical geometry but different transformations.

### Member Variables
- `VAO, VBO, EBO`: OpenGL buffer objects used to store vertex array, vertex buffer, and element (index) buffer data.
- `indexCount`: Number of indices that make up the mesh, used in drawing calls.
- `aabb`: Bounding Box (AABB) representing the axis-aligned bounding box of the mesh. Used for culling or other spatial queries.

This class provides a comprehensive interface for managing and rendering 3D meshes in an OpenGL application, supporting both individual and batched rendering modes.