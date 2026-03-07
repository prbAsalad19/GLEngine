#pragma once
#include "core/CoreConfig.h"
#include "core/scene/Transform.h"
#include "core/assets/CPUMesh.h"
#include "core/assets/MeshLoader.h"
#include "opengl/OpenGLMesh.h"
#include "ResourcePool.h"

class ResourceManager
{
	ResourcePool<MeshTag, OpenGLMesh> meshPool;
	ResourcePool<TextureTag, Texture> texturePool;
	ResourcePool<MaterialTag, Material> materialPool;

public:
	const MeshHandle loadMesh(const std::string& filepath);
	OpenGLMesh* getMesh(MeshHandle handle);
	void deleteMesh(MeshHandle handle);

};
