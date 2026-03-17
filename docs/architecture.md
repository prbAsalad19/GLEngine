# GLEngine Architecture

This document describes the high-level architecture of GLEngine and the interaction between its main systems.

The goal of the engine is to maintain a **modular architecture** that allows the rendering backend and internal systems to evolve without requiring major rewrites.

The current architecture separates:

- Scene data
- Rendering
- Resource management
- GPU backend logic

---

# Core Architecture

The engine is organized into independent modules.

Engine
│
├── Renderer
├── ResourceManager
├── Scene
└── Core Systems

Each module has a well-defined responsibility.

---

# Engine

The Engine coordinates the main subsystems and manages application lifecycle.

Responsibilities include:

- initialization
- system creation
- main loop management
- renderer invocation

Typical frame flow:

update scene
prepare render data
renderer.render(scene)

---

# Scene System

The Scene represents the **current state of the world**.

A scene stores the objects that exist in the world and their spatial properties.

The scene **does not store meshes or textures directly**.  
Instead it references them through resource handles.

Structure:

Scene
├── RenderObject
├── RenderObject
└── RenderObject

The scene is therefore responsible for:

- world state
- object transforms
- object references to resources

---

# RenderObject

A RenderObject represents a renderable instance inside the scene.

It connects scene data with engine resources.

Typical structure:

RenderObject
├── MeshHandle
├── MaterialHandle
└── Transform

Multiple render objects may reference the same mesh or material.

Example:

CubeMesh

↑ ↑ ↑

ObjectA ObjectB ObjectC

This prevents resource duplication and improves memory efficiency.

---

# Transform System

Each render object has a transform describing its spatial state.

A transform typically contains:

Transform
├── position
├── rotation
└── scale

Transforms are **scene data**, not resources.

They are updated by the scene or game logic.

---

# Resource Management

All shared assets are stored inside the ResourceManager.

This includes:

- meshes
- textures
- materials

The ResourceManager ensures that resources are loaded once and reused.

Example:

ResourceManager

meshes:
0 → cube
1 → sphere

textures:
0 → brick
1 → metal

RenderObjects reference these resources through handles.

---

# Renderer

The Renderer is responsible for drawing the scene.

The renderer receives:

- scene data
- resource references

and produces the final frame.

Rendering flow:

Renderer.render(scene)

for each object in scene:
mesh = resources.getMesh(object.mesh)
material = resources.getMaterial(object.material)

apply transform
draw mesh

The renderer does not store scene state.

It only consumes it.

---

# Backend Independence

The architecture is designed so that the renderer backend can be replaced.

Future structure may look like:

Renderer
├── OpenGLRenderer
└── VulkanRenderer

This allows experimenting with different graphics APIs without modifying core engine systems.

---

# Design Principles

The architecture follows several core principles.

Separation of Concerns  
Each system has a clear responsibility.

Resource Sharing  
Assets are centrally managed and reused.

Backend Modularity  
Rendering API logic should not affect engine core systems.

Scalability  
The architecture should allow future features without major refactors.

---

# Future Architectural Extensions

Possible future improvements include:

Renderer abstraction interface

IRenderer
├── OpenGLRenderer
└── VulkanRenderer

Material system improvements

Asset loading pipeline

Resource lifetime management

Scene management improvements