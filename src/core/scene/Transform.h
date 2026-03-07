#pragma once
#include "core/math/algebricOp.h"

class Transform
{
public:
    Vector3    position;
    Quaternion rotation;
    Vector3    scale;

    Transform();
    static Transform getIdentityTransform();

    mat4 getMatrix() const;
    void setQuaternion(float x, float y, float z, float w);
    void setEuler(const EulerAngles& e);
};