# Resource System

This document describes how GLEngine manages shared resources.

Resources represent reusable assets used by multiple objects in the scene.

Examples include:

- meshes
- textures
- materials

The engine uses a centralized ResourceManager to store and manage these assets.

---

# Resource Manager

The ResourceManager is responsible for managing all engine assets.

Responsibilities include:

- loading resources
- storing resources
- providing access through handles
- preventing duplicate resources

The ResourceManager acts as a central storage system for assets.

Example internal structure:

ResourceManager

meshes
textures
materials

Each resource type is stored in a dedicated container.

---

# Resource Types

The main resource types managed by the engine include:

Meshes

Mesh resources contain geometric data such as:

- vertex positions
- normals
- texture coordinates
- indices

Textures

Textures store image data used by materials and shaders.

Materials

Materials define how surfaces are rendered and may reference:

- shaders
- textures
- rendering parameters

---

# Resource Handles

Resources are accessed through handles rather than raw pointers.

Handles act as lightweight identifiers referencing resources stored in the ResourceManager.

Example:

MeshHandle mesh = resourceManager.loadMesh("cube.obj");

Scene objects store handles instead of pointers.

Example:

RenderObject.mesh = meshHandle;

This approach improves safety and prevents invalid references.

---

# Resource Sharing

Multiple objects in the scene can reference the same resource.

Example:

CubeMesh

ObjectA  
ObjectB  
ObjectC

The mesh is stored once in the ResourceManager and reused by all objects.

This reduces memory usage and avoids redundant asset loading.

---

# Resource Access

Resources are retrieved through the ResourceManager.

Example:

mesh = resourceManager.getMesh(meshHandle)

This resolves the handle and returns the corresponding resource.

The scene does not interact directly with resource storage.

---

# Resource Lifetime

Resources remain in memory while they are registered in the ResourceManager.

Future improvements may include more advanced lifetime management techniques such as:

- reference counting
- generation-based handles

These systems help prevent invalid or dangling resource references.

---

# Resource Loading

Resources are typically loaded from external files.

Examples:

loadMesh("model.obj")

loadTexture("brick.png")

Once loaded, the resource is stored inside the ResourceManager and referenced through handles.

---

# Future Improvements

Possible improvements to the resource system include:

- resource caching
- asynchronous asset loading
- GPU resource management
- hot-reloading for development

These systems would improve scalability and usability as the engine evolves.