#include "algebricOp.h"

mat4 mat4::operator*(const mat4& other) const
{
	// Column-major multiplication: (this) * other
	// index = col*4 + row
	mat4 result;
	for (int col = 0; col < 4; ++col)
	{
		for (int row = 0; row < 4; ++row)
		{
			float sum = 0.0f;
			for (int k = 0; k < 4; ++k)
			{
				sum += entries[k * 4 + row] * other.entries[col * 4 + k];
			}
			result.entries[col * 4 + row] = sum;
		}
	}
	return result;
}

mat4 mat4::create_matrix_transform(Vector3 translation)
{
	mat4 matrix;

	matrix.entries[0] = 1.0f;
	matrix.entries[1] = 0.0f; 
	matrix.entries[2] = 0.0f; 
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = 0.0f;
	matrix.entries[5] = 1.0f;
	matrix.entries[6] = 0.0f;
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = 0.0f;
	matrix.entries[9] = 0.0f;
	matrix.entries[10] = 1.0f;
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = translation.entries[0];
	matrix.entries[13] = translation.entries[1];
	matrix.entries[14] = translation.entries[2];
	matrix.entries[15] = 1.0f;

	return matrix;
}

mat4 mat4::create_z_rotation(float angle)
{
	angle = angle * PI / 180.0f;
	mat4 matrix;

	float c = cos(angle);
	float s = sin(angle);

	matrix.entries[0] = c;
	matrix.entries[1] = s;
	matrix.entries[2] = 0.0f;
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = -s;
	matrix.entries[5] = c;
	matrix.entries[6] = 0.0f;
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = 0.0f;
	matrix.entries[9] = 0.0f;
	matrix.entries[10] = 1.0f;
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = 0.0f;
	matrix.entries[13] = 0.0f;
	matrix.entries[14] = 0.0f;
	matrix.entries[15] = 1.0f;

	return matrix;
}

mat4 mat4::create_x_rotation(float angle)
{
	angle = angle * PI / 180.0f;
	mat4 matrix;

	float c = cos(angle);
	float s = sin(angle);

	matrix.entries[0] = 1.0f;
	matrix.entries[1] = 0.0f;
	matrix.entries[2] = 0.0f;
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = 0.0f;
	matrix.entries[5] = c;
	matrix.entries[6] = s;
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = 0.0f;
	matrix.entries[9] = -s;
	matrix.entries[10] = c;
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = 0.0f;
	matrix.entries[13] = 0.0f;
	matrix.entries[14] = 0.0f;
	matrix.entries[15] = 1.0f;

	return matrix;
}

mat4 mat4::create_y_rotation(float angle)
{
	angle = angle * PI / 180.0f;
	mat4 matrix;

	float c = cos(angle);
	float s = sin(angle);

	matrix.entries[0] = c;
	matrix.entries[1] = 0.0f;
	matrix.entries[2] = -s;
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = 0.0f;
	matrix.entries[5] = 1.0f;
	matrix.entries[6] = 0.0f;
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = s;
	matrix.entries[9] = 0.0f;
	matrix.entries[10] = c;
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = 0.0f;
	matrix.entries[13] = 0.0f;
	matrix.entries[14] = 0.0f;
	matrix.entries[15] = 1.0f;

	return matrix;
}

mat4 mat4::create_scale(Vector3 scale)
{
	mat4 matrix;

	// scaling on the diagonal, no rotation/translation
	matrix.entries[0] = scale.entries[0]; // sx
	matrix.entries[1] = 0.0f;
	matrix.entries[2] = 0.0f;
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = 0.0f;
	matrix.entries[5] = scale.entries[1]; // sy
	matrix.entries[6] = 0.0f;
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = 0.0f;
	matrix.entries[9] = 0.0f;
	matrix.entries[10] = scale.entries[2]; // sz
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = 0.0f;
	matrix.entries[13] = 0.0f;
	matrix.entries[14] = 0.0f;
	matrix.entries[15] = 1.0f;

	return matrix;
}

