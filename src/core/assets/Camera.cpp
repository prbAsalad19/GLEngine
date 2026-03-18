#include "Camera.h"

Camera::Camera()
    : position({ 0.0f, -5.0f, 0.0f })
    , target({ 0.0f,  0.0f, 0.0f })
    , fovDegrees(45.0f)
    , nearPlane(0.1f)
    , farPlane(100.0f)
{
}

Camera::Camera(Vector3 position, Vector3 target, float fovDegrees, float nearPlane, float farPlane)
    : position(position)
    , target(target)
    , fovDegrees(fovDegrees)
    , nearPlane(nearPlane)
    , farPlane(farPlane)
{
}

mat4 Camera::getViewMatrix() const
{
    // Delegates directly to the existing math helper.
    // create_look_at builds a right-handed view matrix from position and target.
    return mat4::create_look_at(position, target);
}

mat4 Camera::getProjectionMatrix(float aspect) const
{
    return mat4::create_prospective_projection(fovDegrees, aspect, nearPlane, farPlane);
}
