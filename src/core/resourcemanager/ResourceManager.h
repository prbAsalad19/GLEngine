#pragma once
#include "core/CoreConfig.h"
#include "core/scene/Transform.h"
#include "core/assets/CPUMesh.h"
#include "core/assets/MeshLoader.h"
#include "core/assets/Material.h"
#include "opengl/OpenGLMesh.h"
#include "opengl/OpenGLTexture.h"
#include "ResourcePool.h"

class ResourceManager
{
	ResourcePool<MeshTag, OpenGLMesh> meshPool;
	ResourcePool<TextureTag, OpenGLTexture> texturePool;
	ResourcePool<MaterialTag, Material> materialPool;

	std::unordered_map<TextureHandle, std::string> texturePaths;

public:
	const MeshHandle loadMesh(const std::string& filepath);
	OpenGLMesh* getMesh(MeshHandle handle);
	void deleteMesh(MeshHandle handle);

	const TextureHandle loadTexture(const std::string& filepath);
	OpenGLTexture* getTexture(TextureHandle handle);
	void deleteTexture(TextureHandle handle);

	const MaterialHandle loadMaterial(TextureHandle baseTexture);
	Material* getMaterial(MaterialHandle handle);
	void deleteMaterial(MaterialHandle handle);
};