mat4 mat4::create_model_transform(Vector3 pos, float angle)
{
	angle = angle * PI / 180.0f;
	mat4 matrix;

	float c = cos(angle);
	float s = sin(angle);

	matrix.entries[0] = c;
	matrix.entries[1] = s;
	matrix.entries[2] = 0.0f;
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = -s;
	matrix.entries[5] = c;
	matrix.entries[6] = 0.0f;
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = 0.0f;
	matrix.entries[9] = 0.0f;
	matrix.entries[10] = 1.0f;
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = pos.entries[0];
	matrix.entries[13] = pos.entries[1];
	matrix.entries[14] = pos.entries[2];
	matrix.entries[15] = 1.0f;

	return matrix;
}

mat4 mat4::create_look_at(Vector3 from, Vector3 to)
{
	Vector3 gloab_up = { 0.0f, 0.0f, 1.0f };

	Vector3 f = {
		to.entries[0] - from.entries[0],
		to.entries[1] - from.entries[1],
		to.entries[2] - from.entries[2]
	};
	f.normalize();

	Vector3 r = Vector3::normalize(Vector3::cross(f, gloab_up));
	Vector3 u = Vector3::normalize(Vector3::cross(r, f));

	mat4 matrix;

	matrix.entries[0] = r.entries[0];
	matrix.entries[1] = u.entries[0];
	matrix.entries[2] = -f.entries[0];
	matrix.entries[3] = 0.0f;

	matrix.entries[4] = r.entries[1];
	matrix.entries[5] = u.entries[1];
	matrix.entries[6] = -f.entries[1];
	matrix.entries[7] = 0.0f;

	matrix.entries[8] = r.entries[2];
	matrix.entries[9] = u.entries[2];
	matrix.entries[10] = -f.entries[2];
	matrix.entries[11] = 0.0f;

	matrix.entries[12] = -Vector3::dot(r, from);
	matrix.entries[13] = -Vector3::dot(u, from);
	matrix.entries[14] = Vector3::dot(f, from);
	matrix.entries[15] = 1.0f;

	return matrix;
}


mat4 mat4::create_prospective_projection(float fovy, float aspect, float near, float far)
{
	fovy = fovy * PI / 360.0f; //half the fild of view in radians

	float t = tanf(fovy);
	float n = -near;
	float f = -far;

	mat4 matrix;

	for (int i = 0; i < 16; i++)
	{
		matrix.entries[i] = 0.0f;
	}

	matrix.entries[0] = 1.0f / (aspect * t);
	matrix.entries[5] = 1.0f / t;
	matrix.entries[10] = -(n + f) / (n - f);
	matrix.entries[11] = -1.0f;
	matrix.entries[14] = (2.0f * n * f) / (n - f);

	return matrix;
}

float Vector3::dot(Vector3 u, Vector3 v)
{
	return u.entries[0] * v.entries[0] 
		+ u.entries[1] * v.entries[1] 
		+ u.entries[2] * v.entries[2];
}

void Vector3::normalize()
{
	float mag = sqrtf(entries[0] * entries[0] + entries[1] * entries[1] + entries[2] * entries[2]);
	if (mag > 0.0f)
	{
		entries[0] /= mag;
		entries[1] /= mag;
		entries[2] /= mag;
	}
}

Vector3 Vector3::normalize(Vector3 v)
{
	v.normalize();
	return v;
}

Vector3 Vector3::cross(Vector3 u, Vector3 v)
{
	Vector3 w;

	w.entries[0] = u.entries[1] * v.entries[2] - u.entries[2] * v.entries[1];
	w.entries[1] = -(u.entries[0] * v.entries[2] - u.entries[2] * v.entries[0]);
	w.entries[2] = u.entries[0] * v.entries[1] - u.entries[1] * v.entries[0];

	return w;
}

// --- Euler -> mat4 (ordine: roll Z, yaw Y, pitch X -> R = Rz * Ry * Rx) ---
mat4 mat4_fromEuler(const EulerAngles& e)
{
	return e.toMat4();
}

mat4 mat4_fromQuaternion(const Quaternion& q)
{
	return q.toMat4();
}

mat4 EulerAngles::toMat4() const
{
	mat4 Rz = mat4::create_z_rotation(roll);
	mat4 Ry = mat4::create_y_rotation(yaw);
	mat4 Rx = mat4::create_x_rotation(pitch);
	return Rz * Ry * Rx;;
}

