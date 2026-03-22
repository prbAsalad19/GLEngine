#pragma once
#include "core/math/algebricOp.h"

class Transform
{
public:
    Vector3    position;
    Quaternion rotation;
    Vector3    scale;
    Vector3 forward;

    Transform();
    static Transform getIdentityTransform();

    mat4 getMatrix() const;
    void setQuaternion(float x, float y, float z, float w);
    void setEuler(const EulerAngles& e);
	void setPosition(float x, float y, float z) { position = { x, y, z }; }
    void rotate(Vector3 axis, float angle);
	void translate(Vector3 translation);
	void lookAt(Vector3 target);
    Vector3 getForward();
    Vector3 getRight();
    Vector3 getUp();
	EulerAngles getEuler() const;
	void translateLocal(Vector3 translation);
    void reset();
    bool lerp(Vector3 start, Vector3 target, float t);
    bool slerp(Quaternion startQuaternion, Quaternion target, float t);
    bool lerpSmooth(Vector3 start, Vector3 target, float t);
    bool slerpSmooth(Quaternion startQuaternion, Quaternion target, float t);
};