#pragma once
#include "config.h"
#include "objLoader.h"

struct meshSlot
{
	std::unique_ptr<Mesh> resource;
	uint32_t generation;
	bool active;
};

class ResourceManager
{
	std::unordered_map<std::string, uint32_t> meshCache;
	std::vector<meshSlot> meshes;
	std::vector<uint32_t> freeSlotList;

	const void getFreeSlots();
	const uint32_t getFirstFreeSlot();

public:
	const uint32_t loadMesh(const std::string& filepath, Transform transform = Transform::getIdentityTransform());

};
