# GLEngine

GLEngine is a minimal modular rendering engine written in C++ using OpenGL.

The project focuses on exploring **clean engine architecture** rather than building a full-featured engine.

It is designed with modularity in mind so that the rendering backend can evolve over time.

---

# Goals

The main goals of this project are:

- explore rendering engine architecture
- build a modular engine structure
- separate scene logic from rendering
- experiment with resource management systems
- allow future backend replacement (e.g. Vulkan)

---

# Engine Overview

The engine is built around a small number of core systems:

- Scene system
- Render objects
- Resource manager
- Renderer

Simplified architecture:

Engine
│
├── Renderer
├── ResourceManager
└── Scene

The scene contains objects that reference shared resources managed by the ResourceManager.

---

# Features

Current features:

- scene structure
- render objects
- resource manager
- handle-based resource referencing
- basic rendering pipeline

The project is currently under active refactoring.

---

# Future Plans

Planned improvements include:

- rendering abstraction layer
- Vulkan backend
- improved resource lifetime management
- material system
- asset loading pipeline

---

# Build

Requirements:

- C++17
- OpenGL
- CMake

Build:

mkdir build
cd build
cmake ..
make

---

# Project Status

This project is experimental and mainly intended for learning and experimentation with rendering engine architecture.

---

# License

MIT License