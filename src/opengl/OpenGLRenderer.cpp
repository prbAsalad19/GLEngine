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

    // construction of the render command and staging buffer
	std::vector<RenderCommand> commands;
	commands.reserve(scene.objects.size());

    uint32_t index = 0;
    for (const RenderObject& obj : scene.objects)
    {
        if (index >= MAX_RENDER_OBJECTS)
        {
            std::cout << "Warning: scene has more than " << MAX_RENDER_OBJECTS
                << " objects; some will not be rendered.\n";
            break;
		}

		mat4 model = obj.transform.getMatrix();
		m_transformStagingBuffer[index] = model;

        RenderCommand cmd;
		cmd.transformIndex = index;
        cmd.mesh = obj.mesh;
        cmd.material = obj.material;
		commands.push_back(cmd);

		++index;
    }


    CameraUBOData cameraData;
    mat4 view = camera.getViewMatrix();
    mat4 projection = camera.getProjectionMatrix(
		static_cast<float>(m_width) / static_cast<float>(m_height));
	mat4 vp = projection * view; // pre-multiplied on CPU — saves work in every shader invocation

    memcpy(cameraData.view, view.entries, sizeof(float) * 16);
    memcpy(cameraData.projection, projection.entries, sizeof(float) * 16);
	memcpy(cameraData.viewProjection, vp.entries, sizeof(float) * 16);
    cameraData.cameraPosition[0] = camera.position.entries[0];
    cameraData.cameraPosition[1] = camera.position.entries[1];
    cameraData.cameraPosition[2] = camera.position.entries[2];
    cameraData._padding = 0.0f;

    glNamedBufferSubData(m_cameraUBO, 0, sizeof(CameraUBOData), &cameraData);

    glNamedBufferSubData(m_transformUBO, 0,
        sizeof(mat4) * index,
        m_transformStagingBuffer);

    m_shader.bind();

    for (const RenderCommand& cmd : commands)
    {
        OpenGLMesh* mesh = m_resources.getMesh(cmd.mesh);
        Material* material = m_resources.getMaterial(cmd.material);
        if (!mesh || !material) continue;

		m_shader.setUInt("transformIndex", cmd.transformIndex);

        auto it = material->textures.find("albedo");
        if (it != material->textures.end())
        {
            OpenGLTexture* tex = m_resources.getTexture(it->second);
            if (tex) tex->use(0);
        }

		mesh->draw();
	}

    m_shader.bind();
}