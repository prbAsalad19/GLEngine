# OpenGLTexture.cpp

## Brief Overview

This file contains the implementation for `OpenGLTexture`, a class responsible for loading and managing textures in an OpenGL context. It provides methods to create a texture from a file, delete it, and use it during rendering.

## Table of Classes/Structs/Enums

### Class: OpenGLTexture

- **Purpose**: Manages OpenGL textures, allowing their creation from files and deletion.
- **Methods**:
  - `OpenGLTexture(const char* filename)`: Constructor that loads a texture from a file.
  - `~OpenGLTexture()`: Destructor that cleans up the texture resources.
  - `void use(int unit)`: Binds the texture to a specified texture unit.

## Detailed Methods Section

### Method: OpenGLTexture(const char* filename)

**Full Signature**:
```cpp
OpenGLTexture::OpenGLTexture(const char* filename)
```

**Return Type Explanation**: None (constructor).

**Internal Logic/Purpose**:
- Loads an image from the specified file using `Image::load(filename)`.
- If the image fails to load, it prints an error message and creates a 1x1 magenta texture as a fallback.
- If the image loads successfully, it extracts the width, height, and channels, and then generates and binds an OpenGL texture.
- It sets up texture parameters such as wrapping and filtering modes.

### Method: ~OpenGLTexture()

**Full Signature**:
```cpp
OpenGLTexture::~OpenGLTexture()
```

**Return Type Explanation**: None (destructor).

**Internal Logic/Purpose**:
- Deletes the OpenGL texture to free up resources using `glDeleteTextures`.

### Method: void use(int unit)

**Full Signature**:
```cpp
void OpenGLTexture::use(int unit)
```

**Return Type Explanation**: None.

**Internal Logic/Purpose**:
- Binds the managed OpenGL texture to a specified texture unit for rendering.