// --- Quaternion ---
Quaternion Quaternion::identity()
{
	Quaternion q;
	q.x = 0.0f;
	q.y = 0.0f;
	q.z = 0.0f;
	q.w = 1.0f;
	return q;
}

void Quaternion::normalize()
{
	float len = sqrtf(x * x + y * y + z * z + w * w);
	if (len > 0.0f)
	{
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}
}

Quaternion Quaternion::multiply(const Quaternion& a, const Quaternion& b)
{
	Quaternion r;
	r.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
	r.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
	r.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
	r.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
	return r;
}

Quaternion Quaternion::fromAxisAngle(Vector3 axis, float angleDegrees)
{
	axis.normalize();
	float rad = angleDegrees * PI / 180.0f;
	float half = rad * 0.5f;
	float s = sinf(half);
	Quaternion q;
	q.x = axis.entries[0] * s;
	q.y = axis.entries[1] * s;
	q.z = axis.entries[2] * s;
	q.w = cosf(half);
	q.normalize();
	return q;
}

Quaternion Quaternion::fromEuler(const EulerAngles& e)
{
	// Stesso ordine di mat4_fromEuler: roll, yaw, pitch -> q = q_pitch * q_yaw * q_roll
	float pitchRad = e.pitch * PI / 180.0f * 0.5f;
	float yawRad = e.yaw * PI / 180.0f * 0.5f;
	float rollRad = e.roll * PI / 180.0f * 0.5f;
	float cp = cosf(pitchRad), sp = sinf(pitchRad);
	float cy = cosf(yawRad), sy = sinf(yawRad);
	float cr = cosf(rollRad), sr = sinf(rollRad);
	Quaternion q;
	q.w = cr * cy * cp + sr * sy * sp;
	q.x = sr * cy * cp - cr * sy * sp;
	q.y = cr * sy * cp + sr * cy * sp;
	q.z = cr * cy * sp - sr * sy * cp;
	return q;
}

mat4 Quaternion::toMat4() const
{
	float xx = x * x, yy = y * y, zz = z * z;
	float xy = x * y, xz = x * z, yz = y * z;
	float wx = w * x, wy = w * y, wz = w * z;

	mat4 m;
	m.entries[0] = 1.0f - 2.0f * (yy + zz);
	m.entries[1] = 2.0f * (xy + wz);
	m.entries[2] = 2.0f * (xz - wy);
	m.entries[3] = 0.0f;

	m.entries[4] = 2.0f * (xy - wz);
	m.entries[5] = 1.0f - 2.0f * (xx + zz);
	m.entries[6] = 2.0f * (yz + wx);
	m.entries[7] = 0.0f;

	m.entries[8] = 2.0f * (xz + wy);
	m.entries[9] = 2.0f * (yz - wx);
	m.entries[10] = 1.0f - 2.0f * (xx + yy);
	m.entries[11] = 0.0f;

	m.entries[12] = 0.0f;
	m.entries[13] = 0.0f;
	m.entries[14] = 0.0f;
	m.entries[15] = 1.0f;
	return m;
}

// --- Transform ---
Transform::Transform()
{
	position.entries[0] = 0.0f;
	position.entries[1] = 0.0f;
	position.entries[2] = 0.0f;
	rotation = Quaternion::identity();
	scale.entries[0] = 1.0f;
	scale.entries[1] = 1.0f;
	scale.entries[2] = 1.0f;
}

mat4 Transform::getMatrix() const
{
	mat4 T = mat4::create_matrix_transform(position);
	mat4 R = rotation.toMat4();
	mat4 S = mat4::create_scale(scale);
	return T * R * S;
}

void Transform::setQuaternion(float x, float y, float z, float w)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
	rotation.w = w;
}

void Transform::setEuler(const EulerAngles& e)
{
	rotation = Quaternion::fromEuler(e);
}

Transform Transform::getIdentityTransform()
{
	Transform t;
	t.position.entries[0] = 0.0f;
	t.position.entries[1] = 0.0f;
	t.position.entries[2] = 0.0f;
	t.rotation = Quaternion::identity();
	t.scale.entries[0] = 1.0f;
	t.scale.entries[1] = 1.0f;
	t.scale.entries[2] = 1.0f;
	return t;
}