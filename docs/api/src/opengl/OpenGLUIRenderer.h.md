## OpenGLUIRenderer.h

### Brief Overview

This header file defines the `OpenGLUIRenderer` class, which is responsible for rendering user interface elements using OpenGL. It includes methods for initializing, loading fonts, rendering UI quads, handling resize events, and shutting down the renderer.

### Table of Classes/Structs/Enums

- **Classes:**
  - `OpenGLUIRenderer`

- **Structs/Enums:**
  - None

### Detailed Methods Section

#### Constructor

**Full Signature:**
```cpp
OpenGLUIRenderer(ResourceManager& m_resources, unsigned int width, unsigned int height, const std::string& vertPath, const std::string& fragPath);
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Initializes the renderer with a reference to the resource manager, initial window dimensions, and paths to vertex and fragment shader files.

#### Destructor

**Full Signature:**
```cpp
~OpenGLUIRenderer();
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Cleans up resources and shuts down the renderer.

#### Initialize Method

**Full Signature:**
```cpp
void init();
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Sets up OpenGL buffers (VAO, VBO, EBO), compiles shaders, and initializes other necessary components for rendering.

#### Load Font Method

**Full Signature:**
```cpp
bool loadFont(const std::string& filepath, float pixelHeight);
```

**Return Type Explanation:**  
- **bool**: Returns `true` if the font is loaded successfully, otherwise `false`.

**Internal Logic/Purpose:**  
- Loads a font from a file and initializes it with the specified pixel height. Handles the creation of a GPU-based font atlas.

#### Render Method

**Full Signature:**
```cpp
void render(const UICanvas& canvas);
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Renders the UI elements contained in the provided `UICanvas` object using OpenGL. Batches and uploads quads to the GPU for rendering.

#### On Resize Method

**Full Signature:**
```cpp
void onResize(unsigned int width, unsigned int height);
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Updates the renderer's dimensions when the window is resized. Handles the reconfiguration of OpenGL buffers and viewport settings.

#### Shutdown Method

**Full Signature:**
```cpp
void shutdown();
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Cleans up and releases all resources held by the renderer, ensuring proper cleanup before shutting down.

### Private Methods Section

#### Upload and Draw Method

**Full Signature:**
```cpp
void uploadAndDraw(const UIQuadBatch& batch);
```

**Return Type Explanation:**  
- **None**

**Internal Logic/Purpose:**  
- Binds OpenGL buffers, uploads vertex data from the `UIQuadBatch` to the GPU, and draws the quad batch using the currently loaded shader program.