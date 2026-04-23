# OpenGLUIRenderer.cpp

## Brief Overview
This file defines the `OpenGLUIRenderer` class, responsible for rendering user interface (UI) elements using OpenGL. The class manages shaders, buffers, and rendering logic to display UI components such as text labels and solid quads on a 2D canvas.

## Table of Classes/Structs/Enums
- **OpenGLUIRenderer**: Manages the rendering of UI elements.
- **UIVertex**: Represents a vertex for UI rendering, containing position, UV coordinates, and color information.
- **UICanvas**: Holds the current state of the UI, including elements to be rendered.
- **UIElement**: Represents an individual UI element, such as labels or buttons.
- **UILabel**: Specific type of `UIElement` representing a label with text.
- **UIQuadBatch**: Manages batches of vertices and indices for rendering quad-based UI elements.

## Detailed Methods Section

### OpenGLUIRenderer::OpenGLUIRenderer
```cpp
OpenGLUIRenderer::OpenGLUIRenderer(ResourceManager& resources, unsigned int width, unsigned int height, const std::string& vertPath, const std::string& fragPath)
    : m_shader(vertPath, fragPath)
    , m_width(width)
    , m_height(height)
    , m_resources(resources)
{
}
```
- **Full signature**: `OpenGLUIRenderer::OpenGLUIRenderer(ResourceManager& resources, unsigned int width, unsigned int height, const std::string& vertPath, const std::string& fragPath)`
- **Return type explanation**: Constructor for `OpenGLUIRenderer`.
- **Internal logic/purpose**: Initializes the renderer with shader paths and dimensions. Sets up member variables and loads the initial shader.

### OpenGLUIRenderer::~OpenGLUIRenderer
```cpp
OpenGLUIRenderer::~OpenGLUIRenderer() {
}
```
- **Full signature**: `OpenGLUIRenderer::~OpenGLUIRenderer()`
- **Return type explanation**: Destructor for `OpenGLUIRenderer`.
- **Internal logic/purpose**: Cleans up resources when the renderer is destroyed.

### OpenGLUIRenderer::init
```cpp
void OpenGLUIRenderer::init()
{
    // Create buffers
    glCreateVertexArrays(1, &m_VAO);
    glCreateBuffers(1, &m_VBO);
    glCreateBuffers(1, &m_EBO);

    // Allocate dynamic storage for VBO and EBO
    glNamedBufferStorage(m_VBO, sizeof(UIVertex) * MAX_UI_VERTICES, nullptr, GL_DYNAMIC_STORAGE_BIT);
    glNamedBufferStorage(m_EBO, sizeof(unsigned int) * MAX_UI_INDICES, nullptr, GL_DYNAMIC_STORAGE_BIT);

    // Bind VBO to VAO and set vertex attributes
    glVertexArrayVertexBuffer(m_VAO, 0, m_VBO, 0, sizeof(UIVertex));
    glEnableVertexArrayAttrib(m_VAO, 0);
    glVertexArrayAttribFormat(m_VAO, 0, 2, GL_FLOAT, GL_FALSE, offsetof(UIVertex, position));
    glVertexArrayAttribBinding(m_VAO, 0, 0);

    // Bind EBO to VAO
    glVertexArrayElementBuffer(m_VAO, m_EBO);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
        std::cout << "[UIRenderer] init OpenGL error: " << err << "\n";
}
```
- **Full signature**: `OpenGLUIRenderer::init()`
- **Return type explanation**: Initializes OpenGL buffers and sets up the vertex array object.
- **Internal logic/purpose**: Creates VAO, VBO, and EBO. Allocates memory for vertices and indices using dynamic storage. Binds buffers to the VAO and configures vertex attributes.

### OpenGLUIRenderer::loadFont
```cpp
bool OpenGLUIRenderer::loadFont(const std::string& filepath, float pixelHeight)
{
    if (!m_fontAtlas.load(filepath, pixelHeight))
        return false;

    m_gpuFontAtlas = std::make_unique<OpenGLFontAtlas>(m_fontAtlas);
    return true;
}
```
- **Full signature**: `OpenGLUIRenderer::loadFont(const std::string& filepath, float pixelHeight)`
- **Return type explanation**: Loads a font atlas and creates an OpenGL-specific font atlas.
- **Internal logic/purpose**: Attempts to load a font from the specified path. If successful, initializes the GPU font atlas for rendering text.

### OpenGLUIRenderer::render
```cpp
void OpenGLUIRenderer::render(const UICanvas& canvas)
{
    // Disable depth test and enable blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_shader.bind();

    m_shader.setVec2("screenSize", { static_cast<float>(m_width), static_cast<float>(m_height) });

    // Render solid UI elements (non-text)
    UIQuadBatch solidBatch;
    for (const UIElement& el : canvas.getElements())
    {
        if (el.type != UIElementType::Label)
            solidBatch.push(el);
    }
    if (!solidBatch.getVertices().empty())
        uploadAndDraw(solidBatch);

    // Render text elements
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
            uploadAndDraw(textBatch);
        }
    }

    m_shader.unbind();

    // Re-enable depth test for 3D rendering
    glEnable(GL_DEPTH_TEST);
}
```
- **Full signature**: `OpenGLUIRenderer::render(const UICanvas& canvas)`
- **Return type explanation**: Renders the UI elements in the given canvas.
- **Internal logic/purpose**: Disables depth testing and blending to render UI elements. Separates solid UI elements from text elements, rendering them separately using `uploadAndDraw`.

### OpenGLUIRenderer::onResize
```cpp
void OpenGLUIRenderer::onResize(unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;
}
```
- **Full signature**: `OpenGLUIRenderer::onResize(unsigned int width, unsigned int height)`
- **Return type explanation**: Handles window resize events.
- **Internal logic/purpose**: Updates the renderer's dimensions to match the new window size.

### OpenGLUIRenderer::uploadAndDraw
```cpp
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
```
- **Full signature**: `OpenGLUIRenderer::uploadAndDraw(const UIQuadBatch& batch)`
- **Return type explanation**: Uploads vertex and index data to the GPU and draws the batch.
- **Internal logic/purpose**: Updates the VBO and EBO with new data from the quad batch. Binds the VAO and draws the batch using indexed drawing.

### OpenGLUIRenderer::shutdown
```cpp
void OpenGLUIRenderer::shutdown()
{
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}
```
- **Full signature**: `OpenGLUIRenderer::shutdown()`
- **Return type explanation**: Cleans up OpenGL resources.
- **Internal logic/purpose**: Deletes the VAO and buffers to free up GPU memory.