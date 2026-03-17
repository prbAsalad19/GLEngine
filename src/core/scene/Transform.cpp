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
