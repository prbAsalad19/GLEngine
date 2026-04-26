# OpenGLRenderer.h

## Brief Overview

The `OpenGLRenderer` class is a high-performance, professional-grade renderer designed to utilize the OpenGL API for rendering scenes. This renderer is built with extensibility and performance in mind, providing developers with a robust framework for rendering complex scenes using modern graphics techniques.

## Table of Classes/Structs/Enums

### Enums

None defined in this file.

### Structs

- **CameraUBOData**
  - Represents the Uniform Buffer Object (UBO) data for a camera. This includes the view, projection, and view-projection matrices, as well as the position of the camera.

### Classes

- **OpenGLRenderer**
  - The main renderer class responsible for managing rendering operations, shaders, resources, and scene rendering.

## Detailed Methods Section

### Full Signature: `OpenGLRenderer::OpenGLRenderer(ResourceManager& resources, const std::string& vertPath, const std::string& fragPath)`

#### Return Type Explanation
- **Type:** Constructor.
- **Purpose:** Initializes a new instance of the `OpenGLRenderer` class. Takes in a reference to the `ResourceManager`, and paths to vertex and fragment shader files.

#### Internal Logic/Purpose
- The constructor initializes member variables with the provided resources and shaders.
- It sets up necessary OpenGL resources such as UBOs for camera data.

### Full Signature: `OpenGLRenderer::~OpenGLRenderer()`

#### Return Type Explanation
- **Type:** Destructor.
- **Purpose:** Cleans up all allocated resources when an instance of `OpenGLRenderer` is destroyed.

#### Internal Logic/Purpose
- Releases any GPU resources (e.g., UBOs, shader programs).
- Ensures that all OpenGL bindings are properly unbound before destruction.

### Full Signature: `void OpenGLRenderer::init()`

#### Return Type Explanation
- **Type:** Void.
- **Purpose:** Initializes the renderer. This method is typically called once at the start of rendering operations.

#### Internal Logic/Purpose
- Initializes shaders, UBOs, and other necessary resources.
- Sets up OpenGL context-specific configurations.

### Full Signature: `void OpenGLRenderer::shutdown()`

#### Return Type Explanation
- **Type:** Void.
- **Purpose:** Shuts down the renderer. This method is typically called at the end of rendering operations to clean up.

#### Internal Logic/Purpose
- Cleans up all resources managed by the renderer.
- Resets any global state that may have been altered during operation.

### Full Signature: `void OpenGLRenderer::onResize(unsigned int width, unsigned int height)`

#### Return Type Explanation
- **Type:** Void.
- **Purpose:** Handles window resize events. This method updates the renderer's viewport and projection matrix to accommodate new dimensions.

#### Internal Logic/Purpose
- Updates the internal width and height members with the new dimensions.
- Recalculates the camera's view-projection matrix.
- Adjusts any other state that depends on the screen size.

### Full Signature: `void OpenGLRenderer::render(const Scene& scene, const Camera& camera)`

#### Return Type Explanation
- **Type:** Void.
- **Purpose:** Renders a given scene using the provided camera. This method handles all rendering logic for the specified scene.

#### Internal Logic/Purpose
- Binds necessary shaders and UBOs.
- Updates shader uniforms with current camera and transform data.
- Iterates through the scene's objects, setting up transformations and drawing each one.
- Unbinds shaders and resets state after rendering.