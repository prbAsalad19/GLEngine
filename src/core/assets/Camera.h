#pragma once
#include "core/math/algebricOp.h"

// Camera represents the viewer's point of view in the scene.
// It is NOT a resource — it does not live in the ResourceManager.
// It is owned and updated by the scene or application layer.
//
// The camera produces two matrices every frame:
//   - view matrix:       transforms world space → camera space
//   - projection matrix: transforms camera space → clip space
//
// aspect is passed to getProjectionMatrix() rather than stored here,
// because the window can be resized and the camera should not depend on it.

class Camera
{
public:
    Vector3 position;
    Vector3 target;

    float fovDegrees;
    float nearPlane;
    float farPlane;

    float rollDegrees;
    
    Camera();
    Camera(Vector3 position, Vector3 target, float fovDegrees, float nearPlane, float farPlane);

    mat4 getViewMatrix()                            const;
    mat4 getInverseViewMatrix()                     const;
    mat4 getProjectionMatrix(float aspect)          const;
    mat4 getInverseProjectionMatrix(float aspect)   const;

};
