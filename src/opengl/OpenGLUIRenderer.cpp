#include "OpenGLUIRenderer.h"

OpenGLUIRenderer::OpenGLUIRenderer(ResourceManager& resources, unsigned int width, unsigned int height, const std::string& vertPath, const std::string& fragPath)
    : m_shader(vertPath, fragPath)
    , m_width(width)
    , m_height(height)
    , m_resources(resources)
{
}

OpenGLUIRenderer::~OpenGLUIRenderer() {
}

void OpenGLUIRenderer::init()
{
    // crea i buffer
    glCreateVertexArrays(1, &m_VAO);
    glCreateBuffers(1, &m_VBO);
    glCreateBuffers(1, &m_EBO);

    // alloca il VBO dinamico — GL_DYNAMIC_STORAGE_BIT permette di aggiornarlo dopo
    // MAX_UI_VERTICES è una costante tipo 4096 — abbastanza per tutti i quad UI
    glNamedBufferStorage(m_VBO, sizeof(UIVertex) * MAX_UI_VERTICES, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(m_EBO, sizeof(unsigned int) * MAX_UI_INDICES, nullptr, GL_DYNAMIC_STORAGE_BIT);

    // collega VBO al VAO
    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, sizeof(UIVertex));

    // attributo 0 — position (Vector2 = 2 float)
    glEnableVertexArrayAttrib(m_VAO, 0);
    glVertexArrayAttribFormat(m_VAO, 0, 2, GL_FLOAT, GL_FALSE, offsetof(UIVertex, position));
    glVertexArrayAttribBinding(m_VAO, 0, 0);

    // attributo 1 — uv (Vector2 = 2 float)
    glEnableVertexArrayAttrib(m_VAO, 1);
    glVertexArrayAttribFormat(m_VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(UIVertex, uv));
    glVertexArrayAttribBinding(m_VAO, 1, 0);

    // attributo 2 — color (4 float)
    glEnableVertexArrayAttrib(m_VAO, 2);
    glVertexArrayAttribFormat(m_VAO, 2, 4, GL_FLOAT, GL_FALSE, offsetof(UIVertex, color));
    glVertexArrayAttribBinding(m_VAO, 2, 0);

    // collega EBO al VAO
    glVertexArrayElementBuffer(m_VAO, m_EBO);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
        std::cout << "[UIRenderer] init OpenGL error: " << err << "\n";
}

void OpenGLUIRenderer::render(const UICanvas& canvas)
{
    m_batch.buildFromCanvas(canvas);

    glNamedBufferSubData(m_VBO, 0,
        m_batch.getVertices().size() * sizeof(UIVertex),
        m_batch.getVertices().data());

    glNamedBufferSubData(m_EBO, 0,
        m_batch.getIndices().size() * sizeof(unsigned int),
        m_batch.getIndices().data());

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    m_shader.bind();

    // in render() dopo m_shader.bind()
    //GLenum err;
    //while ((err = glGetError()) != GL_NO_ERROR)
    //    std::cout << "[UIRenderer] render OpenGL error: " << err << "\n";

    m_shader.setVec2("screenSize", { (float)m_width, (float)m_height });

    //std::cout << "[UIRenderer] Drawing " << m_batch.getIndices().size() << " indices\n";

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES,
        static_cast<GLsizei>(m_batch.getIndices().size()),
        GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

    //std::cout << "[UIRenderer] VAO=" << m_VAO
    //    << " VBO=" << m_VBO
    //    << " EBO=" << m_EBO
    //    << " shader valid=" << (m_shader.getProgramID() != 0) << "\n";
}

void OpenGLUIRenderer::shutdown()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}