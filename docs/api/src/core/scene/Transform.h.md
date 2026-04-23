# Transform.h

## Brief Overview

This header file defines the `Transform` class, which is a fundamental component in 3D graphics and game development. It represents the position, rotation, and scale of an object within the scene, as well as provides methods to manipulate these properties.

## Table of Classes/Structs/Enums

- **Class**: Transform
    - **Fields**:
        - `position`: A vector representing the 3D coordinates (x, y, z).
        - `rotation`: A quaternion representing the orientation of the object.
        - `scale`: A vector representing the scale factors along the x, y, and z axes.
        - `forward`: A vector indicating the forward direction of the object.
    - **Methods**:
        - Constructors
        - Static Methods
        - Instance Methods

## Detailed Methods Section

### Constructor
- **Signature**: `Transform();`
- **Return Type Explanation**: Initializes a new instance of the `Transform` class with default values (position at origin, identity rotation, and scale as 1).
- **Internal Logic/Purpose**: Sets up the initial state of the transform.

### Static Method
- **Signature**: `static Transform getIdentityTransform();`
- **Return Type Explanation**: Returns a new instance of `Transform` representing the identity transformation (no translation, no rotation, and no scaling).
- **Internal Logic/Purpose**: Provides a convenient way to create a default transform state.

### Instance Methods

#### `getMatrix() const`
- **Signature**: `mat4 getMatrix() const;`
- **Return Type Explanation**: Returns a 4x4 matrix representing the transformation.
- **Internal Logic/Purpose**: Constructs and returns a matrix that combines translation, rotation, and scaling.

#### `setQuaternion(float x, float y, float z, float w)`
- **Signature**: `void setQuaternion(float x, float y, float z, float w);`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Sets the object's orientation using a quaternion defined by its components.

#### `setEuler(const EulerAngles& e)`
- **Signature**: `void setEuler(const EulerAngles& e);`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Sets the object's orientation using Euler angles, which represent rotations around three axes.

#### `setPosition(float x, float y, float z)`
- **Signature**: `void setPosition(float x, float y, float z) { position = { x, y, z }; }`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Sets the object's position using 3D coordinates.

#### `rotate(Vector3 axis, float angle)`
- **Signature**: `void rotate(Vector3 axis, float angle);`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Rotates the object by a specified angle around an arbitrary axis.

#### `translate(Vector3 translation)`
- **Signature**: `void translate(Vector3 translation);`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Translates the object by a specified vector.

#### `lookAt(Vector3 target)`
- **Signature**: `void lookAt(Vector3 target);`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Rotates the object so that its forward direction points towards a given target point.

#### `getForward()`
- **Signature**: `Vector3 getForward();`
- **Return Type Explanation**: A vector representing the forward direction of the object.
- **Internal Logic/Purpose**: Returns the forward direction based on the current rotation.

#### `getRight()`
- **Signature**: `Vector3 getRight();`
- **Return Type Explanation**: A vector representing the right direction of the object.
- **Internal Logic/Purpose**: Returns the right direction based on the current rotation.

#### `getUp()`
- **Signature**: `Vector3 getUp();`
- **Return Type Explanation**: A vector representing the up direction of the object.
- **Internal Logic/Purpose**: Returns the up direction based on the current rotation.

#### `getEuler() const`
- **Signature**: `EulerAngles getEuler() const;`
- **Return Type Explanation**: Euler angles representing the object's orientation.
- **Internal Logic/Purpose**: Converts the quaternion representation of rotation into Euler angles.

#### `translateLocal(Vector3 translation)`
- **Signature**: `void translateLocal(Vector3 translation);`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Translates the object relative to its current orientation.

#### `reset()`
- **Signature**: `void reset();`
- **Return Type Explanation**: None (void).
- **Internal Logic/Purpose**: Resets the transform to its default state.

#### `lerp(Vector3 start, Vector3 target, float t)`
- **Signature**: `bool lerp(Vector3 start, Vector3 target, float t);`
- **Return Type Explanation**: A boolean indicating whether the interpolation was successful.
- **Internal Logic/Purpose**: Linearly interpolates between a starting and a target position.

#### `slerp(Quaternion startQuaternion, Quaternion targetQuaternion, float t)`
- **Signature**: `bool slerp(Quaternion startQuaternion, Quaternion targetQuaternion, float t);`
- **Return Type Explanation**: A boolean indicating whether the interpolation was successful.
- **Internal Logic/Purpose**: Spherical linearly interpolates between two quaternions.

#### `lerpSmooth(Vector3 start, Vector3 target, float t)`
- **Signature**: `bool lerpSmooth(Vector3 start, Vector3 target, float t);`
- **Return Type Explanation**: A boolean indicating whether the interpolation was successful.
- **Internal Logic/Purpose**: Smoothly interpolates between a starting and a target position.

#### `slerpSmooth(Quaternion startQuaternion, Quaternion targetQuaternion, float t)`
- **Signature**: `bool slerpSmooth(Quaternion startQuaternion, Quaternion targetQuaternion, float t);`
- **Return Type Explanation**: A boolean indicating whether the interpolation was successful.
- **Internal Logic/Purpose**: Smoothly interpolates between two quaternions using spherical linear interpolation.

This class is designed to be versatile and powerful for managing the transformations of objects in a 3D environment. It provides both basic and advanced methods for setting and manipulating position, rotation, and scale, making it essential for the development of interactive and dynamic applications.