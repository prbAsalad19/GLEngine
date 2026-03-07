#pragma once
#include <cstdint>

struct MeshTag {};
struct TextureTag {};
struct MaterialTag {};

template<typename Tag>
struct ResourceHandle
{
	uint32_t slot;
	uint32_t generation;

	ResourceHandle() : slot(UINT32_MAX), generation(0) {}
	ResourceHandle(uint32_t s, uint32_t g) : slot(s), generation(g) {}

	bool isValid() const { return slot != UINT32_MAX; }

	bool operator==(const ResourceHandle& other) const
	{
		return slot == other.slot && generation == other.generation;
	}
	bool operator!=(const ResourceHandle& other) const { return !(*this == other); }
};

using MeshHandle = ResourceHandle<MeshTag>;
using TextureHandle = ResourceHandle<TextureTag>;
using MaterialHandle = ResourceHandle<MaterialTag>;