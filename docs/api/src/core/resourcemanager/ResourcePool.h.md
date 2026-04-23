# ResourcePool.h

## Brief Overview
The `ResourcePool` template class provides a pool-based management system for managing resources of type `T`, associated with tags of type `Tag`. It efficiently handles the allocation and deallocation of resources, ensuring that resources are reused and minimizing memory allocations.

## Table of Classes/Structs/Enums
- **ResourcePool<Tag, T>**: Main template class for managing a pool of resources.
- **Slot**: Internal struct representing a slot in the resource pool.

## Detailed Methods Section

### Method: `explicit ResourcePool(uint32_t capacity = 64)`

#### Full Signature
```cpp
explicit ResourcePool(uint32_t capacity = 64)
```

#### Return Type Explanation
- **None (Constructor)**

#### Internal Logic/Purpose
Initializes the resource pool with a specified capacity. The constructor sets up the `slots` vector to hold the resource slots, the `freeList` to manage free slot indices, and the `cache` to store the mapping from resource paths to their corresponding slot indices.

### Method: `ResourceHandle<Tag> insert(const std::string& path, std::unique_ptr<T> resource)`

#### Full Signature
```cpp
ResourceHandle<Tag> insert(const std::string& path, std::unique_ptr<T> resource)
```

#### Return Type Explanation
- **ResourceHandle<Tag>**: A handle to the inserted resource.

#### Internal Logic/Purpose
Inserts a new resource into the pool. If a resource with the same path already exists in the cache, it returns a handle to that resource. Otherwise, it allocates a free slot from the `freeList`, moves the provided resource into the slot, marks the slot as active, updates the cache, and returns a handle to the new or existing resource.

### Method: `T* get(ResourceHandle<Tag> handle)`

#### Full Signature
```cpp
T* get(ResourceHandle<Tag> handle)
```

#### Return Type Explanation
- **T***: Pointer to the resource associated with the handle. Returns `nullptr` if the handle is invalid.

#### Internal Logic/Purpose
Retrieves a pointer to the resource associated with the given handle. It checks if the handle is valid by calling the `isValid` method. If valid, it returns a pointer to the resource in the corresponding slot; otherwise, it returns `nullptr`.

### Method: `bool remove(ResourceHandle<Tag> handle)`

#### Full Signature
```cpp
bool remove(ResourceHandle<Tag> handle)
```

#### Return Type Explanation
- **bool**: True if the resource was successfully removed, false otherwise.

#### Internal Logic/Purpose
Removes a resource from the pool using the provided handle. It checks if the handle is valid by calling the `isValid` method. If valid, it marks the slot as inactive, resets the resource pointer, increments the generation number to invalidate handles, adds the slot back to the `freeList`, and removes the entry from the cache. Returns true on successful removal.

### Method: `private bool isValid(ResourceHandle<Tag> handle) const`

#### Full Signature
```cpp
private bool isValid(ResourceHandle<Tag> handle) const
```

#### Return Type Explanation
- **bool**: True if the handle is valid, false otherwise.

#### Internal Logic/Purpose
Checks if a handle is valid by ensuring that the slot index is within bounds, the slot is marked as active, and the generation number matches the handle's generation number.