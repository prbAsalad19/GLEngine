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
    static mat4 create_model_transform(Vector3 pos, float angle);
    static mat4 create_look_at(Vector3 from, Vector3 to);
    static mat4 create_prospective_projection(float fovy, float aspect, float near, float far);
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