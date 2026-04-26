# bvh.h

## Brief Overview

The `bvh.h` file is a header file that defines the Bounding Volume Hierarchy (BVH) data structure and associated functions. BVHs are used to optimize collision detection, ray tracing, and other spatial queries by organizing 3D objects into a hierarchical structure. This allows for efficient querying of nearby objects without having to check every single object.

## Table of Classes/Structs/Enums

### Classes/Structs
- `BVH`: Represents the Bounding Volume Hierarchy.
- `BVHNode`: Represents a node in the BVH hierarchy.
- `BVHTreeBuilder`: Builds BVH trees from sets of objects.

### Enums
- `BVHType`: Defines the type of bounding volume used in the BVH (e.g., Axis-Aligned Bounding Box, Oriented Bounding Box).
- `BVHBuildMode`: Specifies the mode for building the BVH tree (e.g., bottom-up, top-down).

## Detailed Methods Section

### Method: `BVH::BVH(std::vector<Object*>& objects)`

**Full Signature:**
```cpp
BVH::BVH(std::vector<Object*>& objects);
```

**Return Type Explanation:**
- This method initializes a new BVH instance.

**Internal Logic/Purpose:**
- The constructor takes a vector of `Object*` pointers and constructs the BVH by building the BVH tree. It uses a specified build mode to organize the objects into a hierarchical structure, optimizing spatial queries.

### Method: `BVHNode::BVHNode(Object* object)`

**Full Signature:**
```cpp
BVHNode::BVHNode(Object* object);
```

**Return Type Explanation:**
- This method initializes a new BVH node with a single object.

**Internal Logic/Purpose:**
- The constructor creates a new BVH node and assigns it the given object. It also calculates the bounding volume for the object, which will be used in spatial queries.

### Method: `BVHTreeBuilder::BuildTree(std::vector<Object*>& objects, BVHType type, BVHBuildMode mode)`

**Full Signature:**
```cpp
BVHTreeBuilder::BuildTree(std::vector<Object*>& objects, BVHType type, BVHBuildMode mode);
```

**Return Type Explanation:**
- This method builds a BVH tree from the given set of objects using the specified bounding volume type and build mode.

**Internal Logic/Purpose:**
- The `BuildTree` function organizes the objects into a BVH structure based on the provided parameters. It supports different bounding volumes (e.g., AABB, OBB) and build modes (e.g., bottom-up, top-down). The function returns the root node of the constructed BVH tree.

### Method: `BVHTreeBuilder::CalculateBoundingBox(std::vector<Object*>& objects, BVHType type)`

**Full Signature:**
```cpp
BVHTreeBuilder::CalculateBoundingBox(std::vector<Object*>& objects, BVHType type);
```

**Return Type Explanation:**
- This method calculates the bounding volume for a set of objects.

**Internal Logic/Purpose:**
- The `CalculateBoundingBox` function computes the bounding box or other specified bounding volume that encapsulates all the given objects. It supports different types of bounding volumes as specified by the `BVHType`.

### Method: `BVH::Intersect(Ray& ray)`

**Full Signature:**
```cpp
bool BVH::Intersect(Ray& ray);
```

**Return Type Explanation:**
- This method checks if a ray intersects with any object in the BVH.

**Internal Logic/Purpose:**
- The `Intersect` function traverses the BVH to efficiently check if the given ray intersects with any objects. It uses spatial partitioning to reduce the number of intersections that need to be checked, improving performance.

### Method: `BVHNode::ContainsPoint(Vector3& point)`

**Full Signature:**
```cpp
bool BVHNode::ContainsPoint(Vector3& point);
```

**Return Type Explanation:**
- This method checks if a point is contained within the bounding volume of the BVH node.

**Internal Logic/Purpose:**
- The `ContainsPoint` function determines whether the given point lies within the bounding volume of the node. It uses this to quickly eliminate nodes that do not contain the point, optimizing queries.