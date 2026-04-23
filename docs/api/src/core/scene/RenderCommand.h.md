## RenderCommand.h

### Brief Overview

The `RenderCommand` struct represents the minimal unit of work for the renderer. It encapsulates all necessary information to issue a single draw call, independent of any scene-specific data. The structure is designed to be lightweight and efficient, allowing for rapid rendering commands generation and execution.

### Table of Classes/Structs/Enums

- **RenderCommand**: A struct that represents a single drawing command.

### Detailed Methods Section

#### 1. Full Signature
```cpp
struct RenderCommand
{
    uint32_t      transformIndex;
    MeshHandle    mesh;
    MaterialHandle material;
};
```

#### 2. Return Type Explanation
- **None**: `RenderCommand` is a struct and does not return any value.

#### 3. Internal Logic/Purpose
The `RenderCommand` struct contains the following members:

- **transformIndex** (`uint32_t`): This member stores the index into the GPU-side transform buffer (UBO). The actual `mat4` transformation matrix lives in VRAM, and this is just a slot number that points to it.
  
- **mesh** (`MeshHandle`): This member holds a handle resolved by the `ResourceManager` to an OpenGL mesh. It represents the geometry data to be drawn.

- **material** (`MaterialHandle`): This member stores a handle resolved by the `ResourceManager` to a material. The material defines how the mesh should be rendered, including shading, textures, and other rendering properties.

### Example Usage

```cpp
// Create a RenderCommand instance
RenderCommand renderCmd;

// Set transform index
renderCmd.transformIndex = 0; // Assuming the first transform slot is used

// Get mesh handle from ResourceManager
MeshHandle meshHandle = ResourceManager::GetInstance()->LoadMesh("path/to/mesh.obj");
renderCmd.mesh = meshHandle;

// Get material handle from ResourceManager
MaterialHandle matHandle = ResourceManager::GetInstance()->LoadMaterial("path/to/material.mat");
renderCmd.material = matHandle;

// Issue the render command (pseudo-code)
Renderer::SubmitRenderCommand(renderCmd);
```

This example demonstrates how to create a `RenderCommand` instance, set its properties, and submit it for rendering. The actual rendering process would involve using these parameters to issue draw calls to the GPU.