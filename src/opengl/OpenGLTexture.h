#pragma once
#include "opengl/OpenGLConfig.h"
#include "core/assets/ImageData.h"

class OpenGLTexture
{
public:
	OpenGLTexture(const char* filename);
	~OpenGLTexture();
	void use(int unit);

private:
	unsigned int texture;
};