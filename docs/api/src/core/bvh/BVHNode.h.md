## BVHNode.h

### Brief Overview
This header file defines the structure of a node in a Bounding Volume Hierarchy (BVH). The BVH is used for efficient spatial queries, such as ray intersection testing, by recursively subdividing space into smaller volumes. Each node contains an axis-aligned bounding box (AABB) and information about its child nodes and objects.

### Table of Classes/Structs/Enums
- **BVHNode**: Represents a single node in the BVH structure.

### Detailed Methods Section

#### Method: `isLeaf() const`

**Full Signature**
```cpp
bool isLeaf() const;
```

**Return Type Explanation**
- **Type:** `bool`
- **Explanation:** Returns `true` if the node is a leaf (i.e., it does not have children), otherwise returns `false`.

**Internal Logic/Purpose**
The method checks if the current node is a leaf by examining the `leftChild` member. If `leftChild` is `-1`, it means there are no child nodes, and thus the node is a leaf. This determination is crucial for BVH traversal logic, as leaf nodes directly contain objects that need to be processed during spatial queries, while internal nodes further subdivide space into smaller bounding volumes.