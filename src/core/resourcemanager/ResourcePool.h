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

	std::vector<Slot> slots;
	std::vector<uint32_t> freeList;
	std::unordered_map<std::string, uint32_t> cache;

public:
	template<typename Func>
	void forEach(Func&& fn)
	{
		for (auto& slot : slots)
		{
			if (slot.active)
				fn(*slot.resource);
		}
	}

	uint32_t size() const { return slots.size() - freeList.size(); }

	explicit ResourcePool(uint32_t capacity = 64)
	{
		slots.resize(capacity);
		freeList.reserve(capacity);
		for (uint32_t i = 0; i < capacity; ++i)
			freeList.push_back(i);
	} //making the free list a non null vector

	ResourceHandle<Tag> insert(const std::string& path, std::unique_ptr<T> resource)
	{
	    auto it = cache.find(path);
	    if (it != cache.end())
	    {
	        uint32_t s = it->second;
	        return ResourceHandle<Tag>(s, slots[s].generation);
	    }
	
	    uint32_t s = freeList.back();   // O(1)
	    freeList.pop_back();            // O(1)
	
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
	
	    freeList.push_back(handle.slot);  // O(1), ordine non importante
	
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
