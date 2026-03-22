#include "Transform.h"

Transform::Transform()
{
    position.entries[0] = position.entries[1] = position.entries[2] = 0.0f;
    rotation = Quaternion::identity();
    scale.entries[0] = scale.entries[1] = scale.entries[2] = 1.0f;
}

Transform Transform::getIdentityTransform()
{
    return Transform();
}

mat4 Transform::getMatrix() const
{
    return mat4::create_matrix_transform(position) * rotation.toMat4() * mat4::create_scale(scale);
}

void Transform::setQuaternion(float x, float y, float z, float w)
{
    rotation = { x, y, z, w };
}

void Transform::setEuler(const EulerAngles& e)
{
    rotation = Quaternion::fromEuler(e);
}

void Transform::rotate(Vector3 axis, float angle)
{
	rotation = Quaternion::multiply(rotation, Quaternion::fromAxisAngle(axis, angle));
}

void Transform::translate(Vector3 translation)
{
    position = position + translation;
}

void Transform::lookAt(Vector3 target)
{
    rotation = Quaternion::fromEuler({ 0.0f, atan2f(target.entries[0] - position.entries[0], target.entries[2] - position.entries[2]) * 180.0f / 3.14159265f, 0.0f });
}

Vector3 Transform::getForward()
{
	forward.entries[0] = 2.0f * (rotation.x * rotation.z + rotation.w * rotation.y);
	forward.entries[1] = 2.0f * (rotation.y * rotation.z - rotation.w * rotation.x);
	forward.entries[2] = 1.0f - 2.0f * (rotation.x * rotation.x + rotation.y * rotation.y);

    return forward;
}

Vector3 Transform::getRight()
{
    Vector3 right;
    right.entries[0] = 1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z);
    right.entries[1] = 2.0f * (rotation.x * rotation.y + rotation.w * rotation.z);
    right.entries[2] = 2.0f * (rotation.x * rotation.z - rotation.w * rotation.y);
    return right;
}

Vector3 Transform::getUp()
{
    Vector3 up;
    up.entries[0] = 2.0f * (rotation.x * rotation.y - rotation.w * rotation.z);
    up.entries[1] = 1.0f - 2.0f * (rotation.x * rotation.x + rotation.z * rotation.z);
    up.entries[2] = 2.0f * (rotation.y * rotation.z + rotation.w * rotation.x);
    return up;
}

void Transform::translateLocal(Vector3 translation)
{
    position = position + getRight() * translation.entries[0] + getUp() * translation.entries[1] + getForward() * translation.entries[2];
}

EulerAngles Transform::getEuler() const
{
    EulerAngles e;
    e = rotation.getEuler();
	return e;
}

void Transform::reset()
{
	position.entries[0] = position.entries[1] = position.entries[2] = 0.0f;
	rotation = Quaternion::identity();
	scale.entries[0] = scale.entries[1] = scale.entries[2] = 1.0f;
}

bool Transform::lerp(Vector3 start, Vector3 target, float t)
{
    if (t >= 1.0f) { position = target; return true; }
    position = start * (1.0f - t) + target * t;
    return false;
}

bool Transform::slerp(Quaternion startQuaternion, Quaternion target, float t)
{
    if (t >= 1.0f) { rotation = target; return true; }

    float dot = startQuaternion.x * target.x + startQuaternion.y * target.y
        + startQuaternion.z * target.z + startQuaternion.w * target.w;

    if (dot < 0.0f)
    {
        target = { -target.x, -target.y, -target.z, -target.w };
        dot = -dot;
    }

    if (dot > 0.9995f)
    {
        rotation = {
            startQuaternion.x + t * (target.x - startQuaternion.x),
            startQuaternion.y + t * (target.y - startQuaternion.y),
            startQuaternion.z + t * (target.z - startQuaternion.z),
            startQuaternion.w + t * (target.w - startQuaternion.w)
        };
        rotation.normalize();
        return false;
    }

    float theta = acosf(dot);
    float sinTheta = sinf(theta);
    float wa = sinf((1.0f - t) * theta) / sinTheta;
    float wb = sinf(t * theta) / sinTheta;

    rotation = {
        wa * startQuaternion.x + wb * target.x,
        wa * startQuaternion.y + wb * target.y,
        wa * startQuaternion.z + wb * target.z,
        wa * startQuaternion.w + wb * target.w
    };
    return false;
}

bool Transform::lerpSmooth(Vector3 start, Vector3 target, float t)
{
    if (t >= 1.0f) { position = target; return true; }
    float st = t * t * (3.0f - 2.0f * t);
    position = start * (1.0f - st) + target * st;
    return false;
}

bool Transform::slerpSmooth(Quaternion startQuaternion, Quaternion target, float t)
{
    if (t >= 1.0f) { rotation = target; return true; }
    float st = t * t * (3.0f - 2.0f * t);
    return slerp(startQuaternion, target, st);
}