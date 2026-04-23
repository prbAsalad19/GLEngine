### OpenGLRenderer.cpp

#### Brief Overview
This file contains the implementation of the `OpenGLRenderer` class, which is responsible for rendering 3D scenes using OpenGL. The class manages shaders, uniform buffers, and other resources to efficiently render objects in a scene.

#### Table of Classes/Structs/Enums
- **OpenGLRenderer**: Main class that handles rendering.
- **CameraUBOData**: Struct representing the camera data for the vertex shader.
- **Material**: Class representing a material with textures.
- **Scene**: Class representing a scene containing renderable objects.
- **Camera**: Class representing a camera in the scene.

#### Detailed Methods Section

1. **Constructor**
   - **Full signature**: `OpenGLRenderer::OpenGLRenderer(ResourceManager& resources, const std::string& vertPath, const std::string& fragPath)`
   - **Return type explanation**: None (void)
   - **Internal logic/purpose**: Initializes the renderer with a resource manager and paths to vertex and fragment shader files. Sets up member variables for camera and transform UBOs.

2. **Destructor**
   - **Full signature**: `OpenGLRenderer::~OpenGLRenderer()`
   - **Return type explanation**: None (void)
   - **Internal logic/purpose**: Deletes the camera and transform UBOs to free up resources.

3. **Initialization**
   - **Full signature**: `void OpenGLRenderer::init()`
   - **Return type explanation**: None (void)
   - **Internal logic/purpose**: Sets up OpenGL state (depth test, blending, culling), creates and binds uniform buffers for camera and transforms, and binds shader uniform blocks.

4. **Shutdown**
   - **Full signature**: `void OpenGLRenderer::shutdown()`
   - **Return type explanation**: None (void)
   - **Internal logic/purpose**: Deletes the camera and transform UBOs to free up resources.

5. **Resize Event Handling**
   - **Full signature**: `void OpenGLRenderer::onResize(unsigned int width, unsigned int height)`
   - **Return type explanation**: None (void)
   - **Internal logic/purpose**: Updates the viewport dimensions when the window is resized.

6. **Rendering a Scene**
   - **Full signature**: `void OpenGLRenderer::render(const Scene& scene, const Camera& camera)`
   - **Return type explanation**: None (void)
   - **Internal logic/purpose**:
     - Clears the color and depth buffers.
     - Groups render objects by mesh for efficient rendering.
     - Uploads transform data to the UBO.
     - Updates camera data in the UBO.
     - Renders each mesh with its associated material.
     - Unbinds the shader after rendering.

This class provides a comprehensive solution for rendering 3D scenes using OpenGL, handling shaders, uniform buffers, and scene objects efficiently.