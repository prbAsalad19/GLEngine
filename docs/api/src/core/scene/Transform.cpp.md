## Transform.cpp

### Brief Overview:
This file contains the implementation of the `Transform` class, which is a fundamental component in managing an object's position, rotation, and scale within a scene. The `Transform` class provides methods to get and set these properties, perform transformations such as translation, rotation, and scaling, and utilities like look-at and forward/up/right vectors.

### Table of Classes/Structs/Enums:

1. **Class**: Transform
   - Inherits from: None
   - Description: Manages an object's position, rotation, and scale within the scene.

2. **Structs/Enums**:
   - `Vector3`
   - `Quaternion`
   - `EulerAngles`

### Detailed Methods Section:

#### 1. Constructor
```cpp
Transform::Transform()
{
    position.entries[0] = position.entries[1] = position.entries[2] = 0.0f;
    rotation = Quaternion::identity();
    scale.entries[0] = scale.entries[1] = scale.entries[2] = 1.0f;
}
```
- **Full signature**: `Transform::Transform()`
- **Return type explanation**: None (Constructor)
- **Internal logic/purpose**: Initializes the transform to default values: position at origin, identity rotation, and unit scale.

#### 2. Static Method - Get Identity Transform
```cpp
Transform Transform::getIdentityTransform()
{
    return Transform();
}
```
- **Full signature**: `static Transform Transform::getIdentityTransform()`
- **Return type explanation**: Returns a new `Transform` object with identity values.
- **Internal logic/purpose**: Provides a way to create a transform that represents no transformation.

#### 3. Method - Get Matrix
```cpp
mat4 Transform::getMatrix() const
{
    return mat4::create_matrix_transform(position) * rotation.toMat4() * mat4::create_scale(scale);
}
```
- **Full signature**: `mat4 Transform::getMatrix() const`
- **Return type explanation**: Returns a 4x4 transformation matrix.
- **Internal logic/purpose**: Constructs a transformation matrix using the current position, rotation, and scale.

#### 4. Method - Set Quaternion
```cpp
void Transform::setQuaternion(float x, float y, float z, float w)
{
    rotation = { x, y, z, w };
}
```
- **Full signature**: `void Transform::setQuaternion(float x, float y, float z, float w)`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Sets the rotation of the transform using a quaternion.

#### 5. Method - Set Euler Angles
```cpp
void Transform::setEuler(const EulerAngles& e)
{
    rotation = Quaternion::fromEuler(e);
}
```
- **Full signature**: `void Transform::setEuler(const EulerAngles& e)`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Sets the rotation of the transform using Euler angles.

#### 6. Method - Rotate
```cpp
void Transform::rotate(Vector3 axis, float angle)
{
    rotation = Quaternion::multiply(rotation, Quaternion::fromAxisAngle(axis, angle));
}
```
- **Full signature**: `void Transform::rotate(Vector3 axis, float angle)`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Applies a rotation around a given axis by the specified angle.

#### 7. Method - Translate
```cpp
void Transform::translate(Vector3 translation)
{
    position = position + translation;
}
```
- **Full signature**: `void Transform::translate(Vector3 translation)`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Translates the transform by a given vector.

#### 8. Method - Look At
```cpp
void Transform::lookAt(Vector3 target)
{
    rotation = Quaternion::fromEuler({ 0.0f, atan2f(target.entries[0] - position.entries[0], target.entries[2] - position.entries[2]) * 180.0f / 3.14159265f, 0.0f });
}
```
- **Full signature**: `void Transform::lookAt(Vector3 target)`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Rotates the transform so that its forward direction points towards a given target.

#### 9. Method - Get Forward Vector
```cpp
Vector3 Transform::getForward()
{
    // Calculation using quaternion to determine forward direction.
}
```
- **Full signature**: `Vector3 Transform::getForward() const`
- **Return type explanation**: Returns the forward direction vector of the transform.
- **Internal logic/purpose**: Calculates and returns the forward direction based on the current rotation.

