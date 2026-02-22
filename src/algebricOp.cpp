#include "algebricOp.h"

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