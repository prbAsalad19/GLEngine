#ifndef _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#endif

#include <experimental/filesystem>
#include "config.h"
#include "resourceManager.h"

const meshHandle ResourceManager::loadMesh(const std::string& filepath, Transform transform)
{
	meshHandle handle(0, 0);
	std::string canonicalPath = std::experimental::filesystem::canonical(filepath).string();
	if (meshCache.find(canonicalPath) != meshCache.end())
	{
		handle.slot = meshCache[canonicalPath];
		handle.generation = meshes[handle.slot].generation;
		return handle;
	}
	
	uint32_t slot = getFirstFreeSlot();
	CPUMesh cpuMesh;
	std::vector<unsigned int> vertices;
	if (!MeshLoader::loadOBJ(canonicalPath, cpuMesh))
	{
		std::cout << "Failed to create mesh resource" << canonicalPath << std::endl;
		return meshHandle(static_cast<uint32_t>(-1), 0);
	}
	std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(MeshLoader::toVertexArray(cpuMesh, vertices), cpuMesh.getFaces(), transform);
	meshes[slot].resource = std::move(mesh);
	meshes[slot].active = true;
	meshCache[canonicalPath] = slot;
	handle.slot = slot;
	handle.generation = meshes[slot].generation;

	return handle;
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

const int ResourceManager::deleteMesh(meshHandle handle)
{
	if (handle.slot >= meshes.size() || handle.generation != meshes[handle.slot].generation)
	{
		return -1;
	}
	meshes[handle.slot].active = false;
	meshes[handle.slot].resource.reset();
	meshes[handle.slot].generation++;
	auto mapIt = std::find_if(meshCache.begin(), meshCache.end(), [handle](const std::pair<const std::string, uint32_t>& pair) { return pair.second == handle.slot; });
	if (mapIt != meshCache.end())
	{
		meshCache.erase(mapIt);
	}
	auto it = std::lower_bound(freeSlotList.begin(), freeSlotList.end(), handle.slot);
	freeSlotList.insert(it, handle.slot);
	return 0;
}