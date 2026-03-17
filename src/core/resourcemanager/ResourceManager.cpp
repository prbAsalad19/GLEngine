#ifndef _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif

#include <experimental/filesystem>
#include "resourceManager.h"

const MeshHandle ResourceManager::loadMesh(const std::string& path)
{
	CPUMesh cpu;
	if (!MeshLoader::loadOBJ(path, cpu)) return MeshHandle{};

	std::vector<unsigned int> indices;
	auto mesh = std::make_unique<OpenGLMesh>(MeshLoader::toVertexArray(cpu, indices), cpu.getFaces());

	return meshPool.insert(path, std::move(mesh));
}

OpenGLMesh* ResourceManager::getMesh(MeshHandle handle)
{
	return meshPool.get(handle);
}

void ResourceManager::deleteMesh(MeshHandle handle)
{
	meshPool.remove(handle);
}

const TextureHandle ResourceManager::loadTexture(const std::string& path)
{
	auto texture = std::make_unique<OpenGLTexture>(path.c_str());
	return texturePool.insert(path, std::move(texture));
}

OpenGLTexture* ResourceManager::getTexture(TextureHandle handle)
{
	return texturePool.get(handle);
}

void ResourceManager::deleteTexture(TextureHandle handle)
{
	texturePool.remove(handle);
}

const MaterialHandle ResourceManager::loadMaterial(TextureHandle albedo)
{
	Material mat;
	
	if (albedo.isValid())
	{
		mat.textures["albedo"] = albedo;
	}
	else
	{
		mat.textures["albedo"] = loadTexture("img/defaultTexture.png");
	}

	auto material = std::make_unique<Material>(mat);
	return materialPool.insert("__generated__", std::move(material));
}

Material* ResourceManager::getMaterial(MaterialHandle handle)
{
	return materialPool.get(handle);
}

void ResourceManager::deleteMaterial(MaterialHandle handle)
{
	materialPool.remove(handle);
}