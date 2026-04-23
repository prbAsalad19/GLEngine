# BVHTree.cpp

## Brief Overview
The `BVHTree` class is designed to manage and build a Bounding Volume Hierarchy (BVH) tree for efficient spatial partitioning in rendering applications. The BVH is particularly useful for speeding up ray tracing, collision detection, and other space-related operations.

## Table of Classes/Structs/Enums
- **Class**: `BVHTree`
  - **Description**: Manages the construction and querying of a BVH tree.
- **Struct**: `AABB` (Bounding Box)
  - **Description**: Represents an axis-aligned bounding box, used to encapsulate objects in the BVH.

## Detailed Methods Section

### Method: `computeAABB`

**Full Signature**:
```cpp
AABB BVHTree::computeAABB(uint32_t first, uint32_t count, const std::vector<RenderObject>& objects)
```

**Return Type Explanation**:
- **Type**: `AABB`
  - **Description**: An axis-aligned bounding box that encloses a range of render objects.

**Internal Logic/Purpose**:
The `computeAABB` method calculates the axis-aligned bounding box (AABB) for a given range of render objects. It iterates through the specified number of objects, starting from the index `first`, and merges their individual AABBs into a single encompassing AABB. This is achieved by using the `m_resources.getMeshAABB` function to retrieve the AABB of each object's mesh and then merging these AABBs using the `AABB::merge` method.

### Example Usage
Here's how you might use the `computeAABB` method in your code:

```cpp
BVHTree bvh;
std::vector<RenderObject> objects; // Assume this is populated with render objects
uint32_t startIndex = 0;
uint32_t objectCount = 10;

AABB treeAABB = bvh.computeAABB(startIndex, objectCount, objects);
```

In this example:
- `startIndex` specifies the starting index of the range of objects.
- `objectCount` specifies the number of objects in the range.
- The resulting `treeAABB` will encompass all the specified render objects.

This method is essential for initializing the BVH with the correct bounding box, which can then be used to optimize spatial queries.