# Camera.cpp

## Brief Overview

The `Camera` class is a fundamental component in the rendering system, responsible for defining how the scene is viewed. It manages camera properties such as position, target, field of view (FOV), and clipping planes.

## Table of Classes/Structs/Enums

- **Camera**: Main class representing the camera.
  - Fields:
    - `position`: The current position of the camera in world space.
    - `target`: The point in space that the camera is looking at.
    - `fovDegrees`: Field of view of the camera in degrees.
    - `nearPlane` and `farPlane`: Clipping planes for depth-based culling.

## Detailed Methods Section

### 1. Camera::Camera()

**Full Signature:**
```cpp
Camera::Camera()
    : position({ 0.0f, -5.0f, 0.0f })
    , target({ 0.0f,  0.0f, 0.0f })
    , fovDegrees(45.0f)
    , nearPlane(0.1f)
    , farPlane(100.0f)
{
}
```

**Return Type Explanation:**
- **No return value**: This is a constructor; it initializes the camera with default values.

**Internal Logic/Purpose:**
- Initializes a new instance of the `Camera` class with default values:
  - Position set to `{0.0f, -5.0f, 0.0f}` (looking downwards from above).
  - Target set to `{0.0f, 0.0f, 0.0f}` (looking at the origin).
  - Field of view set to `45.0f` degrees.
  - Near and far clipping planes set to `0.1f` and `100.0f`, respectively.

### 2. Camera::Camera(Vector3 position, Vector3 target, float fovDegrees, float nearPlane, float farPlane)

**Full Signature:**
```cpp
Camera::Camera(Vector3 position, Vector3 target, float fovDegrees, float nearPlane, float farPlane)
    : position(position)
    , target(target)
    , fovDegrees(fovDegrees)
    , nearPlane(nearPlane)
    , farPlane(farPlane)
{
}
```

**Return Type Explanation:**
- **No return value**: This is a constructor; it initializes the camera with custom values.

**Internal Logic/Purpose:**
- Initializes a new instance of the `Camera` class with the provided parameters:
  - `position`: The initial position of the camera.
  - `target`: The point in space that the camera will look at.
  - `fovDegrees`: The field of view for the camera.
  - `nearPlane` and `farPlane`: Clipping planes for depth-based culling.

### 3. mat4 Camera::getViewMatrix() const

**Full Signature:**
```cpp
mat4 Camera::getViewMatrix() const
{
    return mat4::create_look_at(position, target);
}
```

**Return Type Explanation:**
- **`mat4`**: A 4x4 transformation matrix representing the view from the camera's position to its target.

**Internal Logic/Purpose:**
- Calculates and returns a right-handed view matrix using the `position` and `target`.
- The `create_look_at` function constructs this matrix by transforming the coordinate system so that the camera is at the origin, looking along the negative Z-axis towards the target.

### 4. mat4 Camera::getProjectionMatrix(float aspect) const

**Full Signature:**
```cpp
mat4 Camera::getProjectionMatrix(float aspect) const
{
    return mat4::create_prospective_projection(fovDegrees, aspect, nearPlane, farPlane);
}
```

**Return Type Explanation:**
- **`mat4`**: A 4x4 transformation matrix representing the perspective projection of the camera.

**Internal Logic/Purpose:**
- Calculates and returns a perspective projection matrix using the `fovDegrees`, `aspect`, `nearPlane`, and `farPlane`.
- The `create_prospective_projection` function constructs this matrix based on the camera's field of view, aspect ratio, near clipping plane, and far clipping plane, ensuring that only objects within the specified depth range are rendered.