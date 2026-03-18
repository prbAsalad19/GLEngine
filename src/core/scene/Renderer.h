#pragma once
#include <span>
#include <vector>

#include "opengl/OpenGLConfig.h"
#include "core/scene/RenderCommand.h"
#include "core/assets/Camera.h"
#include "core/resourcemanager/ResourceManager.h"

// Maximum number of objects that can be rendered in a single frame.
// This defines the size of the transform UBO pre-allocated in VRAM.
// Increase if scenes grow beyond this limit.
static constexpr uint32_t MAX_RENDER_OBJECTS = 1024;

// CameraUBOData is the exact memory layout uploaded to the camera UBO.
// The 'std140' layout in GLSL requires 16-byte alignment for all members.
// mat4 is 64 bytes, Vector3 needs 4 bytes of padding to reach 16 bytes.
// This struct must match the 'CameraData' uniform block in the shader exactly.
struct CameraUBOData
{
    float view[16];             // mat4 — 64 bytes
    float projection[16];       // mat4 — 64 bytes
    float viewProjection[16];   // mat4 — 64 bytes, pre-multiplied on CPU to save GPU work
    float cameraPosition[3];    // vec3 — 12 bytes
    float _padding;             // 4 bytes padding to reach 16-byte alignment
};

// Renderer consumes a list of RenderCommands and issues GPU draw calls.
// It knows nothing about the Scene — only about commands, resources, and GPU state.
//
// Ownership:
//   - ResourceManager is owned externally, passed by reference
//   - shaderProgram is a raw GL handle for now (will become ShaderProgram class later)
//   - UBO handles are owned by Renderer and released in the destructor

class Renderer
{
public:
    Renderer(ResourceManager& resourceManager, GLuint shaderProgram);
    ~Renderer();

    // Must be called once after OpenGL is initialized.
    // Allocates the two UBOs in VRAM.
    void initialize();

    // Called every frame from the main loop.
    // - uploads updated camera matrices to the camera UBO
    // - uploads the transform buffer to the transform UBO
    // - iterates commands and issues draw calls
    void render(std::span<const RenderCommand> commands, const Camera& camera, float aspectRatio);

    // Pointer to the CPU-side staging buffer for transform matrices.
    // Scene::gatherRenderCommands writes model matrices here.
    // Renderer uploads this to VRAM at the start of render().
    mat4 transformStagingBuffer[MAX_RENDER_OBJECTS];

private:
    ResourceManager& m_resourceManager;
    GLuint           m_shader;

    // GPU buffer handles (VRAM)
    GLuint m_cameraUBO;     // binding point 0 — camera matrices, updated every frame
    GLuint m_transformUBO;  // binding point 1 — array of model mat4, updated every frame

    void uploadCameraData(const Camera& camera, float aspectRatio);
    void uploadTransformData(uint32_t count);
};