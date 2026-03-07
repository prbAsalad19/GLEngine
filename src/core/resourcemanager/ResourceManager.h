#pragma once
#include "core/CoreConfig.h"
#include "core/scene/Transform.h"
#include "core/assets/CPUMesh.h"
#include "core/assets/MeshLoader.h"
#include "opengl/OpenGLMesh.h"
#include "ResourcePool.h"

template<typename T>
struct ResourceSlot
{
	std::unique_ptr<T> resource;
	uint32_t generation = 0;
	bool active = false;
};

class ResourceManager
{
	ResourcePool<MeshTag, OpenGLMesh> meshPool;
	ResourcePool<TextureTag, Texture> texturePool;
	ResourcePool<MaterialTag, Material> materialPool;

	const void getFreeSlots();
	const uint32_t getFirstFreeSlot();

public:
	const MeshHandle loadMesh(const std::string& filepath);
	const int deleteMesh(meshHandle handle);

};
