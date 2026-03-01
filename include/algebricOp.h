#pragma once
#include <cmath>
#include <cstring>

#define PI 3.14159265358979323846f

// ─────────────────────────────────────────────
//  Vector3
// ─────────────────────────────────────────────

class Vector3
{
public:
    float entries[3];

    static float   dot(Vector3 u, Vector3 v);
    static Vector3 normalize(Vector3 v);
    static Vector3 cross(Vector3 u, Vector3 v);
    void           normalize();
};

// ─────────────────────────────────────────────
//  mat4  (column-major, compatible with OpenGL)
// ─────────────────────────────────────────────

struct mat4
{
    float entries[16];

    mat4 operator*(const mat4& other) const;

    static mat4 create_matrix_transform(Vector3 translation);
    static mat4 create_scale(Vector3 scale);
    static mat4 create_x_rotation(float angleDeg);
    static mat4 create_y_rotation(float angleDeg);
    static mat4 create_z_rotation(float angleDeg);
    static mat4 create_model_transform(Vector3 pos, float angleDeg);
    static mat4 create_look_at(Vector3 from, Vector3 to);
    static mat4 create_prospective_projection(float fovy, float aspect, float nearPlane, float farPlane);
};

// ─────────────────────────────────────────────
//  Vertex  (position + normal + uv)
//  Kept here because both the mesh and the math
//  layer need to agree on the layout.
// ─────────────────────────────────────────────

struct Vertex
{
    float position[3];
    float normal[3];
    float uv[2];

    void zero() { memset(this, 0, sizeof(Vertex)); }

    bool operator==(const Vertex& o) const { return memcmp(this, &o, sizeof(Vertex)) == 0; }
    bool operator< (const Vertex& o) const { return memcmp(this, &o, sizeof(Vertex)) <  0; }
};

// ─────────────────────────────────────────────
//  Rotation helpers
// ─────────────────────────────────────────────

// Euler angles in degrees: pitch = X, yaw = Y, roll = Z
// Application order: roll → yaw → pitch  (R = Rz * Ry * Rx)
class EulerAngles
{
public:
    float pitch;
    float yaw;
    float roll;

    mat4 toMat4() const;
};

// Unit quaternion (x, y, z, w)
class Quaternion
{
public:
    float x, y, z, w;

    void normalize();
    mat4 toMat4() const;

    static Quaternion identity();
    static Quaternion multiply(const Quaternion& a, const Quaternion& b);
    static Quaternion fromAxisAngle(Vector3 axis, float angleDeg);
    static Quaternion fromEuler(const EulerAngles& e);
};

// ─────────────────────────────────────────────
//  Transform  (T * R * S)
// ─────────────────────────────────────────────

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
