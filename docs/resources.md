# Resource System

This document describes how AevumEngine manages shared resources.

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

- textures
- rendering parameters

---

# Resource Handles

Resources are accessed through handles rather than raw pointers.

A handle contains two fields:

- slot: index into the resource pool
- generation: version counter that invalidates stale handles

Handle structure:

ResourceHandle
├── slot
└── generation

Handles provide a null check via isNull(), which returns true when the handle
has never been assigned. isNull() is a lightweight check and does not verify
whether the resource is still alive — that validation is performed internally
by the ResourcePool when resolving the handle.

Example:

MeshHandle mesh = resourceManager.loadMesh("cube.obj");
if (!mesh.isNull()) { ... }

Scene objects store handles instead of raw pointers.

This approach improves safety and prevents dangling references.

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

# Resource Pool

Each resource type is managed by a dedicated ResourcePool.

The pool uses a slot-based allocator with a free list for O(1) allocation
and deallocation. Generation counters ensure that stale handles are safely
rejected after a resource has been removed and its slot reused.

Pool structure:

ResourcePool
├── slots (indexed storage)
├── freeList (available slots, LIFO)
└── cache (path → slot, prevents duplicate loads)

---

# Resource Access

Resources are retrieved through the ResourceManager.

Example:

mesh = resourceManager.getMesh(meshHandle)

This resolves the handle and returns the corresponding resource.
If the handle is stale or null, nullptr is returned.

The scene does not interact directly with resource storage.

---

# Material Loading

Materials are created programmatically via loadMaterial(textureHandle).

Each unique combination of textures produces a unique material. If the same
texture handle is passed again, the existing material is returned from cache
rather than creating a duplicate.

If no texture handle is provided, a default texture is used automatically.

---

# Resource Loading

Resources are typically loaded from external files.

Examples:

loadMesh("model.obj")
loadTexture("brick.png")

Once loaded, the resource is stored inside the ResourceManager and referenced
through handles.

---

# Future Improvements

Possible improvements to the resource system include:

- asynchronous asset loading
- GPU resource management
- hot-reloading for development
- reference counting for automatic lifetime management

These systems would improve scalability and usability as the engine evolves.
