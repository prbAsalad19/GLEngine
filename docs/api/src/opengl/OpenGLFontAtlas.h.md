### OpenGLFontAtlas.h

#### Brief Overview

The `OpenGLFontAtlas` class provides a GPU-side font atlas for rendering text using OpenGL. It uploads the RGBA bitmap from a CPU-based `FontAtlas` to an OpenGL texture, enabling linear filtering for sub-pixel smoothness. The texture is then bound as a regular `sampler2D` in the UI shader.

#### Table of Classes/Structs/Enums

- **Class:** `OpenGLFontAtlas`

#### Detailed Methods Section

##### Constructor
```cpp
explicit OpenGLFontAtlas(const FontAtlas& atlas);
```
**Return Type Explanation:** None (Constructor)
**Internal Logic/Purpose:** Initializes an instance of `OpenGLFontAtlas` with a reference to the CPU-side `FontAtlas`. The constructor also uploads the font data to an OpenGL texture and sets up filtering for sub-pixel smoothness.

##### Destructor
```cpp
~OpenGLFontAtlas();
```
**Return Type Explanation:** None (Destructor)
**Internal Logic/Purpose:** Cleans up the OpenGL resources, including deleting the texture.

##### Copy Constructor
```cpp
OpenGLFontAtlas(const OpenGLFontAtlas&) = delete;
```
**Return Type Explanation:** Deleted (Copy Constructor)
**Internal Logic/Purpose:** Prevents copying of `OpenGLFontAtlas` instances to avoid issues with resource management.

##### Assignment Operator
```cpp
OpenGLFontAtlas& operator=(const OpenGLFontAtlas&) = delete;
```
**Return Type Explanation:** Deleted (Assignment Operator)
**Internal Logic/Purpose:** Prevents assignment of `OpenGLFontAtlas` instances, maintaining exclusive ownership of resources.

##### Bind Method
```cpp
void bind(int unit) const;
```
**Return Type Explanation:** None (Void)
**Internal Logic/Purpose:** Binds the OpenGL texture to a specified texture unit, making it accessible in shaders as `sampler2D`.

##### Get CPU Atlas Method
```cpp
const FontAtlas& getCPUAtlas() const { return m_cpuAtlas; }
```
**Return Type Explanation:** Returns a constant reference to the CPU-side `FontAtlas`.
**Internal Logic/Purpose:** Provides read-only access to the CPU-based font atlas data, allowing other components to query it without modifying it.

### Summary

The `OpenGLFontAtlas` class is designed to handle the GPU-side management of font textures for rendering text in a graphical application using OpenGL. It ensures efficient and high-quality text rendering through proper texture handling and filtering.