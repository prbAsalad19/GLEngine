### 1. `Camera.h`
---

### 2. Brief Overview

The `Camera` class represents the viewer's point of view in the scene. It is designed to be used by the scene or application layer, and it does not live in the ResourceManager. The camera produces two essential matrices every frame: the `view matrix` which transforms world space into camera space, and the `projection matrix` which transforms camera space into clip space.

### 3. Table of Classes/Structs/Enums

- **Class/Struct:** Camera
- **Summary:** Represents the viewer's point of view in the scene.
- **Fields:**
  - `position`: The current position of the camera in world space.
  - `target`: The target position that the camera is looking at.
  - `fovDegrees`: The field of view of the camera in degrees.
  - `nearPlane`: The near clipping plane distance.
  - `farPlane`: The far clipping plane distance.

### 4. Detailed Methods Section

#### Method: Camera()
- **Full Signature:** `Camera();`
- **Return Type Explanation:** This is a default constructor for creating an instance of the `Camera` class with default values.
- **Internal Logic/Purpose:** Initializes the camera with default values (position at `(0, 0, 0)`, target at `(0, 0, -1)`, field of view of `60` degrees, near plane at `0.1`, and far plane at `100`).

#### Method: Camera(Vector3 position, Vector3 target, float fovDegrees, float nearPlane, float farPlane)
- **Full Signature:** `Camera(Vector3 position, Vector3 target, float fovDegrees, float nearPlane, float farPlane);`
- **Return Type Explanation:** This is a parameterized constructor for creating an instance of the `Camera` class with specified values.
- **Internal Logic/Purpose:** Initializes the camera with the provided parameters. The camera's initial position and target are set based on the provided vectors and the field of view, near plane, and far plane are set accordingly.

#### Method: getViewMatrix() const
- **Full Signature:** `mat4 getViewMatrix() const;`
- **Return Type Explanation:** This method returns a 4x4 matrix that represents the transformation from world space to camera space.
- **Internal Logic/Purpose:** Calculates and returns the view matrix using the camera's position and target.

#### Method: getProjectionMatrix(float aspect) const
- **Full Signature:** `mat4 getProjectionMatrix(float aspect) const;`
- **Return Type Explanation:** This method returns a 4x4 projection matrix that transforms points from camera space to clip space.
- **Internal Logic/Purpose:** Calculates and returns the projection matrix using the field of view, near plane, far plane, and the provided aspect ratio. The aspect ratio is passed as a parameter since it can change (e.g., due to window resizing), and the camera should not depend on it internally.

---

This documentation provides a professional, Unity-like style reference for the `Camera` class, detailing its purpose, fields, and methods with explanations of their functionality and internal logic.