#pragma once
#include "core/resourcemanager/ResourceHandle.h"

// A RenderCommand is the minimal unit of work for the renderer.
// It contains everything needed to issue a single draw call,
// with no scene knowledge required.
//
// transformIndex: index into the GPU-side transform buffer (UBO).
//                 The actual mat4 lives in VRAM; this is just the slot number.
// mesh:           handle resolved by ResourceManager to an OpenGLMesh.
// material:       handle resolved by ResourceManager to a Material.

struct RenderCommand
{
    uint32_t      transformIndex;
    MeshHandle    mesh;
    MaterialHandle material;
};