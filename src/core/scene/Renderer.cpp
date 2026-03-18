#include "Renderer.h"
#include "opengl/OpenGLMesh.h"
#include "opengl/OpenGLTexture.h"
#include "core/assets/Material.h"
#include <cstring>
#include <iostream>

// UBO binding point conventions (must match shader layout bindings):
//   0 = CameraData   (view, projection, viewProjection, cameraPosition)
//   1 = TransformData (array of model mat4)
static constexpr GLuint CAMERA_UBO_BINDING = 0;
static constexpr GLuint TRANSFORM_UBO_BINDING = 1;

Renderer::Renderer(ResourceManager& resourceManager, GLuint shaderProgram)
    : m_resourceManager(resourceManager)
    , m_shader(shaderProgram)
    , m_cameraUBO(0)
    , m_transformUBO(0)
{
    // UBOs are NOT created here.
    // OpenGL objects must be created after a valid GL context exists,
    // which is guaranteed only after glfwMakeContextCurrent().
    // initialize() is called explicitly after window creation.
}

Renderer::~Renderer()
{
    // Release GPU memory. glDeleteBuffers silently ignores 0 handles,
    // so this is safe even if initialize() was never called.
    glDeleteBuffers(1, &m_cameraUBO);
    glDeleteBuffers(1, &m_transformUBO);
}

void Renderer::initialize()
{
    // ── Camera UBO ────────────────────────────────────────────────────────────
    // Allocate a buffer in VRAM sized exactly for one CameraUBOData struct.
    // GL_DYNAMIC_DRAW tells the driver this buffer will be rewritten every frame
    // (as opposed to GL_STATIC_DRAW for data that never changes).
    // Passing nullptr as data creates the buffer empty — we fill it in render().

    glGenBuffers(1, &m_cameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraUBOData), nullptr, GL_DYNAMIC_DRAW);

    // Bind the buffer to binding point 0 permanently.
    // From this point on, any shader that declares:
    //   layout(std140, binding = 0) uniform CameraData { ... }
    // will read from this buffer.
    glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_UBO_BINDING, m_cameraUBO);

    // ── Transform UBO ─────────────────────────────────────────────────────────
    // Allocate space for MAX_RENDER_OBJECTS model matrices.
    // Each mat4 is 16 floats = 64 bytes.
    // Total = MAX_RENDER_OBJECTS * 64 bytes.

    glGenBuffers(1, &m_transformUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_transformUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * MAX_RENDER_OBJECTS, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM_UBO_BINDING, m_transformUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, 0); // unbind — good practice

    // ── Tell the shader which binding points to use ───────────────────────────
    // Even if the shader uses 'layout(binding = N)', it's good practice to
    // set this explicitly for compatibility with older GL drivers.
    glUseProgram(m_shader);

    GLuint cameraBlockIndex = glGetUniformBlockIndex(m_shader, "CameraData");
    GLuint transformBlockIndex = glGetUniformBlockIndex(m_shader, "TransformData");

    if (cameraBlockIndex != GL_INVALID_INDEX)
        glUniformBlockBinding(m_shader, cameraBlockIndex, CAMERA_UBO_BINDING);

    if (transformBlockIndex != GL_INVALID_INDEX)
        glUniformBlockBinding(m_shader, transformBlockIndex, TRANSFORM_UBO_BINDING);

    // Bind the albedo texture sampler to texture unit 0.
    // This tells the shader: "when you sample 'albedoTexture', use unit 0".
    glUniform1i(glGetUniformLocation(m_shader, "albedoTexture"), 0);

    glUseProgram(0);
}

void Renderer::uploadCameraData(const Camera& camera, float aspectRatio)
{
    CameraUBOData data;

    mat4 view = camera.getViewMatrix();
    mat4 projection = camera.getProjectionMatrix(aspectRatio);
    mat4 vp = projection * view; // pre-multiplied on CPU — saves work in every shader invocation

    // Copy matrices into the tightly-packed struct.
    // memcpy is used because mat4::entries is a float[16], not a float*.
    memcpy(data.view, view.entries, sizeof(float) * 16);
    memcpy(data.projection, projection.entries, sizeof(float) * 16);
    memcpy(data.viewProjection, vp.entries, sizeof(float) * 16);

    data.cameraPosition[0] = camera.position.entries[0];
    data.cameraPosition[1] = camera.position.entries[1];
    data.cameraPosition[2] = camera.position.entries[2];
    data._padding = 0.0f;

    // glBufferSubData rewrites the contents of the existing GPU buffer
    // without destroying and recreating it — this is the correct pattern
    // for data that changes every frame.
    glBindBuffer(GL_UNIFORM_BUFFER, m_cameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(CameraUBOData), &data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::uploadTransformData(uint32_t count)
{
    if (count == 0) return;

    // Upload only the range we actually use (count matrices, not MAX_RENDER_OBJECTS).
    // This avoids uploading megabytes of unused data every frame.
    glBindBuffer(GL_UNIFORM_BUFFER, m_transformUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4) * count, transformStagingBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Renderer::render(std::span<const RenderCommand> commands, const Camera& camera, float aspectRatio)
{
    if (commands.empty()) return;

    // ── 1. Upload per-frame GPU data ──────────────────────────────────────────
    uploadCameraData(camera, aspectRatio);
    uploadTransformData(static_cast<uint32_t>(commands.size()));

    // ── 2. Bind shader ────────────────────────────────────────────────────────
    glUseProgram(m_shader);

    // ── 3. Issue draw calls ───────────────────────────────────────────────────
    for (const RenderCommand& cmd : commands)
    {
        // Resolve handles → actual GPU objects via ResourceManager.
        OpenGLMesh* mesh = m_resourceManager.getMesh(cmd.mesh);
        Material* material = m_resourceManager.getMaterial(cmd.material);

        if (!mesh || !material) continue; // skip invalid commands gracefully

        // Pass the transform index as a regular uniform.
        // The vertex shader uses this to index into the TransformData UBO array
        // and fetch the correct model matrix for this object.
        glUniform1ui(glGetUniformLocation(m_shader, "transformIndex"), cmd.transformIndex);

        // Bind the albedo texture to unit 0.
        // Only the albedo slot is used for now — more slots added with PBR materials.
        auto albedoIt = material->textures.find("albedo");
        if (albedoIt != material->textures.end())
        {
            OpenGLTexture* tex = m_resourceManager.getTexture(albedoIt->second);
            if (tex) tex->use(0);
        }

        // Issue the draw call. OpenGLMesh::draw() binds its own VAO internally.
        mesh->draw();
    }

    glUseProgram(0);
}