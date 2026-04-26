### UIQuadBatch.h

#### Brief Overview

`UIQuadBatch` is a class designed to efficiently manage and render batches of 2D quads for user interface elements in a graphical application. It optimizes rendering by reducing the number of draw calls, which can significantly improve performance on lower-end hardware.

#### Table of Classes/Structs/Enums

- **Struct UIVertex**: Represents a single vertex with position, UV coordinates, and color.
- **Class UIQuadBatch**: Manages batches of quads to optimize rendering in the user interface.

#### Detailed Methods Section

**1. Full Signature:**
```cpp
UIQuadBatch()
```

**2. Return Type Explanation:**
N/A (Constructor)

**3. Internal Logic/Purpose:**
Initializes a new instance of `UIQuadBatch`. Sets up any internal data structures or resources required for managing quad batches.

---

**1. Full Signature:**
```cpp
~UIQuadBatch()
```

**2. Return Type Explanation:**
N/A (Destructor)

**3. Internal Logic/Purpose:**
Releases any resources held by the `UIQuadBatch` instance, such as memory allocated for vertices and indices.

---

**1. Full Signature:**
```cpp
void push(const UIElement& element)
```

**2. Return Type Explanation:**
N/A (Void)

**3. Internal Logic/Purpose:**
Adds a single `UIElement` to the batch. The method calculates the necessary vertices and indices for the quad representing the element.

---

**1. Full Signature:**
```cpp
void pushLabel(const UIElement& element, const FontAtlas& atlas, const std::string& resolvedText)
```

**2. Return Type Explanation:**
N/A (Void)

**3. Internal Logic/Purpose:**
Adds a `UIElement` that represents text to the batch. It calculates the vertices and indices for the quad representing the text, taking into account the font atlas and the resolved text content.

---

**1. Full Signature:**
```cpp
void buildFromCanvas(const UICanvas& canvas, const FontAtlas& atlas)
```

**2. Return Type Explanation:**
N/A (Void)

**3. Internal Logic/Purpose:**
Builds the quad batch from a `UICanvas` object and a `FontAtlas`. It iterates through all UI elements in the canvas, adding them to the batch appropriately.

---

**1. Full Signature:**
```cpp
void clear()
```

**2. Return Type Explanation:**
N/A (Void)

**3. Internal Logic/Purpose:**
Clears all vertices and indices from the batch, preparing it for a new set of data.

---

**1. Full Signature:**
```cpp
const std::vector<UIVertex>& getVertices() const
```

**2. Return Type Explanation:**
`const std::vector<UIVertex>&`: A reference to the vector containing all vertices in the batch.

**3. Internal Logic/Purpose:**
Provides read-only access to the vector of vertices, which can be used by other parts of the application for rendering or other purposes.

---

**1. Full Signature:**
```cpp
const std::vector<unsigned int>& getIndices() const
```

**2. Return Type Explanation:**
`const std::vector<unsigned int>&`: A reference to the vector containing all indices in the batch.

**3. Internal Logic/Purpose:**
Provides read-only access to the vector of indices, which can be used by other parts of the application for rendering or other purposes.