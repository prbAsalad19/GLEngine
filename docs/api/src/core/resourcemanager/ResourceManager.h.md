## ResourceManager.h

### Brief Overview

The `ResourceManager` class is a crucial component of the resource management system in our application. It provides methods for loading and managing different types of resources such as meshes, textures, and materials. This class uses `ResourcePool` to efficiently manage the lifecycle of these resources, ensuring that they are properly allocated and deallocated.

### Table of Classes/Structs/Enums

- **ResourceManager**: Manages loading, retrieving, and deleting various types of resources.
- **MeshTag, TextureTag, MaterialTag**: Tags used for resource identification in the pools.
- **OpenGLMesh, OpenGLTexture**: Specific implementations of mesh and texture resources.
- **ResourceHandle**: Handle type used to reference resources managed by `ResourceManager`.
- **AABB (Axis-Aligned Bounding Box)**: Structure representing a 3D bounding box.

### Detailed Methods Section

#### Method: loadMesh(const std::string& filepath)

**Full Signature:**
```cpp
const MeshHandle loadMesh(const std::string& filepath);
```

**Return Type Explanation:**
- `MeshHandle`: A handle to the newly loaded mesh resource. This handle can be used later to access or delete the mesh.

**Internal Logic/Purpose:**
This method takes a file path as input and attempts to load a mesh from that file. If successful, it stores the mesh in the `meshPool` and returns a handle to it. The purpose is to ensure that each mesh is loaded only once and can be efficiently accessed throughout the application.

#### Method: getMesh(MeshHandle handle)

**Full Signature:**
```cpp
OpenGLMesh* getMesh(MeshHandle handle);
```

**Return Type Explanation:**
- `OpenGLMesh*`: A pointer to the OpenGL mesh resource corresponding to the given handle. This allows direct access to the mesh's data.

**Internal Logic/Purpose:**
This method retrieves a pointer to an existing mesh resource from the `meshPool` using the provided handle. It is used when accessing properties or methods of the mesh that are not exposed through the `MeshHandle`.

#### Method: getMeshAABB(MeshHandle handle)

**Full Signature:**
```cpp
AABB getMeshAABB(MeshHandle handle);
```

**Return Type Explanation:**
- `AABB`: The Axis-Aligned Bounding Box (AABB) of the mesh. This is useful for collision detection and other spatial queries.

**Internal Logic/Purpose:**
This method retrieves the AABB for a specific mesh resource identified by the given handle from the `meshPool`. The AABB provides quick bounds-checking information for the mesh.

#### Method: deleteMesh(MeshHandle handle)

**Full Signature:**
```cpp
void deleteMesh(MeshHandle handle);
```

**Return Type Explanation:**
- `void`: No return value. This method simply removes the specified mesh from the pool, ensuring that it is no longer accessible and can be freed.

**Internal Logic/Purpose:**
This method deletes a specific mesh resource identified by the given handle from the `meshPool`. It is used to manage memory when resources are no longer needed. The purpose is to prevent memory leaks and ensure that resources are properly deallocated.

#### Method: loadTexture(const std::string& filepath)

**Full Signature:**
```cpp
const TextureHandle loadTexture(const std::string& filepath);
```

**Return Type Explanation:**
- `TextureHandle`: A handle to the newly loaded texture resource. This handle can be used later to access or delete the texture.

**Internal Logic/Purpose:**
This method takes a file path as input and attempts to load a texture from that file. If successful, it stores the texture in the `texturePool` and returns a handle to it. The purpose is to ensure that each texture is loaded only once and can be efficiently accessed throughout the application.

#### Method: getTexture(TextureHandle handle)

**Full Signature:**
```cpp
OpenGLTexture* getTexture(TextureHandle handle);
```

**Return Type Explanation:**
- `OpenGLTexture*`: A pointer to the OpenGL texture resource corresponding to the given handle. This allows direct access to the texture's data.

**Internal Logic/Purpose:**
This method retrieves a pointer to an existing texture resource from the `texturePool` using the provided handle. It is used when accessing properties or methods of the texture that are not exposed through the `TextureHandle`.

#### Method: deleteTexture(TextureHandle handle)

**Full Signature:**
```cpp
void deleteTexture(TextureHandle handle);
```

**Return Type Explanation:**
- `void`: No return value. This method simply removes the specified texture from the pool, ensuring that it is no longer accessible and can be freed.

**Internal Logic/Purpose:**
This method deletes a specific texture resource identified by the given handle from the `texturePool`. It is used to manage memory when resources are no longer needed. The purpose is to prevent memory leaks and ensure that resources are properly deallocated.

#### Method: loadMaterial(TextureHandle baseTexture)

**Full Signature:**
```cpp
const MaterialHandle loadMaterial(TextureHandle baseTexture);
```

**Return Type Explanation:**
- `MaterialHandle`: A handle to the newly loaded material resource. This handle can be used later to access or delete the material.

**Internal Logic/Purpose:**
This method takes a texture handle as input and attempts to create a material using that texture as its base. If successful, it stores the material in the `materialPool` and returns a handle to it. The purpose is to provide a way to manage materials with different textures efficiently.

#### Method: getMaterial(MaterialHandle handle)

**Full Signature:**
```cpp
Material* getMaterial(MaterialHandle handle);
```

**Return Type Explanation:**
- `Material*`: A pointer to the material resource corresponding to the given handle. This allows direct access to the material's properties and methods.

**Internal Logic/Purpose:**
This method retrieves a pointer to an existing material resource from the `materialPool` using the provided handle. It is used when accessing or modifying properties of the material that are not exposed through the `MaterialHandle`.

#### Method: deleteMaterial(MaterialHandle handle)

**Full Signature:**
```cpp
void deleteMaterial(MaterialHandle handle);
```

**Return Type Explanation:**
- `void`: No return value. This method simply removes the specified material from the pool, ensuring that it is no longer accessible and can be freed.

**Internal Logic/Purpose:**
This method deletes a specific material resource identified by the given handle from the `materialPool`. It is used to manage memory when resources are no longer needed. The purpose is to prevent memory leaks and ensure that resources are properly deallocated.