# OpenGLShaderProgram.cpp

## Brief Overview

This file contains the implementation of a class for managing OpenGL shader programs. The `OpenGLShaderProgram` class encapsulates the functionality to load, compile, link, and use vertex and fragment shaders, as well as set uniforms within those shaders.

## Table of Classes/Structs/Enums
- **OpenGLShaderProgram**: Class responsible for managing and using OpenGL shader programs.
  - **Public Methods**:
    - `OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath)`: Constructor to create a new shader program from vertex and fragment shader paths.
    - `~OpenGLShaderProgram()`: Destructor to clean up the shader program.
    - `void bind() const`: Binds the shader program for use.
    - `void unbind() const`: Unbinds the current shader program.
    - `void setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const`: Sets the binding point for a uniform block.
    - `void setInt(const std::string& name, int value) const`: Sets an integer uniform.
    - `void setFloat(const std::string& name, float value) const`: Sets a floating-point uniform.
    - `void setMat4(const std::string& name, const mat4& matrix) const`: Sets a 4x4 matrix uniform.
    - `void setVec2(const std::string& name, const Vector2& v) const`: Sets a 2D vector uniform.
    - `void setVec3(const std::string& name, const Vector3& v) const`: Sets a 3D vector uniform.
    - `void setUInt(const std::string& name, unsigned int value) const`: Sets an unsigned integer uniform.
    - `GLint getUniformLocation(const std::string& name) const`: Retrieves the location of a uniform variable.
  - **Private Methods**:
    - `GLuint compileModule(const std::string& filepath, GLenum type)`: Compiles a shader module from a file.

## Detailed Methods Section

### Method: OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath)

- **Full Signature**: 
  ```cpp
  OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath)
  ```
  
- **Return Type Explanation**: None (Constructor)
  
- **Internal Logic/Purpose**:
  - The constructor initializes a new `OpenGLShaderProgram` object.
  - It attempts to compile the vertex and fragment shaders from the given file paths using `compileModule`.
  - If either shader compilation fails, it cleans up any created shaders and logs an error message.
  - If both shaders compile successfully, it creates a program ID, attaches the shaders to it, links the program, and handles linking errors by logging them.
  - It deletes temporary shader objects after successful linkage or if they fail to link.

### Method: ~OpenGLShaderProgram()

- **Full Signature**: 
  ```cpp
  ~OpenGLShaderProgram()
  ```
  
- **Return Type Explanation**: None (Destructor)
  
- **Internal Logic/Purpose**:
  - The destructor cleans up the OpenGL shader program.
  - It checks if a program ID exists and deletes it using `glDeleteProgram`.

### Method: void bind() const

- **Full Signature**: 
  ```cpp
  void bind() const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method binds the shader program for use by calling `glUseProgram(m_programID)`.

### Method: void unbind() const

- **Full Signature**: 
  ```cpp
  void unbind() const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method unbinds the current shader program by calling `glUseProgram(0)`.

### Method: void setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const

- **Full Signature**: 
  ```cpp
  void setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets the binding point for a uniform block identified by `blockName`.
  - It retrieves the index of the uniform block and then calls `glUniformBlockBinding`.

### Method: void setInt(const std::string& name, int value) const

- **Full Signature**: 
  ```cpp
  void setInt(const std::string& name, int value) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets an integer uniform identified by `name`.
  - It retrieves the location of the uniform using `getUniformLocation` and then calls `glUniform1i`.

### Method: void setFloat(const std::string& name, float value) const

- **Full Signature**: 
  ```cpp
  void setFloat(const std::string& name, float value) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets a floating-point uniform identified by `name`.
  - It retrieves the location of the uniform using `getUniformLocation` and then calls `glUniform1f`.

### Method: void setMat4(const std::string& name, const mat4& matrix) const

- **Full Signature**: 
  ```cpp
  void setMat4(const std::string& name, const mat4& matrix) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets a 4x4 matrix uniform identified by `name`.
  - It retrieves the location of the uniform using `getUniformLocation` and then calls `glUniformMatrix4fv`.

### Method: void setVec2(const std::string& name, const Vector2& v) const

- **Full Signature**: 
  ```cpp
  void setVec2(const std::string& name, const Vector2& v) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets a 2D vector uniform identified by `name`.
  - It retrieves the location of the uniform using `getUniformLocation` and then calls `glUniform2fv`.

### Method: void setVec3(const std::string& name, const Vector3& v) const

- **Full Signature**: 
  ```cpp
  void setVec3(const std::string& name, const Vector3& v) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets a 3D vector uniform identified by `name`.
  - It retrieves the location of the uniform using `getUniformLocation` and then calls `glUniform3fv`.

### Method: void setUInt(const std::string& name, unsigned int value) const

- **Full Signature**: 
  ```cpp
  void setUInt(const std::string& name, unsigned int value) const
  ```
  
- **Return Type Explanation**: None
  
- **Internal Logic/Purpose**:
  - This method sets an unsigned integer uniform identified by `name`.
  - It retrieves the location of the uniform using `getUniformLocation` and then calls `glUniform1ui`.

### Method: GLint getUniformLocation(const std::string& name) const

- **Full Signature**: 
  ```cpp
  GLint getUniformLocation(const std::string& name) const
  ```
  
- **Return Type Explanation**:
  - Returns the location of a uniform variable within the shader program.
  
- **Internal Logic/Purpose**:
  - This method retrieves the location of a uniform identified by `name`.
  - It uses a cache to store previously queried locations for efficiency.
  - If the uniform is not found, it logs a warning message.

### Method: GLuint compileModule(const std::string& filepath, GLenum type)

- **Full Signature**: 
  ```cpp
  GLuint compileModule(const std::string& filepath, GLenum type)
  ```
  
- **Return Type Explanation**:
  - Returns the ID of the compiled shader module.
  - If compilation fails, it returns 0 and logs an error message.
  
- **Internal Logic/Purpose**:
  - This method compiles a shader module from a file specified by `filepath`.
  - It reads the source code from the file, creates a shader object, sets the source code, and compiles it.
  - If compilation fails, it deletes the shader object and logs an error message.

This class provides a robust way to manage and use OpenGL shaders in applications that require rendering with complex shading.