# ResourceHandle.h

## Brief Overview
This header file defines a generic `ResourceHandle` template struct used for managing references to resources in a resource management system. It includes specializations for different types of resources such as Mesh, Texture, and Material.

## Table of Classes/Structs/Enums
- **MeshTag**: A tag type for mesh resources.
- **TextureTag**: A tag type for texture resources.
- **MaterialTag**: A tag type for material resources.
- **ResourceHandle<Tag>**: A template struct representing a handle to a resource, uniquely identified by a slot and generation number.

## Detailed Methods Section

### ResourceHandle
```cpp
template<typename Tag>
struct ResourceHandle
{
	uint32_t slot;
	uint32_t generation;

	ResourceHandle() : slot(UINT32_MAX), generation(0) {}
	ResourceHandle(uint32_t s, uint32_t g) : slot(s), generation(g) {}

	bool isNull() const { return slot == UINT32_MAX; }

	bool operator==(const ResourceHandle& other) const
	{
		return slot == other.slot && generation == other.generation;
	}
	bool operator!=(const ResourceHandle& other) const { return !(*this == other); }
};
```

- **Full Signature**: `template<typename Tag> struct ResourceHandle`
- **Return Type Explanation**:
  - `uint32_t slot`: Represents the unique identifier for the resource within a slot.
  - `uint32_t generation`: Represents the version or generation of the resource, useful for detecting changes.
- **Internal Logic/Purpose**:
  - The constructor initializes the handle with a null state (slot set to `UINT32_MAX` and generation set to `0`).
  - The `isNull()` method checks if the handle is in a null state by verifying if the slot is equal to `UINT32_MAX`.
  - The equality and inequality operators (`==` and `!=`) compare two handles based on their slot and generation numbers, ensuring that only handles referring to the same resource version are considered equal.

### MeshHandle
```cpp
using MeshHandle = ResourceHandle<MeshTag>;
```

- **Full Signature**: `using MeshHandle = ResourceHandle<MeshTag>`
- **Return Type Explanation**:
  - This is a type alias for `ResourceHandle` specialized with the `MeshTag`. It represents a handle specifically used to manage mesh resources.
- **Internal Logic/Purpose**:
  - By using this alias, the code can easily create handles for mesh resources without needing to specify the `MeshTag` each time.

### TextureHandle
```cpp
using TextureHandle = ResourceHandle<TextureTag>;
```

- **Full Signature**: `using TextureHandle = ResourceHandle<TextureTag>`
- **Return Type Explanation**:
  - This is a type alias for `ResourceHandle` specialized with the `TextureTag`. It represents a handle specifically used to manage texture resources.
- **Internal Logic/Purpose**:
  - Similar to `MeshHandle`, this alias facilitates the creation of handles for texture resources in a concise manner.

### MaterialHandle
```cpp
using MaterialHandle = ResourceHandle<MaterialTag>;
```

- **Full Signature**: `using MaterialHandle = ResourceHandle<MaterialTag>`
- **Return Type Explanation**:
  - This is a type alias for `ResourceHandle` specialized with the `MaterialTag`. It represents a handle specifically used to manage material resources.
- **Internal Logic/Purpose**:
  - Like the previous aliases, this helps in managing handles for material resources efficiently.

### std::hash<ResourceHandle<Tag>>
```cpp
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
```

- **Full Signature**: `template<typename Tag> struct std::hash<ResourceHandle<Tag>>`
- **Return Type Explanation**:
  - This specialization of the `std::hash` function template provides a way to compute a hash value for `ResourceHandle` instances. It uses the slot and generation numbers of the handle to generate a unique hash.
- **Internal Logic/Purpose**:
  - The combination of the slot and generation in a specific manner (slot XOR shifted generation) ensures that each handle has a distinct hash, preventing collisions even when handles differ only slightly.

This header file provides a robust mechanism for managing resource handles in a generic, type-safe way, suitable for use in systems where resources need to be efficiently referenced and compared.