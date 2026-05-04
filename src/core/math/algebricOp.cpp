#include "algebricOp.h"
#include <cmath>
#include <cstdlib>

// ─────────────────────────────────────────────
//  mat4
// ─────────────────────────────────────────────

mat4 mat4::operator*(const mat4& other) const
{
    // Column-major: index = col*4 + row
    mat4 result;
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row)
        {
            float sum = 0.0f;
            for (int k = 0; k < 4; ++k)
                sum += entries[k * 4 + row] * other.entries[col * 4 + k];
            result.entries[col * 4 + row] = sum;
        }
    return result;
}

mat4 mat4::create_matrix_transform(Vector3 t)
{
    mat4 m;
    m.entries[0]  = 1.0f; m.entries[1]  = 0.0f; m.entries[2]  = 0.0f; m.entries[3]  = 0.0f;
    m.entries[4]  = 0.0f; m.entries[5]  = 1.0f; m.entries[6]  = 0.0f; m.entries[7]  = 0.0f;
    m.entries[8]  = 0.0f; m.entries[9]  = 0.0f; m.entries[10] = 1.0f; m.entries[11] = 0.0f;
    m.entries[12] = t.entries[0];
    m.entries[13] = t.entries[1];
    m.entries[14] = t.entries[2];
    m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_scale(Vector3 s)
{
    mat4 m;
    m.entries[0]  = s.entries[0]; m.entries[1]  = 0.0f; m.entries[2]  = 0.0f; m.entries[3]  = 0.0f;
    m.entries[4]  = 0.0f; m.entries[5]  = s.entries[1]; m.entries[6]  = 0.0f; m.entries[7]  = 0.0f;
    m.entries[8]  = 0.0f; m.entries[9]  = 0.0f; m.entries[10] = s.entries[2]; m.entries[11] = 0.0f;
    m.entries[12] = 0.0f; m.entries[13] = 0.0f; m.entries[14] = 0.0f;         m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_x_rotation(float angleDeg)
{
    float a = angleDeg * PI / 180.0f;
    float c = cosf(a), s = sinf(a);
    mat4 m;
    m.entries[0]  = 1.0f; m.entries[1]  = 0.0f; m.entries[2]  = 0.0f; m.entries[3]  = 0.0f;
    m.entries[4]  = 0.0f; m.entries[5]  = c;    m.entries[6]  = s;    m.entries[7]  = 0.0f;
    m.entries[8]  = 0.0f; m.entries[9]  = -s;   m.entries[10] = c;    m.entries[11] = 0.0f;
    m.entries[12] = 0.0f; m.entries[13] = 0.0f; m.entries[14] = 0.0f; m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_y_rotation(float angleDeg)
{
    float a = angleDeg * PI / 180.0f;
    float c = cosf(a), s = sinf(a);
    mat4 m;
    m.entries[0]  = c;    m.entries[1]  = 0.0f; m.entries[2]  = -s;   m.entries[3]  = 0.0f;
    m.entries[4]  = 0.0f; m.entries[5]  = 1.0f; m.entries[6]  = 0.0f; m.entries[7]  = 0.0f;
    m.entries[8]  = s;    m.entries[9]  = 0.0f; m.entries[10] = c;    m.entries[11] = 0.0f;
    m.entries[12] = 0.0f; m.entries[13] = 0.0f; m.entries[14] = 0.0f; m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_z_rotation(float angleDeg)
{
    float a = angleDeg * PI / 180.0f;
    float c = cosf(a), s = sinf(a);
    mat4 m;
    m.entries[0]  = c;    m.entries[1]  = s;    m.entries[2]  = 0.0f; m.entries[3]  = 0.0f;
    m.entries[4]  = -s;   m.entries[5]  = c;    m.entries[6]  = 0.0f; m.entries[7]  = 0.0f;
    m.entries[8]  = 0.0f; m.entries[9]  = 0.0f; m.entries[10] = 1.0f; m.entries[11] = 0.0f;
    m.entries[12] = 0.0f; m.entries[13] = 0.0f; m.entries[14] = 0.0f; m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_model_transform(Vector3 pos, float angleDeg)
{
    // Combined Z-rotation + translation (legacy helper)
    float a = angleDeg * PI / 180.0f;
    float c = cosf(a), s = sinf(a);
    mat4 m;
    m.entries[0]  = c;              m.entries[1]  = s;              m.entries[2]  = 0.0f; m.entries[3]  = 0.0f;
    m.entries[4]  = -s;             m.entries[5]  = c;              m.entries[6]  = 0.0f; m.entries[7]  = 0.0f;
    m.entries[8]  = 0.0f;           m.entries[9]  = 0.0f;           m.entries[10] = 1.0f; m.entries[11] = 0.0f;
    m.entries[12] = pos.entries[0]; m.entries[13] = pos.entries[1]; m.entries[14] = pos.entries[2]; m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_look_at(Vector3 from, Vector3 to)
{
    const Vector3 worldUp = { 0.0f, 0.0f, 1.0f };

    Vector3 f = { to.entries[0] - from.entries[0],
                  to.entries[1] - from.entries[1],
                  to.entries[2] - from.entries[2] };
    f.normalize();

    Vector3 r = Vector3::normalize(Vector3::cross(f, worldUp));
    Vector3 u = Vector3::normalize(Vector3::cross(r, f));

    mat4 m;
    m.entries[0]  = r.entries[0]; m.entries[1]  = u.entries[0]; m.entries[2]  = -f.entries[0]; m.entries[3]  = 0.0f;
    m.entries[4]  = r.entries[1]; m.entries[5]  = u.entries[1]; m.entries[6]  = -f.entries[1]; m.entries[7]  = 0.0f;
    m.entries[8]  = r.entries[2]; m.entries[9]  = u.entries[2]; m.entries[10] = -f.entries[2]; m.entries[11] = 0.0f;
    m.entries[12] = -Vector3::dot(r, from);
    m.entries[13] = -Vector3::dot(u, from);
    m.entries[14] =  Vector3::dot(f, from);
    m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::calculate_inverse_view(Vector3 from, Vector3 to) {
    const Vector3 worldUp = { 0.0f, 0.0f, 1.0f };
    Vector3 f = { to.entries[0] - from.entries[0], to.entries[1] - from.entries[1], to.entries[2] - from.entries[2] };
    f.normalize();

    Vector3 r = Vector3::normalize(Vector3::cross(f, worldUp));
    Vector3 u = Vector3::normalize(Vector3::cross(r, f));

    mat4 m;
    // La parte 3x3 è semplicemente la trasposta della rotazione della View
    m.entries[0] = r.entries[0];  m.entries[1] = r.entries[1];  m.entries[2] = r.entries[2];  m.entries[3] = 0.0f;
    m.entries[4] = u.entries[0];  m.entries[5] = u.entries[1];  m.entries[6] = u.entries[2];  m.entries[7] = 0.0f;
    m.entries[8] = -f.entries[0]; m.entries[9] = -f.entries[1]; m.entries[10] = -f.entries[2]; m.entries[11] = 0.0f;
    
    // La traslazione dell'inversa è semplicemente la posizione della camera nel mondo
    m.entries[12] = from.entries[0];
    m.entries[13] = from.entries[1];
    m.entries[14] = from.entries[2];
    m.entries[15] = 1.0f;
    return m;
}

mat4 mat4::create_prospective_projection(float fovy, float aspect, float nearPlane, float farPlane)
{
    float halfFov = fovy * PI / 360.0f;
    float t = tanf(halfFov);
    float n = nearPlane;
    float f = farPlane;

    mat4 m;
    for (int i = 0; i < 16; ++i) m.entries[i] = 0.0f;
    m.entries[0]  = 1.0f / (aspect * t);
    m.entries[5]  = 1.0f / t;
    m.entries[10] = -(f + n) / (f - n);
    m.entries[11] = -1.0f;
    m.entries[14] = -(2.0f * n * f) / (f - n);
    return m;
}
mat4 mat4::calculate_inverse_projection(float fovy, float aspect, float nearPlane, float farPlane)
{
    float halfFov = fovy * PI / 360.0f;
    float t = tanf(halfFov);
    float n = nearPlane;
    float f = farPlane;

    mat4 m;
    for (int i = 0; i < 16; ++i) m.entries[i] = 0.0f;

    m.entries[0]  = aspect * t;                      // col0, row0
    m.entries[5]  = t;                               // col1, row1
    m.entries[11] = (n - f) / (2.0f * n * f);        // col2, row3  ← was wrongly -1
    m.entries[14] = -1.0f;                           // col3, row2
    m.entries[15] = (n + f) / (2.0f * n * f);        // col3, row3
    // entries[13] intentionally stays 0             // col3, row1  ← was wrongly ≠ 0

    return m;
}

// mat4 mat4::calculate_inverse_projection(float fovy, float aspect, float nearPlane, float farPlane) {
//     float halfFov = fovy * PI / 360.0f;
//     float t = tanf(halfFov);
//     float n = nearPlane;
//     float f = farPlane;

//     mat4 m;
//     for (int i = 0; i < 16; ++i) m.entries[i] = 0.0f;

//     m.entries[0]  = aspect * t;
//     m.entries[5]  = t;
    
//     // In una proiezione inversa standard:
//     // m.entries[14] mappa Z = -W (Riga 2, Colonna 3)
//     m.entries[14] = -1.0f; 

//     // m.entries[11] e [15] decodificano la depth non-lineare
//     m.entries[11] = -(f - n) / (2.0f * n * f); // <-- Corretto (Riga 3, Colonna 2)
//     m.entries[15] =  (f + n) / (2.0f * n * f); // <-- Corretto (Riga 3, Colonna 3)
    
//     return m;
// }

// mat4 mat4::calculate_inverse_projection(float fovy, float aspect, float nearPlane, float farPlane) {
//     float halfFov = fovy * PI / 360.0f;
//     float t = tanf(halfFov);
//     float n = nearPlane;
//     float f = farPlane;

//     mat4 m;
//     for (int i = 0; i < 16; ++i) m.entries[i] = 0.0f;

//     m.entries[0]  = aspect * t;
//     m.entries[5]  = t;
//     m.entries[11] = -1.0f;
//     m.entries[14] = -1.0f;
//     m.entries[13] = -(f - n) / (2.0f * n * f);
//     m.entries[15] = (f + n) / (2.0f * n * f);
    
//     return m;
// }

// ─────────────────────────────────────────────
//  Vector4
// ─────────────────────────────────────────────

float Vector4::mag(Vector4 u)
{
    return u.mag();
}

float Vector4::mag()
{
    return std::sqrt(x * x + y * y + z * z + w * w);
}

Vector4 Vector4::operator+(const Vector4& other) const
{
    return {
        entries[0] + other.entries[0],
        entries[1] + other.entries[1],
        entries[2] + other.entries[2],
        entries[3] + other.entries[3]
    };
}

Vector4 Vector4::operator-(const Vector4& other) const
{
    return {
        entries[0] - other.entries[0],
        entries[1] - other.entries[1],
        entries[2] - other.entries[2],
        entries[3] - other.entries[3]
    };
}

Vector4 Vector4::operator*(float scalar) const
{
    return {
        entries[0] * scalar,
        entries[1] * scalar,
        entries[2] * scalar,
        entries[3] * scalar
    };
}

float Vector4::dot(Vector4 u, Vector4 v)
{
    return u.entries[0] * v.entries[0]
         + u.entries[1] * v.entries[1]
         + u.entries[2] * v.entries[2]
         + u.entries[3] * v.entries[3];
}

void Vector4::normalize()
{
    float mag = sqrtf(entries[0]*entries[0] + entries[1]*entries[1] + entries[2]*entries[2] + entries[3]*entries[3]);
    if (mag > 0.0f) {
        entries[0] /= mag;
        entries[1] /= mag;
        entries[2] /= mag;
        entries[3] /= mag;
    }
}

Vector4 Vector4::normalize(Vector4 v)
{
    v.normalize();
    return v;
}

float Vector4::dist(Vector4 u, Vector4 v)
{
    return std::sqrtf(std::fabs(u.x - v.x) * std::fabs(u.x - v.x)
                     + std::fabs(u.y -v.y) * std::fabs(u.y -v.y)
                     + std::fabs(u.z -v.z) * std::fabs(u.z -v.z)
                     + std::fabs(u.w - v.w) * std::fabs(u.w - v.w));
}

Vector4 Vector4::midpoint(Vector4 u, Vector4 v)
{
    return{
        (u.entries[0] + v.entries[0]) / 2.0f,
        (u.entries[1] + v.entries[1]) / 2.0f,
        (u.entries[2] + v.entries[2]) / 2.0f,
        (u.entries[3] + v.entries[3]) / 2.0f
    };
}

Vector4 Vector4::Vmin(Vector4 u, Vector4 v)
{
    return {
        std::min(u.entries[0], v.entries[0]),
        std::min(u.entries[1], v.entries[1]),
        std::min(u.entries[2], v.entries[2]),
        std::min(u.entries[3], v.entries[3]),
    };
}

Vector4 Vector4::Vmax(Vector4 u, Vector4 v)
{
        return {
        std::max(u.entries[0], v.entries[0]),
        std::max(u.entries[1], v.entries[1]),
        std::max(u.entries[2], v.entries[2]),
        std::max(u.entries[3], v.entries[3]),
    };
}

// ─────────────────────────────────────────────
//  Vector3
// ─────────────────────────────────────────────

float Vector3::mag(Vector3 u)
{
    return u.mag();
}

float Vector3::mag()
{
    return std::sqrt(x * x + y * y + z * z);
}

Vector3 Vector3::operator+(const Vector3& other) const
{
    return {
        entries[0] + other.entries[0],
        entries[1] + other.entries[1],
        entries[2] + other.entries[2]
    };
}

Vector3 Vector3::operator-(const Vector3& other) const
{
    return {
        entries[0] - other.entries[0],
        entries[1] - other.entries[1],
        entries[2] - other.entries[2]
    };
}

Vector3 Vector3::operator*(float scalar) const
{
    return {
        entries[0] * scalar,
        entries[1] * scalar,
        entries[2] * scalar
    };
}

float Vector3::dot(Vector3 u, Vector3 v)
{
    return u.entries[0] * v.entries[0]
         + u.entries[1] * v.entries[1]
         + u.entries[2] * v.entries[2];
}

void Vector3::normalize()
{
    float mag = sqrtf(entries[0]*entries[0] + entries[1]*entries[1] + entries[2]*entries[2]);
    if (mag > 0.0f) { entries[0] /= mag; entries[1] /= mag; entries[2] /= mag; }
}

Vector3 Vector3::normalize(Vector3 v)
{
    v.normalize();
    return v;
}

Vector3 Vector3::cross(Vector3 u, Vector3 v)
{
    return {
        u.entries[1] * v.entries[2] - u.entries[2] * v.entries[1],
       -(u.entries[0] * v.entries[2] - u.entries[2] * v.entries[0]),
        u.entries[0] * v.entries[1] - u.entries[1] * v.entries[0]
    };
}

float Vector3::dist(Vector3 u, Vector3 v)
{
    return std::sqrtf(std::fabs(u.x - v.x) * std::fabs(u.x - v.x)
                     + std::fabs(u.y -v.y) * std::fabs(u.y -v.y)
                     + std::fabs(u.z -v.z) * std::fabs(u.z -v.z));
}

Vector3 Vector3::midpoint(Vector3 u, Vector3 v)
{
    return{
        (u.entries[0] + v.entries[0]) / 2.0f,
        (u.entries[1] + v.entries[1]) / 2.0f,
        (u.entries[2] + v.entries[2]) / 2.0f
    };
}

Vector3 Vector3::Vmin(Vector3 u, Vector3 v)
{
    return {
        std::min(u.entries[0], v.entries[0]),
        std::min(u.entries[1], v.entries[1]),
        std::min(u.entries[2], v.entries[2]),
    };
}

Vector3 Vector3::Vmax(Vector3 u, Vector3 v)
{
        return {
        std::max(u.entries[0], v.entries[0]),
        std::max(u.entries[1], v.entries[1]),
        std::max(u.entries[2], v.entries[2]),
    };
}

Vector3 Vector3::targetFromEuler(const EulerAngles& e)
{
    float cp = cosf(e.pitch * PI / 180.0f);
    float sp = sinf(e.pitch * PI / 180.0f);
    float cy = cosf(e.yaw   * PI / 180.0f);
    float sy = sinf(e.yaw   * PI / 180.0f);
    return { cy*cp, sy*cp, sp };
}

Vector3 Vector3::targetFromQuaternion(const Quaternion& q)
{
    return {
        2.0f * (q.x*q.z + q.w*q.y),
        2.0f * (q.y*q.z - q.w*q.x),
        1.0f - 2.0f * (q.x*q.x + q.y*q.y)
    };
}

// ─────────────────────────────────────────────
//  Vector2
// ─────────────────────────────────────────────

float Vector2::mag(Vector2 u)
{
    return u.mag();
}

float Vector2::mag()
{
    return std::sqrt(x * x + y * y);
}

Vector2 Vector2::operator+(const Vector2& other) const
{
    return {
        entries[0] + other.entries[0],
        entries[1] + other.entries[1]
    };
}

Vector2 Vector2::operator-(const Vector2& other) const
{
    return {
        entries[0] - other.entries[0],
        entries[1] - other.entries[1]
    };
}

Vector2 Vector2::operator*(float scalar) const
{
    return {
        entries[0] * scalar,
        entries[1] * scalar
    };
}

float Vector2::dot(Vector2 u, Vector2 v)
{
    return u.entries[0] * v.entries[0]
        + u.entries[1] * v.entries[1];
}

void Vector2::normalize()
{
    float mag = sqrtf(entries[0]*entries[0] + entries[1]*entries[1]);
	if (mag > 0.0f) { entries[0] /= mag; entries[1] /= mag; }
}

Vector2 Vector2::normalize(Vector2 v)
{
    v.normalize();
    return v;
}

float Vector2::dist(Vector2 u, Vector2 v)
{
    return std::sqrtf(std::fabs(u.x - v.x) * std::fabs(u.x - v.x)
                     + std::fabs(u.y -v.y) * std::fabs(u.y -v.y));
}

// ─────────────────────────────────────────────
//  EulerAngles
// ─────────────────────────────────────────────

mat4 EulerAngles::toMat4() const
{
    // R = Rz * Ry * Rx
    return mat4::create_z_rotation(roll)
         * mat4::create_y_rotation(yaw)
         * mat4::create_x_rotation(pitch);
}

// ─────────────────────────────────────────────
//  Quaternion
// ─────────────────────────────────────────────

Quaternion Quaternion::identity()
{
    return { 0.0f, 0.0f, 0.0f, 1.0f };
}

void Quaternion::normalize()
{
    float len = sqrtf(x*x + y*y + z*z + w*w);
    if (len > 0.0f) { x /= len; y /= len; z /= len; w /= len; }
}

Quaternion Quaternion::multiply(const Quaternion& a, const Quaternion& b)
{
    return {
        a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y,
        a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x,
        a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w,
        a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z
    };
}

Quaternion Quaternion::fromAxisAngle(Vector3 axis, float angleDeg)
{
    axis.normalize();
    float half = angleDeg * PI / 360.0f;
    float s = sinf(half);
    Quaternion q = { axis.entries[0]*s, axis.entries[1]*s, axis.entries[2]*s, cosf(half) };
    q.normalize();
    return q;
}

Quaternion Quaternion::fromEuler(const EulerAngles& e)
{
    float cp = cosf(e.pitch * PI / 360.0f), sp = sinf(e.pitch * PI / 360.0f);
    float cy = cosf(e.yaw   * PI / 360.0f), sy = sinf(e.yaw   * PI / 360.0f);
    float cr = cosf(e.roll  * PI / 360.0f), sr = sinf(e.roll  * PI / 360.0f);
    return {
        sr*cy*cp - cr*sy*sp,
        cr*sy*cp + sr*cy*sp,
        cr*cy*sp - sr*sy*cp,
        cr*cy*cp + sr*sy*sp
    };
}

mat4 Quaternion::toMat4() const
{
    float xx=x*x, yy=y*y, zz=z*z;
    float xy=x*y, xz=x*z, yz=y*z;
    float wx=w*x, wy=w*y, wz=w*z;

    mat4 m;
    m.entries[0]  = 1.0f - 2.0f*(yy+zz); m.entries[1]  = 2.0f*(xy+wz);        m.entries[2]  = 2.0f*(xz-wy);        m.entries[3]  = 0.0f;
    m.entries[4]  = 2.0f*(xy-wz);        m.entries[5]  = 1.0f - 2.0f*(xx+zz); m.entries[6]  = 2.0f*(yz+wx);        m.entries[7]  = 0.0f;
    m.entries[8]  = 2.0f*(xz+wy);        m.entries[9]  = 2.0f*(yz-wx);        m.entries[10] = 1.0f - 2.0f*(xx+yy); m.entries[11] = 0.0f;
    m.entries[12] = 0.0f;                 m.entries[13] = 0.0f;                 m.entries[14] = 0.0f;                 m.entries[15] = 1.0f;
    return m;
}

EulerAngles Quaternion::getEuler() const
{
    EulerAngles e;

    // pitch (X) — rotazione attorno all'asse X
    float sinPitch = 2.0f * (w * x + y * z);
    float cosPitch = 1.0f - 2.0f * (x * x + y * y);
    e.pitch = atan2f(sinPitch, cosPitch) * 180.0f / PI;

    // yaw (Y) — rotazione attorno all'asse Y
    float sinYaw = 2.0f * (w * y - z * x);
    if (fabsf(sinYaw) >= 1.0f)
        e.yaw = copysignf(90.0f, sinYaw);  // gimbal lock — clamp a ±90°
    else
        e.yaw = asinf(sinYaw) * 180.0f / PI;

    // roll (Z) — rotazione attorno all'asse Z
    float sinRoll = 2.0f * (w * z + x * y);
    float cosRoll = 1.0f - 2.0f * (y * y + z * z);
    e.roll = atan2f(sinRoll, cosRoll) * 180.0f / PI;

    return e;
}
