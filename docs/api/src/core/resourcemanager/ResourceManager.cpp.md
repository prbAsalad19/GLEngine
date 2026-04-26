# ResourceManager.cpp

## Brief Overview
The `ResourceManager` class manages the loading and retrieval of various types of resources (meshes, textures, materials) for a rendering engine. It uses resource pools to store loaded resources efficiently and provides methods to load, retrieve, and delete these resources.

## Table of Classes/Structs/Enums
- **MeshHandle**: A handle to a mesh resource.
- **TextureHandle**: A handle to a texture resource.
- **MaterialHandle**: A handle to a material resource.
- **ResourcePool<T>**: A template class for managing a pool of resources.
- **OpenGLMesh**: Represents a mesh using OpenGL.
- **OpenGLTexture**: Represents a texture using OpenGL.
- **Material**: Contains properties and textures defining a material.

## Detailed Methods Section

### `const MeshHandle ResourceManager::loadMesh(const std::string& path)`
**Full Signature:**
```cpp
const MeshHandle ResourceManager::loadMesh(const std::string& path)
```
**Return Type Explanation:**  
A `MeshHandle` that uniquely identifies the loaded mesh. If the mesh cannot be loaded, an invalid handle is returned.

**Internal Logic/Purpose:**  
1. Load the OBJ file into a CPU-based mesh structure using `MeshLoader::loadOBJ`.
2. Convert the CPU mesh to vertex and index data.
3. Create an `OpenGLMesh` object from the converted data.
4. Insert the mesh into the `meshPool` with its path as the key.

### `OpenGLMesh* ResourceManager::getMesh(MeshHandle handle)`
**Full Signature:**
```cpp
OpenGLMesh* ResourceManager::getMesh(MeshHandle handle)
```
**Return Type Explanation:**  
A pointer to an `OpenGLMesh` object if the handle is valid, otherwise `nullptr`.

**Internal Logic/Purpose:**  
Retrieve an `OpenGLMesh` from the `meshPool` using the provided handle.

### `AABB ResourceManager::getMeshAABB(MeshHandle handle)`
**Full Signature:**
```cpp
AABB ResourceManager::getMeshAABB(MeshHandle handle)
```
**Return Type Explanation:**  
An axis-aligned bounding box (AABB) of the mesh associated with the given handle.

**Internal Logic/Purpose:**  
Retrieve the AABB from the `OpenGLMesh` object in the `meshPool`.

### `void ResourceManager::deleteMesh(MeshHandle handle)`
**Full Signature:**
```cpp
void ResourceManager::deleteMesh(MeshHandle handle)
```
**Return Type Explanation:**  
None (void).

**Internal Logic/Purpose:**  
Remove an `OpenGLMesh` from the `meshPool` using the provided handle.

### `const TextureHandle ResourceManager::loadTexture(const std::string& path)`
**Full Signature:**
```cpp
const TextureHandle ResourceManager::loadTexture(const std::string& path)
```
**Return Type Explanation:**  
A `TextureHandle` that uniquely identifies the loaded texture.

**Internal Logic/Purpose:**  
1. Create an `OpenGLTexture` object from the given file path.
2. Insert the texture into the `texturePool` with its path as the key.

### `OpenGLTexture* ResourceManager::getTexture(TextureHandle handle)`
**Full Signature:**
```cpp
OpenGLTexture* ResourceManager::getTexture(TextureHandle handle)
```
**Return Type Explanation:**  
A pointer to an `OpenGLTexture` object if the handle is valid, otherwise `nullptr`.

**Internal Logic/Purpose:**  
Retrieve an `OpenGLTexture` from the `texturePool` using the provided handle.

### `void ResourceManager::deleteTexture(TextureHandle handle)`
**Full Signature:**
```cpp
void ResourceManager::deleteTexture(TextureHandle handle)
```
**Return Type Explanation:**  
None (void).

**Internal Logic/Purpose:**  
Remove an `OpenGLTexture` from the `texturePool` using the provided handle.

### `const MaterialHandle ResourceManager::loadMaterial(TextureHandle albedo)`
**Full Signature:**
```cpp
const MaterialHandle ResourceManager::loadMaterial(TextureHandle albedo)
```
**Return Type Explanation:**  
A `MaterialHandle` that uniquely identifies the loaded material.

**Internal Logic/Purpose:**  
1. Resolve the case where the albedo texture is null by loading a default texture if necessary.
2. Construct a unique key based on the albedo texture properties.
3. Create a `Material` object with the specified textures.
4. Insert the material into the `materialPool` with its key.

### `Material* ResourceManager::getMaterial(MaterialHandle handle)`
**Full Signature:**
```cpp
Material* ResourceManager::getMaterial(MaterialHandle handle)
```
**Return Type Explanation:**  
A pointer to a `Material` object if the handle is valid, otherwise `nullptr`.

**Internal Logic/Purpose:**  
Retrieve a `Material` from the `materialPool` using the provided handle.

### `void ResourceManager::deleteMaterial(MaterialHandle handle)`
**Full Signature:**
```cpp
void ResourceManager::deleteMaterial(MaterialHandle handle)
```
**Return Type Explanation:**  
None (void).

**Internal Logic/Purpose:**  
Remove a `Material` from the `materialPool` using the provided handle.