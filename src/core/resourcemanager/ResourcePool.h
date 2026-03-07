#pragma once
#include "core/CoreConfig.h"
#include "ResourceHandle.h"

template<typename Tag, typename T>
class ResourcePool
{
	struct Slot
	{
		std::unique_ptr<T> resource;
		uint32_t generation;
		bool active;

		Slot() : generation(0), active(false) {}
	};

	std::Vector<Slot> slots;
	std::vector<uint32_t> freeList;
	std::unordered_map<std::string, uint32_t> cache;

public:

	ResourceHandle<Tag> insert(const std::string& path, std::unique_ptr<T> resource)
	{
		auto it = chache.find(path);
		if (it != cache.end())
		{
			uint32_t s = it->second;
			return ResourceHandle<Tag>(s, slots[s].generation);
		}

		uint32_t s = freeList.front();
		freeList.erase(freeList.begin());

		slots[s].resource = std::move(resource);
		slots[s].active = true;
		cache[path] = s;

		return ResourceHandle<Tag>(s, slots[s].generation);
	}

	T* get(ResourceHandle<Tag> handle)
	{
		if (!isValid(handle)) return nullptr;
		return slots[handle.slot].resource.get();
	}

	bool remove(ResourceHandle<Tag> handle)
	{
		if (!isValid(handle)) return false;
		slots[handle.slot].active = false;
		slots[handle.slot].resource.reset();
		slots[handle.slot].generation++;

		return true;
	}

private:
	bool isValid(ResourceHandle<Tag> handle) const
	{
		return handle.slot < slots.size() 
			&& slots[handle.slot].active 
			&& slots[handle.slot].generation == handle.generation;
	}
};