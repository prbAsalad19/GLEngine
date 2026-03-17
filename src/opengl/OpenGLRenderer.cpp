#include "OpenGLRenderer.h"
#include "core/assets/Material.h"

OpenGLRenderer::OpenGLRenderer(ResourceManager& resources,
    const std::string& vertPath,
    const std::string& fragPath)
    : m_resources(resources)
    , m_shader(vertPath, fragPath)
{
}

void OpenGLRenderer::init()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    m_shader.bind();
    m_shader.setInt("material", 0);
    m_shader.setInt("mask", 1);
    m_shader.setInt("useMaskAlpha", 0);
}

void OpenGLRenderer::shutdown()
{
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

    m_shader.bind();

    m_shader.setMat4("view", camera.getViewMatrix());
    m_shader.setMat4("projection", camera.getProjectionMatrix(
        static_cast<float>(m_width) / static_cast<float>(m_height)));

    for (const RenderObject& obj : scene.objects)
    {
        OpenGLMesh* mesh = m_resources.getMesh(obj.mesh);
        Material* mat = m_resources.getMaterial(obj.material);
        if (!mesh || !mat) continue;

        m_shader.setMat4("model", obj.transform.getMatrix());

        auto it = mat->textures.find("albedo");
        if (it != mat->textures.end())
        {
            OpenGLTexture* tex = m_resources.getTexture(it->second);
            if (tex) tex->use(0);
        }

        mesh->draw();
    }
}