#include "Camera.h"

Vector3 Camera::getForward() const
{
	Vector3 forward;
	forward.entries[0] = cosf(yaw) * cosf(pitch);
	forward.entries[1] = cosf(pitch) * sinf(yaw);
	forward.entries[2] = sinf(pitch);
	return forward;
}

mat4 Camera::getViewMatrix() const
{
	Vector3 forward = getForward();
	return mat4::create_look_at(position, position + forward);
}

mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
	return mat4::create_prospective_projection(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::setTarget(const Vector3& target)
{
	Vector3 direction = { target - position};
	direction.normalize();
	pitch = asin(direction.entries[2]);
	yaw = atan2(direction.entries[1], direction.entries[0]);
}