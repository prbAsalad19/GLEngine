# OpenGLShaderProgram.h

## Brief Overview
The `OpenGLShaderProgram` class provides a professional scripting API for managing and utilizing shader programs in an OpenGL application. It encapsulates the creation, binding, and manipulation of shaders, as well as setting uniform variables for rendering.

## Table of Classes/Structs/Enums
- **Class:**
  - `OpenGLShaderProgram`
- **Structs:**
  None.
- **Enums:**
  None.

## Detailed Methods Section

### Constructor
#### Full signature:
```cpp
OpenGLShaderProgram(const std::string& vertPath, const std::string& fragPath);
```

#### Return type explanation:
N/A (Constructor)

#### Internal logic/purpose:
Initializes a new instance of the `OpenGLShaderProgram` class by loading and compiling vertex and fragment shaders from specified file paths.

### Destructor
#### Full signature:
```cpp
~OpenGLShaderProgram();
```

#### Return type explanation:
N/A (Destructor)

#### Internal logic/purpose:
Releases all resources associated with the shader program, including deleting the OpenGL objects and clearing any internal caches.

### Copy Constructor
#### Full signature:
```cpp
OpenGLShaderProgram(const OpenGLShaderProgram&) = delete;
```

#### Return type explanation:
N/A (Deleted)

#### Internal logic/purpose:
Prevents copying of `OpenGLShaderProgram` instances by declaring the copy constructor as deleted. This enforces the use of move semantics or other means of resource management.

### Assignment Operator
#### Full signature:
```cpp
OpenGLShaderProgram& operator=(const OpenGLShaderProgram&) = delete;
```

#### Return type explanation:
N/A (Deleted)

#### Internal logic/purpose:
Prevents assignment of `OpenGLShaderProgram` instances by declaring the copy assignment operator as deleted. This enforces the use of move semantics or other means of resource management.

### bind
#### Full signature:
```cpp
void bind() const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Binds the shader program to the current rendering context, making it active for subsequent rendering operations.

### unbind
#### Full signature:
```cpp
void unbind() const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Unbinds any currently bound shader program, reverting to the default rendering state if no other shader programs are active.

### setUniformBlockBinding
#### Full signature:
```cpp
void setUniformBlockBinding(const std::string& blockName, GLuint bindingPoint) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Binds a uniform buffer object (UBO) to a specific binding point within the shader program. This is useful for managing large blocks of uniform data efficiently.

### setInt
#### Full signature:
```cpp
void setInt(const std::string& name, int value) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Sets an integer uniform variable in the shader program to a specified value. The `name` parameter specifies the uniform's location within the shader.

### setFloat
#### Full signature:
```cpp
void setFloat(const std::string& name, float value) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Sets a floating-point uniform variable in the shader program to a specified value. The `name` parameter specifies the uniform's location within the shader.

### setMat4
#### Full signature:
```cpp
void setMat4(const std::string& name, const mat4& matrix) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Sets a 4x4 matrix uniform variable in the shader program to a specified value. The `name` parameter specifies the uniform's location within the shader.

### setVec2
#### Full signature:
```cpp
void setVec2(const std::string& name, const Vector2& v) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Sets a 2-component vector uniform variable in the shader program to a specified value. The `name` parameter specifies the uniform's location within the shader.

### setVec3
#### Full signature:
```cpp
void setVec3(const std::string& name, const Vector3& v) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Sets a 3-component vector uniform variable in the shader program to a specified value. The `name` parameter specifies the uniform's location within the shader.

### setUInt
#### Full signature:
```cpp
void setUInt(const std::string& name, unsigned int value) const;
```

#### Return type explanation:
`void`

#### Internal logic/purpose:
Sets an unsigned integer uniform variable in the shader program to a specified value. The `name` parameter specifies the uniform's location within the shader.

### getProgramID
#### Full signature:
```cpp
GLuint getProgramID() const;
```

#### Return type explanation:
`GLuint`

#### Internal logic/purpose:
Returns the OpenGL ID of the shader program, allowing external code to interact with it directly if necessary.

### Private Methods

#### getUniformLocation
#### Full signature:
```cpp
GLint getUniformLocation(const std::string& name) const;
```

#### Return type explanation:
`GLint`

#### Internal logic/purpose:
Retrieves the location of a uniform variable within the shader program. Caches the result to avoid querying OpenGL repeatedly for the same uniform.

#### compileModule
#### Full signature:
```cpp
static GLuint compileModule(const std::string& filepath, GLenum type);
```

#### Return type explanation:
`GLuint`

#### Internal logic/purpose:
Compiles a shader module (either vertex or fragment) from a specified file path. Handles compilation errors and logs them appropriately.