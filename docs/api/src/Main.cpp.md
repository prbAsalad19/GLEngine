## Main.cpp

### Brief Overview
The `Main.cpp` file is the primary entry point for a game engine application. It initializes the GLFW window, OpenGL context, resource management, rendering pipeline, and user interface components. The main loop handles input, updates scene objects, renders the 3D scene and UI, and manages frame rate.

### Table of Classes/Structs/Enums
- `RenderContext`
- `GLFWwindow*`

### Detailed Methods Section

#### Method: createWindow(int width, int height, const char* title)
**Full Signature:** 
```cpp
static GLFWwindow* createWindow(int width, int height, const char* title);
```

**Return Type Explanation:**
- Returns a pointer to the created GLFW window.

**Internal Logic/Purpose:**
This function initializes the GLFW library and creates a new window with the specified dimensions and title. It sets various GLFW hints to specify the OpenGL context version and profile. The function loads the OpenGL functions using `gladLoadGLLoader` and configures the OpenGL settings, including clearing color and enabling vertical synchronization (VSync). If any step fails, it prints an error message and returns `nullptr`.

#### Method: main()
**Full Signature:** 
```cpp
int main();
```

**Return Type Explanation:**
- Returns 0 on successful execution.

**Internal Logic/Purpose:**
This is the main function that initializes the engine components, loads resources, sets up the rendering loop, and handles events. It initializes the GLFW window, resource manager, renderer, and UI renderer. The main loop processes user input, updates scene objects, renders the 3D scene and UI, and manages frame rate. On exit, it cleans up by shutting down the renderer, destroying the window, and terminating GLFW.

### Additional Notes
- The `RenderContext` struct is used to pass rendering context information between the GLFW window and the rendering components.
- The `main()` function includes a fixed timestep loop for deterministic physics updates.
- Debugging code is commented out but can be enabled for development purposes.