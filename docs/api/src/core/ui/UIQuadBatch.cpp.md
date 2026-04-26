### UIQuadBatch.cpp

#### Brief Overview
The `UIQuadBatch` class is responsible for batching and managing UI elements for rendering. It handles the creation of vertex data (`vertices`) and index data (`indices`) based on the geometries of UI elements, including simple shapes and labels. The class also provides methods to clear the batch and build it from a canvas.

#### Table of Classes/Structs/Enums
- **UIQuadBatch**: Manages the batching of UI elements for rendering.
- **UIGeometryMode**: Enumerates different geometry modes (e.g., Free, Simple).
- **UIElement**: Represents a UI element with properties like geometry and color.
- **FontAtlas**: Provides glyph metrics and UV coordinates for text rendering.
- **GlyphMetrics**: Contains metrics for a single glyph.

#### Detailed Methods Section

1. **Constructor (`UIQuadBatch`)**:
   - **Full Signature**:
     ```cpp
     UIQuadBatch::UIQuadBatch()
     ```
   - **Return Type Explanation**:
     None (default constructor).
   - **Internal Logic/Purpose**:
     Initializes a new instance of `UIQuadBatch`.

2. **Destructor (`~UIQuadBatch`)**:
   - **Full Signature**:
     ```cpp
     UIQuadBatch::~UIQuadBatch()
     ```
   - **Return Type Explanation**:
     None (destructor).
   - **Internal Logic/Purpose**:
     Cleans up any resources held by the instance.

3. **Clear Method (`clear`)**:
   - **Full Signature**:
     ```cpp
     void UIQuadBatch::clear()
     ```
   - **Return Type Explanation**:
     `void`.
   - **Internal Logic/Purpose**:
     Clears all vertices and indices, effectively resetting the batch.

4. **Push Method for Free Geometry (`push`)**:
   - **Full Signature**:
     ```cpp
     void UIQuadBatch::push(const UIElement& element)
     ```
   - **Return Type Explanation**:
     `void`.
   - **Internal Logic/Purpose**:
     Adds a quadrilateral to the batch based on the provided `UIElement`. The method handles both free-form and simple geometries, including rotation for simple shapes.

5. **Push Method for Labels (`pushLabel`)**:
   - **Full Signature**:
     ```cpp
     void UIQuadBatch::pushLabel(const UIElement& element,
         const FontAtlas& atlas,
         const std::string& resolvedText)
     ```
   - **Return Type Explanation**:
     `void`.
   - **Internal Logic/Purpose**:
     Adds a label to the batch by rendering each character as a quadrilateral. It handles text alignment and rotation.

6. **Build From Canvas Method (`buildFromCanvas`)**:
   - **Full Signature**:
     ```cpp
     void UIQuadBatch::buildFromCanvas(const UICanvas& canvas, const FontAtlas& atlas)
     ```
   - **Return Type Explanation**:
     `void`.
   - **Internal Logic/Purpose**:
     Clears the existing batch and rebuilds it from a given `UICanvas`. It iterates through each element in the canvas, adding them to the batch using either the `push` or `pushLabel` method.