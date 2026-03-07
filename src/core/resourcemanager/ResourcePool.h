#pragma once
#include "core/CoreConfig.h"
#include "ResourceHandle.h"

template<typename Tag, typename T>
class ResourcePool
{
	explicit ResourcePool(uint32_t capacity = 64)
	{
		slots.resize(capacity);
		freeList.resize(capacity);
		for (uint32_t i = 0; i < capacity; ++i)
			freeList[i] = i;
	} //making the free list a non null vector

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
		auto it = cache.find(path);
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

		auto it = std::lower_bound(freeList.begin(), freeList.end(), handle.slot);
		freeList.insert(it, handle.slot);

		//pulizia cache
		auto cacheIt = std::find_if(cache.begin(), cache.end(),
			[&handle](const auto& pair) { return pair.second == handle.slot; });
		if (cacheIt != cache.end()) cache.erase(cacheIt);

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