# Renderer

This document describes the renderer system used in GLEngine.

The renderer is responsible for converting scene data into GPU draw calls and producing the final rendered image.

The renderer does not store scene state.  
Instead, it consumes scene data and resource references to generate the frame.

---

# Renderer Responsibilities

The renderer performs the following tasks:

- retrieving renderable objects from the scene
- accessing meshes and materials through the ResourceManager
- applying object transforms
- issuing GPU draw commands
- producing the final image

The renderer should not contain game logic or scene management logic.

---

# Rendering Flow

The renderer processes the scene every frame.

Simplified rendering pipeline:

Scene  
↓  
Renderer  
↓  
GPU commands  
↓  
Framebuffer  
↓  
Final image

Typical frame process:

1. The scene provides a list of RenderObjects
2. Each RenderObject references resources through handles
3. The renderer retrieves the required resources
4. GPU draw calls are issued

Example rendering loop:

Renderer.render(scene)

for each RenderObject in scene:
mesh = resourceManager.getMesh(object.mesh)
material = resourceManager.getMaterial(object.material)
applyTransform(object.transform)
draw(mesh)

The renderer acts as a bridge between **scene data and GPU resources**.

---

# Rendering Backend

The current implementation uses **OpenGL** as the graphics API.

OpenGL is used to:

- create vertex buffers
- upload mesh data
- manage textures
- issue draw calls

The renderer currently focuses on simplicity while the engine architecture is being developed.

---

# Renderer Data Sources

The renderer combines data from two main systems.

Scene data:

- render objects
- transforms

Resource data:

- meshes
- textures
- materials

Scene objects reference resources using handles, which are resolved through the ResourceManager.

---

# Future Renderer Architecture

The renderer is designed so that the rendering backend can evolve in the future.

Possible architecture:

Renderer  
├── OpenGLRenderer  
└── VulkanRenderer

This allows the rendering API to change without modifying core engine systems.

---

# Deferred Rendering (Future Direction)

The renderer may eventually implement a deferred rendering pipeline.

Deferred rendering separates geometry rendering from lighting calculations.

Typical pipeline:

Geometry Pass → GBuffer

Lighting Pass → Final Image

Advantages:

- efficient handling of large numbers of lights
- lighting decoupled from geometry rendering
- better scalability for complex scenes

However, deferred rendering introduces additional complexity such as:

- multiple render targets
- GBuffer management
- additional memory usage

For this reason, the current renderer remains simple while the engine architecture stabilizes.

---

# Possible Future Features

Future renderer improvements may include:

- deferred rendering
- shadow mapping
- post-processing effects
- physically based materials
- render graph architecture

These features depend on the evolution of the engine architecture.