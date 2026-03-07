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