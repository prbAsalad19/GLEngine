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