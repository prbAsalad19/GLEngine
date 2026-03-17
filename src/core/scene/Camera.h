#pragma once
#include "core/math/algebricOp.h"

class Camera
{
public:

	Camera() : pitch(0.0f), yaw(0.0f), fov(45.0f), nearPlane(0.1f), farPlane(100.0f) {}

	mat4 getViewMatrix() const;
	mat4 getProjectionMatrix(float aspectRatio) const;
	Vector3 getForward() const;
	void setTarget(const Vector3& target);
	void setFov(float fovDeg) { fov = fovDeg; }

	Vector3 position;
	float pitch, yaw;
	float fov, nearPlane, farPlane;
};
