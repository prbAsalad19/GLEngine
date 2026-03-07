#pragma once
#include "core/CoreConfig.h"
#include "opengl/OpenGLConfig.h"

class Material
{
public:
	Material(const char* filename);
	~Material();
	void use(int unit);

private:
	unsigned int texture;
};