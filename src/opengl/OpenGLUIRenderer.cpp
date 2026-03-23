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

bool OpenGLUIRenderer::loadFont(const std::string& filepath, float pixelHeight)
{
    if (!m_fontAtlas.load(filepath, pixelHeight))
        return false;

    m_gpuFontAtlas = std::make_unique<OpenGLFontAtlas>(m_fontAtlas);
    return true;
}

void OpenGLUIRenderer::render(const UICanvas& canvas)
{
    // Salva e disabilita depth test — l'UI va sempre sopra la scena 3D
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);   // ← AGGIUNGI QUESTO
    // Assicura blending attivo per l'alpha del testo
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();

    m_shader.setVec2("screenSize", { static_cast<float>(m_width), static_cast<float>(m_height) });

    // Pass 1: elementi solidi — nessuna texture, uIsText = 0
    m_shader.setInt("uTexture", 0);



    UIQuadBatch solidBatch;
    for (const UIElement& el : canvas.getElements())
    {
        if (el.type != UIElementType::Label)
            solidBatch.push(el);
    }
    if (!solidBatch.getVertices().empty())
        uploadAndDraw(solidBatch);

    // Pass 2: testo — font texture su unit 0, uIsText = 1
    if (m_gpuFontAtlas)
    {
        m_shader.setInt("uIsText", 1);
        m_shader.setInt("uTexture", 0);


        UIQuadBatch textBatch;
        for (const UIElement& el : canvas.getElements())
        {
            if (el.type == UIElementType::Label)
            {
                std::string displayText = canvas.getString(el.id);
                if (displayText.empty()) displayText = el.text;
                textBatch.pushLabel(el, m_fontAtlas, displayText);
            }
        }
        if (!textBatch.getVertices().empty())
        {
            m_gpuFontAtlas->bind(0);
            //std::cout << "[UIRenderer] textBatch vertices: " << textBatch.getVertices().size()
            //    << " indices: " << textBatch.getIndices().size() << "\n";
            uploadAndDraw(textBatch);
            //GLenum err;
            //while ((err = glGetError()) != GL_NO_ERROR)
            //    std::cout << "[UIRenderer] GL error after text draw: " << err << "\n";
        }
    }
    //std::cout << "[UIRenderer] screenSize: " << m_width << "x" << m_height << "\n";
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
        std::cout << "[UIRenderer] OpenGL error: " << err << "\n";

    m_shader.unbind();

    // Ripristina depth test per il renderer 3D al prossimo frame
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void OpenGLUIRenderer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // Controlliamo solo il click sinistro premuto
    if (button != GLFW_MOUSE_BUTTON_LEFT || action != GLFW_PRESS)
        return;

    // Recuperiamo il puntatore alla Canvas dal user pointer di GLFW
    UICanvas* canvas = static_cast<UICanvas*>(glfwGetWindowUserPointer(window));
    if (!canvas) return;

    // Flag per capire se ci sono pulsanti attivi
    bool clickable = canvas->hasClickableButtons(); // implementa tu questa funzione
    if (!clickable) return;

    // Otteniamo posizione del mouse
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Chiamata alla funzione interna che scorre i pulsanti
    canvas->processClickAt(xpos, ypos); // implementi tu qui la logica che ritorna l'ID e dispatch
}

void OpenGLUIRenderer::onResize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
}

void OpenGLUIRenderer::uploadAndDraw(const UIQuadBatch& batch)
{
    const auto& verts = batch.getVertices();
    const auto& indices = batch.getIndices();
    if (verts.empty()) return;

    glNamedBufferSubData(m_VBO, 0,
        verts.size() * sizeof(UIVertex), verts.data());
    glNamedBufferSubData(m_EBO, 0,
        indices.size() * sizeof(unsigned int), indices.data());

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES,
        static_cast<GLsizei>(indices.size()),
        GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void OpenGLUIRenderer::shutdown()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}