#### 10. Method - Get Right Vector
```cpp
Vector3 Transform::getRight()
{
    // Calculation using quaternion to determine right direction.
}
```
- **Full signature**: `Vector3 Transform::getRight() const`
- **Return type explanation**: Returns the right direction vector of the transform.
- **Internal logic/purpose**: Calculates and returns the right direction based on the current rotation.

#### 11. Method - Get Up Vector
```cpp
Vector3 Transform::getUp()
{
    // Calculation using quaternion to determine up direction.
}
```
- **Full signature**: `Vector3 Transform::getUp() const`
- **Return type explanation**: Returns the up direction vector of the transform.
- **Internal logic/purpose**: Calculates and returns the up direction based on the current rotation.

#### 12. Method - Translate Local
```cpp
void Transform::translateLocal(Vector3 translation)
{
    position = position + getRight() * translation.entries[0] + getUp() * translation.entries[1] + getForward() * translation.entries[2];
}
```
- **Full signature**: `void Transform::translateLocal(Vector3 translation)`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Translates the transform relative to its local space.

#### 13. Method - Get Euler Angles
```cpp
EulerAngles Transform::getEuler() const
{
    EulerAngles e;
    e = rotation.getEuler();
	return e;
}
```
- **Full signature**: `EulerAngles Transform::getEuler() const`
- **Return type explanation**: Returns the current orientation of the transform as Euler angles.
- **Internal logic/purpose**: Converts the quaternion to Euler angles.

#### 14. Method - Reset
```cpp
void Transform::reset()
{
    position.entries[0] = position.entries[1] = position.entries[2] = 0.0f;
    rotation = Quaternion::identity();
    scale.entries[0] = scale.entries[1] = scale.entries[2] = 1.0f;
}
```
- **Full signature**: `void Transform::reset()`
- **Return type explanation**: None (Void function)
- **Internal logic/purpose**: Resets the transform to default values.

#### 15. Method - Linear Interpolation
```cpp
bool Transform::lerp(Vector3 start, Vector3 target, float t)
{
    if (t >= 1.0f) { position = target; return true; }
    position = start * (1.0f - t) + target * t;
    return false;
}
```
- **Full signature**: `bool Transform::lerp(Vector3 start, Vector3 target, float t)`
- **Return type explanation**: Returns `true` if interpolation is complete.
- **Internal logic/purpose**: Interpolates the position from start to target using linear interpolation.

#### 16. Method - Spherical Linear Interpolation
```cpp
bool Transform::slerp(Quaternion startQuaternion, Quaternion target, float t)
{
    // Calculation using quaternion slerp for smooth rotation.
}
```
- **Full signature**: `bool Transform::slerp(Quaternion startQuaternion, Quaternion target, float t)`
- **Return type explanation**: Returns `true` if interpolation is complete.
- **Internal logic/purpose**: Interpolates the rotation from start to target using spherical linear interpolation.

#### 17. Method - Smooth Linear Interpolation
```cpp
bool Transform::lerpSmooth(Vector3 start, Vector3 target, float t)
{
    if (t >= 1.0f) { position = target; return true; }
    float st = t * t * (3.0f - 2.0f * t);
    position = start * (1.0f - st) + target * st;
    return false;
}
```
- **Full signature**: `bool Transform::lerpSmooth(Vector3 start, Vector3 target, float t)`
- **Return type explanation**: Returns `true` if interpolation is complete.
- **Internal logic/purpose**: Interpolates the position smoothly from start to target using a smooth function.

#### 18. Method - Smooth Spherical Linear Interpolation
```cpp
bool Transform::slerpSmooth(Quaternion startQuaternion, Quaternion target, float t)
{
    if (t >= 1.0f) { rotation = target; return true; }
    float st = t * t * (3.0f - 2.0f * t);
    return slerp(startQuaternion, target, st);
}
```
- **Full signature**: `bool Transform::slerpSmooth(Quaternion startQuaternion, Quaternion target, float t)`
- **Return type explanation**: Returns `true` if interpolation is complete.
- **Internal logic/purpose**: Interpolates the rotation smoothly from start to target using a smooth function.

This implementation provides a comprehensive set of methods for managing and manipulating transforms in a scene, ensuring flexibility and ease of use.