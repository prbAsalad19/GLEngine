# OpenGL Rendering Project

## Overview

This project is a modern OpenGL rendering application written in C++.
Its purpose is to load 3D mesh data from `.obj` files, transfer it to the GPU, and render it using custom vertex and fragment shaders.

The codebase is structured to separate:

* CPU-side mesh handling
* GPU-side mesh representation
* Shader management
* Resource loading

This makes the project a solid foundation for learning OpenGL or building a custom rendering engine.

---

## Features

* OBJ mesh loading
* CPU mesh representation (`CPUMesh`)
* GPU mesh abstraction (`Mesh`)
* Shader compilation and linking from external files
* Texture sampling support in shaders
* Modern OpenGL (Core Profile, GLSL 330)
* Modular and extensible structure

---

## Project Structure

```
root/
│
├── src/            # Source files (.cpp)
├── include/       # Header files (.h)
├── shaders/      # GLSL shader files
│
├── progetto.sln
├── progetto.vcxproj
```

---

## How it works

### Mesh loading pipeline

**Step 1 — Load from file**

OBJ file → CPUMesh

Contains:

* vertex positions
* texture coordinates
* normals
* face indices

Stored in CPU memory.

---

**Step 2 — Format for OpenGL**

CPUMesh → Vertex array

Data is converted into a format suitable for GPU upload.

---

**Step 3 — Upload to GPU**

Mesh class creates:

* VAO
* VBO
* EBO

Data is transferred to GPU memory.

---

**Step 4 — Rendering**

During rendering:

* Shader program is activated
* Mesh VAO is bound
* Draw call is executed

---

## Shader system

Shaders are stored as external files:

```
shaders/
    vertex.glsl
    fragment.glsl
```

They are:

* loaded from disk
* compiled
* linked into a shader program

---

## Requirements

* Windows
* Visual Studio
* OpenGL 3.3+
* GLFW
* GLAD

---

## Building

Open:

```
progetto.sln
```

in Visual Studio

Then:

```
Build → Build Solution
```

Run:

```
F5
```

---

## Learning goals of this project

This project demonstrates:

* Modern OpenGL pipeline
* GPU resource management
* Shader programming
* Mesh loading
* Engine-style architecture

---

## Future improvements

Possible extensions:

* Texture loading
* Camera system
* Lighting
* Model transforms
* Material system
* Scene management

---

## Author

prbAsalad19

---

This project is experimental and provided as-is.

## License

MIT License
