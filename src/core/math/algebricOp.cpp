#include "algebricOp.h"

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

mat4 mat4::create_prospective_projection(float fovy, float aspect, float nearPlane, float farPlane)
{
    float halfFov = fovy * PI / 360.0f;
    float t = tanf(halfFov);
    float n = -nearPlane;
    float f = -farPlane;

    mat4 m;
    for (int i = 0; i < 16; ++i) m.entries[i] = 0.0f;
    m.entries[0]  = 1.0f / (aspect * t);
    m.entries[5]  = 1.0f / t;
    m.entries[10] = -(n + f) / (n - f);
    m.entries[11] = -1.0f;
    m.entries[14] = (2.0f * n * f) / (n - f);
    return m;
}

// ─────────────────────────────────────────────
//  Vector3
// ─────────────────────────────────────────────

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

// ─────────────────────────────────────────────
//  Transform
// ─────────────────────────────────────────────

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
