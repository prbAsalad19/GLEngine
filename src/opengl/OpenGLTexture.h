#pragma once
#include "opengl/OpenGLConfig.h"
#include "core/assets/ImageData.h"

class Material
{
public:
	Material(const char* filename);
	~Material();
	void use(int unit);

private:
	unsigned int texture;
};