#pragma once
#include <cstdint>
#include <functional>

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

	bool isNull() const { return slot != UINT32_MAX; }

	bool operator==(const ResourceHandle& other) const
	{
		return slot == other.slot && generation == other.generation;
	}
	bool operator!=(const ResourceHandle& other) const { return !(*this == other); }
};

using MeshHandle = ResourceHandle<MeshTag>;
using TextureHandle = ResourceHandle<TextureTag>;
using MaterialHandle = ResourceHandle<MaterialTag>;

namespace std {
	template<typename Tag>
	struct hash<ResourceHandle<Tag>>
	{
		size_t operator()(const ResourceHandle<Tag>& handle) const noexcept
		{
			// Combina slot e generation per creare un hash unico
			return std::hash<uint32_t>()(handle.slot) ^ (std::hash<uint32_t>()(handle.generation) << 1);
		}
	};
}
