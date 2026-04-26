## OpenGLTexture.h

### Brief Overview

The `OpenGLTexture` class is designed to handle the creation and management of textures in an OpenGL context. It facilitates loading textures from files, binding them for rendering purposes, and ensuring proper resource cleanup.

### Table of Classes/Structs/Enums

- **Class**: `OpenGLTexture`
  - Inherits from: None
  - Description: Manages OpenGL texture resources.

### Detailed Methods Section

#### Constructor: `OpenGLTexture(const char* filename)`

- **Full Signature**:
  ```cpp
  OpenGLTexture(const char* filename);
  ```
  
- **Return Type Explanation**: 
  - This method initializes a new instance of the `OpenGLTexture` class. It takes one parameter, `filename`, which is a C-style string representing the path to the image file from which the texture will be loaded.

- **Internal Logic/Purpose**:
  - The constructor loads the specified image file using `ImageData::LoadImageFromFile`.
  - It then generates an OpenGL texture ID and binds it.
  - The texture data is uploaded to the GPU for rendering.

#### Destructor: `~OpenGLTexture()`

- **Full Signature**:
  ```cpp
  ~OpenGLTexture();
  ```
  
- **Return Type Explanation**: 
  - This method destroys the current instance of the `OpenGLTexture` class. It does not return any value.

- **Internal Logic/Purpose**:
  - The destructor ensures that the OpenGL texture is properly deleted by calling `glDeleteTextures`.
  - This helps in freeing up GPU memory and preventing resource leaks.

#### Method: `use(int unit)`

- **Full Signature**:
  ```cpp
  void use(int unit);
  ```
  
- **Return Type Explanation**: 
  - This method binds the texture to a specific texture unit for rendering purposes. It takes one parameter, `unit`, which specifies the index of the texture unit.

- **Internal Logic/Purpose**:
  - The `use` method sets the active texture unit using `glActiveTexture`.
  - It then binds the texture to this unit with `glBindTexture`.
  - This is crucial for rendering operations where multiple textures are used simultaneously.