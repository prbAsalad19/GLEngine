#pragma once // Evita la "ridefinizione tipo struct"
#include <cmath>
#include <cstring>

#define PI 3.14159265358979323846f

// 1. Sposta Vector3 qui sopra, altrimenti mat4 non sa cos'è!
class Vector3
{
public:
    float entries[3];
    static float dot(Vector3 u, Vector3 v);
    static Vector3 normalize(Vector3 v);
    void normalize();
    static Vector3 cross(Vector3 u, Vector3 v);
};

// 2. Ora mat4 può usare Vector3
struct mat4
{
    float entries[16];
    static mat4 create_matrix_transform(Vector3 translation);
    static mat4 create_z_rotation(float angle);
    static mat4 create_x_rotation(float angle);
    static mat4 create_y_rotation(float angle);
    static mat4 create_scale(Vector3 scale);
    static mat4 create_model_transform(Vector3 pos, float angle);
    static mat4 create_look_at(Vector3 from, Vector3 to);
    static mat4 create_prospective_projection(float fovy, float aspect, float near, float far);

    // Matrix multiplication (column-major, compatible with OpenGL uniforms):
    // result = this * other
    mat4 operator*(const mat4& other) const;
};

// 3. Vertex usa memcmp, quindi lasciamolo dopo
struct Vertex
{
    float position[3];
    float normal[3];
    float uv[2];

    void Vzero() {
        memset(this, 0, sizeof(Vertex));
    }

    bool operator==(const Vertex& other) const {
        return std::memcmp(this, &other, sizeof(Vertex)) == 0;
    }

    bool operator<(const Vertex& other) const {
        return std::memcmp(this, &other, sizeof(Vertex)) < 0;
    }
};

// --- Euler angles (gradi): pitch=X, yaw=Y, roll=Z. Ordine applicazione: roll, yaw, pitch. ---
class EulerAngles
{
public:
    float pitch; 
    float yaw;   
    float roll;  

	mat4 toMat4() const;
};

// --- Quaternion (x, y, z, w); norma 1 per rotazione valida. ---
class Quaternion
{
public:
    float x, y, z, w;

    void normalize();
    mat4 toMat4() const;

    static Quaternion identity();
    static Quaternion multiply(const Quaternion& a, const Quaternion& b);
    static Quaternion fromAxisAngle(Vector3 axis, float angleDegrees);
    static Quaternion fromEuler(const EulerAngles& e);
};

mat4 mat4_fromEuler(const EulerAngles& e);

// --- Transform: posizione, rotazione (quaternion), scala. getMatrix() = T * R * S. ---
class Transform
{
public:
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    Transform();

    static Transform getIdentityTransform();
    mat4 getMatrix() const;
    void setQuaternion(float x, float y, float z, float w);
    void setEuler(const EulerAngles& e);
};