## algebricOp.h

### Brief Overview

This header file contains definitions for basic vector and matrix operations commonly used in graphics programming. The provided classes include `Vector3`, `Vector2`, `mat4`, `Vertex`, `EulerAngles`, and `Quaternion`. These tools facilitate mathematical computations necessary for 3D transformations, projections, and other geometric calculations.

### Table of Classes/Structs/Enums

1. **Vector3**
2. **Vector2**
3. **mat4**
4. **Vertex**
5. **EulerAngles**
6. **Quaternion**

### Detailed Methods Section

#### Vector3 Class

##### `Vector3 operator+(const Vector3& other) const`
- **Return Type:** `Vector3`
- **Purpose:** Adds two vectors element-wise.

##### `Vector3 operator-(const Vector3& other) const`
- **Return Type:** `Vector3`
- **Purpose:** Subtracts one vector from another element-wise.

##### `Vector3 operator*(float scalar) const`
- **Return Type:** `Vector3`
- **Purpose:** Multiplies a vector by a scalar value.

##### `static float dot(Vector3 u, Vector3 v)`
- **Return Type:** `float`
- **Purpose:** Calculates the dot product of two vectors.

##### `static Vector3 normalize(Vector3 v)`
- **Return Type:** `Vector3`
- **Purpose:** Normalizes (converts to unit length) a vector.

##### `static Vector3 cross(Vector3 u, Vector3 v)`
- **Return Type:** `Vector3`
- **Purpose:** Computes the cross product of two vectors.

##### `static Vector3 midpoint(Vector3 u, Vector3 v)`
- **Return Type:** `Vector3`
- **Purpose:** Calculates the midpoint between two points represented as vectors.

##### `static Vector3 min(Vector3 u, Vector3 v)`
- **Return Type:** `Vector3`
- **Purpose:** Returns a vector with each component being the minimum of the corresponding components of the input vectors.

##### `static Vector3 max(Vector3 u, Vector3 v)`
- **Return Type:** `Vector3`
- **Purpose:** Returns a vector with each component being the maximum of the corresponding components of the input vectors.

##### `void normalize()`
- **Return Type:** None
- **Purpose:** Normalizes the vector in-place (modifies the original vector).

#### Vector2 Class

##### `Vector2 operator+(const Vector2& other) const`
- **Return Type:** `Vector2`
- **Purpose:** Adds two vectors element-wise.

##### `Vector2 operator-(const Vector2& other) const`
- **Return Type:** `Vector2`
- **Purpose:** Subtracts one vector from another element-wise.

##### `Vector2 operator*(float scalar) const`
- **Return Type:** `Vector2`
- **Purpose:** Multiplies a vector by a scalar value.

##### `static float dot(Vector2 u, Vector2 v)`
- **Return Type:** `float`
- **Purpose:** Calculates the dot product of two vectors.

##### `static Vector2 normalize(Vector2 v)`
- **Return Type:** `Vector2`
- **Purpose:** Normalizes (converts to unit length) a vector.

##### `void normalize()`
- **Return Type:** None
- **Purpose:** Normalizes the vector in-place (modifies the original vector).

#### mat4 Class

##### `mat4 operator*(const mat4& other) const`
- **Return Type:** `mat4`
- **Purpose:** Multiplies two matrices.

##### `static mat4 create_matrix_transform(Vector3 translation)`
- **Return Type:** `mat4`
- **Purpose:** Creates a translation matrix.

##### `static mat4 create_scale(Vector3 scale)`
- **Return Type:** `mat4`
- **Purpose:** Creates a scaling matrix.

##### `static mat4 create_x_rotation(float angleDeg)`
- **Return Type:** `mat4`
- **Purpose:** Creates an X-axis rotation matrix.

##### `static mat4 create_y_rotation(float angleDeg)`
- **Return Type:** `mat4`
- **Purpose:** Creates a Y-axis rotation matrix.

##### `static mat4 create_z_rotation(float angleDeg)`
- **Return Type:** `mat4`
- **Purpose:** Creates a Z-axis rotation matrix.

##### `static mat4 create_model_transform(Vector3 pos, float angleDeg)`
- **Return Type:** `mat4`
- **Purpose:** Combines translation and rotation to form a model transformation matrix.

##### `static mat4 create_look_at(Vector3 from, Vector3 to)`
- **Return Type:** `mat4`
- **Purpose:** Creates a view matrix using the look-at method.

##### `static mat4 create_prospective_projection(float fovy, float aspect, float nearPlane, float farPlane)`
- **Return Type:** `mat4`
- **Purpose:** Creates a perspective projection matrix.

#### Vertex Struct

No detailed methods provided; includes data fields and utility functions like `zero()` and comparison operators for sorting.

#### EulerAngles Class

##### `mat4 toMat4() const`
- **Return Type:** `mat4`
- **Purpose:** Converts Euler angles (pitch, yaw, roll) into a rotation matrix.

#### Quaternion Class

##### `void normalize()`
- **Return Type:** None
- **Purpose:** Normalizes the quaternion in-place (modifies the original quaternion).

##### `mat4 toMat4() const`
- **Return Type:** `mat4`
- **Purpose:** Converts a quaternion into a rotation matrix.

##### `static Quaternion identity()`
- **Return Type:** `Quaternion`
- **Purpose:** Returns an identity quaternion.

##### `static Quaternion multiply(const Quaternion& a, const Quaternion& b)`
- **Return Type:** `Quaternion`
- **Purpose:** Multiplies two quaternions.

##### `static Quaternion fromAxisAngle(Vector3 axis, float angleDeg)`
- **Return Type:** `Quaternion`
- **Purpose:** Creates a quaternion from an axis and an angle in degrees.

##### `static Quaternion fromEuler(const EulerAngles& e)`
- **Return Type:** `Quaternion`
- **Purpose:** Creates a quaternion from Euler angles.

##### `EulerAngles getEuler() const`
- **Return Type:** `EulerAngles`
- **Purpose:** Converts a quaternion to Euler angles.