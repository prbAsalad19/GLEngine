#pragma once
#include "opengl/OpenGLTexture.h"
#include "core/resourcemanager/ResourceHandle.h"

struct Material
{
	std::unordered_map<std::string, TextureHandle> textures;
};