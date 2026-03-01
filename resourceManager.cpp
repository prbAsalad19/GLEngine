#ifndef _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif

#include <experimental/filesystem>
#include "config.h"
#include "resourceManager.h"

const uint32_t ResourceManager::loadMesh(const std::string& filepath, Transform transform)
{
	std::string canonicalPath = std::experimental::filesystem::canonical(filepath).string();
	if (meshCache.find(canonicalPath) != meshCache.end())
	{
		return meshCache[canonicalPath];
	}

	uint32_t slot = getFirstFreeSlot();
	CPUMesh cpuMesh;
	std::vector<unsigned int> vertices;
	if (!MeshLoader::loadOBJ(canonicalPath, cpuMesh))
	{
		std::cout << "Failed to create mesh resource" << canonicalPath << std::endl;
		return static_cast<uint32_t>(-1);
	}
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(MeshLoader::toVertexArray(cpuMesh, vertices), cpuMesh.getFaces(), transform);
	meshes[slot].resource = std::move(mesh);
	meshes[slot].active = true;
	meshCache[canonicalPath] = slot;

	return slot;
}

const void ResourceManager::getFreeSlots()
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		if (!meshes[i].active)
		{
			freeSlotList.push_back(i);
		}
	}
}

const uint32_t ResourceManager::getFirstFreeSlot()
{
	uint32_t slot = freeSlotList[0];
	for (size_t i = 1; i < meshes.size(); i++)
	{
		freeSlotList[i - 1] = freeSlotList[i];
	}
	freeSlotList.pop_back();
	return slot;
}
