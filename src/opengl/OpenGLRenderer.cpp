#include "OpenGLRenderer.h"
#include "core/assets/Material.h"


OpenGLRenderer::OpenGLRenderer(ResourceManager& resources,
    const std::string& vertPath,
    const std::string& fragPath)
    : m_resources(resources)
    , m_shader(vertPath, fragPath)
    , m_cameraUBO(0)
	, m_transformUBO(0)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
    glDeleteBuffers(1, &m_cameraUBO);
    glDeleteBuffers(1, &m_transformUBO);
}

void OpenGLRenderer::init()
{
	//-- OpenGL state setup (depth test, blending, culling) ───────────────────────────────────────────────────────
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEBUG_OUTPUT);

	//-- UBO setup ────────────────────────────────────────────────────────────────────────────────────────────────
	glCreateBuffers(1, &m_cameraUBO);
	glNamedBufferStorage(m_cameraUBO, sizeof(CameraUBOData), nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, CAMERA_UBO_BINDING, m_cameraUBO);

    glCreateBuffers(1, &m_transformUBO);
    glNamedBufferStorage(m_transformUBO, sizeof(mat4) * MAX_RENDER_OBJECTS, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, TRANSFORM_UBO_BINDING, m_transformUBO);


    m_shader.bind();
	m_shader.setUniformBlockBinding("CameraData", CAMERA_UBO_BINDING);
	m_shader.setUniformBlockBinding("TransformData", TRANSFORM_UBO_BINDING);

    m_shader.setInt("material", 0);
    m_shader.setInt("mask", 1);
    m_shader.setInt("useMaskAlpha", 0);
	m_shader.unbind();
}

void OpenGLRenderer::shutdown()
{
    glDeleteBuffers(1, &m_cameraUBO);
	glDeleteBuffers(1, &m_transformUBO);
}

void OpenGLRenderer::onResize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
    glViewport(0, 0, width, height);
}

void OpenGLRenderer::render(const Scene& scene, const Camera& camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (scene.objects.empty()) return;

    // Raggruppa per mesh — stessa mesh = stessa draw call
    // key:   MeshHandle
    // value: lista di { transformIndex, materialHandle }
    std::unordered_map<MeshHandle, std::vector<std::pair<uint32_t, MaterialHandle>>> groups;

    uint32_t index = 0;
    for (const RenderObject& obj : scene.objects)
    {
        if (index >= MAX_RENDER_OBJECTS) break;

        m_transformStagingBuffer[index] = obj.transform.getMatrix();
        groups[obj.mesh].push_back({ index, obj.material });
        ++index;
    }

    // Upload transforms — tutti in un colpo solo
    glNamedBufferSubData(m_transformUBO, 0,
        sizeof(mat4) * index,
        m_transformStagingBuffer);

    // Upload camera UBO — invariato rispetto a prima
    CameraUBOData cameraData;
    mat4 view = camera.getViewMatrix();
    mat4 projection = camera.getProjectionMatrix(
        static_cast<float>(m_width) / static_cast<float>(m_height));
    mat4 vp = projection * view;
    memcpy(cameraData.view, view.entries, sizeof(float) * 16);
    memcpy(cameraData.projection, projection.entries, sizeof(float) * 16);
    memcpy(cameraData.viewProjection, vp.entries, sizeof(float) * 16);
    cameraData.cameraPosition[0] = camera.position.entries[0];
    cameraData.cameraPosition[1] = camera.position.entries[1];
    cameraData.cameraPosition[2] = camera.position.entries[2];
    cameraData._padding = 0.0f;
    glNamedBufferSubData(m_cameraUBO, 0, sizeof(CameraUBOData), &cameraData);

    m_shader.bind();

    for (auto& [meshHandle, instances] : groups)
    {
        OpenGLMesh* mesh = m_resources.getMesh(meshHandle);
        if (!mesh) continue;

        // Per ora usa il materiale della prima istanza del gruppo.
        // Con materiali diversi sulla stessa mesh servirebbe un'altra
        // suddivisione — ma è un caso raro e lo gestirai dopo.
        MaterialHandle matHandle = instances[0].second;
        Material* material = m_resources.getMaterial(matHandle);
        if (material)
        {
            auto it = material->textures.find("albedo");
            if (it != material->textures.end())
            {
                OpenGLTexture* tex = m_resources.getTexture(it->second);
                if (tex) tex->use(0);
            }
        }

        // I transform di questo gruppo sono contigui nel buffer?
        // No — potrebbero essere sparsi. Dobbiamo riordinarli.
        // Copiamo gli indici in un sotto-buffer compatto e aggiorniamo
        // il UBO con solo quelli di questo gruppo, partendo da offset 0.
        std::vector<mat4> groupTransforms;
        groupTransforms.reserve(instances.size());
        for (auto& [tIdx, _] : instances)
            groupTransforms.push_back(m_transformStagingBuffer[tIdx]);

        glNamedBufferSubData(m_transformUBO, 0,
            sizeof(mat4) * groupTransforms.size(),
            groupTransforms.data());

        mesh->drawInstanced(static_cast<uint32_t>(instances.size()));
    }

    m_shader.unbind();
}