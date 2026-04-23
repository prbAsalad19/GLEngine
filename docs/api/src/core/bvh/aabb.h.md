### 1. `aabb.h`

This header file defines the `AABB` (Axis-Aligned Bounding Box) class, which is a fundamental data structure used in spatial partitioning and collision detection algorithms. The AABB represents a box that encloses a set of points or other geometric objects without rotation.

---

### 2. Brief Overview

The `AABB` class provides methods to:
- Initialize an empty bounding box.
- Retrieve the center, minimum, and maximum points of the bounding box.
- Merge two AABBs to form a new one that encompasses both.
- Calculate the surface area of the bounding box.
- Expand the bounding box to include a new point.

---

### 3. Table of Classes/Structs/Enums

#### Class: `AABB`
Represents an axis-aligned bounding box with minimum and maximum points.

---

### 4. Detailed Methods Section

#### Method: `AABB()`

**Full Signature:** 
```cpp
AABB()
```

**Return Type Explanation:**
- **Type:** Constructor.
- **Explanation:** Initializes a new instance of the `AABB` class with both bounds set to represent an empty bounding box.

**Internal Logic/Purpose:**
- Sets the initial values of the bounding points to the maximum and minimum possible float values, respectively. This ensures that any point added later will automatically expand the bounding box.
  
---

#### Method: `center() const`

**Full Signature:** 
```cpp
Vector3 center() const
```

**Return Type Explanation:**
- **Type:** `Vector3`.
- **Explanation:** Returns the center point of the AABB.

**Internal Logic/Purpose:**
- Calculates and returns the midpoint between the minimum and maximum points using the `midpoint` function from the `algebricOp.h` header.

---

#### Method: `min() const`

**Full Signature:** 
```cpp
Vector3 min() const
```

**Return Type Explanation:**
- **Type:** `Vector3`.
- **Explanation:** Returns the minimum point of the AABB.

**Internal Logic/Purpose:**
- Directly returns the first element of the `bounds` array, which represents the minimum point.

---

#### Method: `max() const`

**Full Signature:** 
```cpp
Vector3 max() const
```

**Return Type Explanation:**
- **Type:** `Vector3`.
- **Explanation:** Returns the maximum point of the AABB.

**Internal Logic/Purpose:**
- Directly returns the second element of the `bounds` array, which represents the maximum point.

---

#### Method: `merge(const AABB& a, const AABB& b)`

**Full Signature:** 
```cpp
static AABB merge(const AABB& a, const AABB& b)
```

**Return Type Explanation:**
- **Type:** `AABB`.
- **Explanation:** Returns a new AABB that encompasses both input AABBs.

**Internal Logic/Purpose:**
- Creates a new AABB and sets its bounds to the minimum and maximum of the corresponding points from the two input AABBs.
- Uses the `min` and `max` functions from the `algebricOp.h` header to determine the new min and max points.

---

#### Method: `surfaceArea() const`

**Full Signature:** 
```cpp
float surfaceArea() const
```

**Return Type Explanation:**
- **Type:** `float`.
- **Explanation:** Returns the surface area of the AABB.

**Internal Logic/Purpose:**
- Calculates the dimensions of the bounding box by subtracting the minimum and maximum points.
- Uses the absolute values to ensure positive dimensions.
- Computes the surface area using the formula \(2 \times (d_0 \cdot d_1 + d_1 \cdot d_2 + d_2 \cdot d_0)\), where \(d_i\) are the side lengths of the bounding box.

---

#### Method: `expand(const Vector3& p)`

**Full Signature:** 
```cpp
void expand(const Vector3& p)
```

**Return Type Explanation:**
- **Type:** `void`.
- **Explanation:** Expands the AABB to include a new point.

**Internal Logic/Purpose:**
- Updates the bounds of the AABB to ensure that the new point \(p\) is within the bounding box.
- Uses the `min` and `max` functions from the `algebricOp.h` header to adjust the bounds accordingly.