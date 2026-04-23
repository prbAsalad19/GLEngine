# RenderObject.h

## Brief Overview

This header file defines the `RenderObject` struct, which represents a basic rendering entity within a scene. It includes properties such as a mesh, a material (with support for handling cases where no material is applied), and a transformation matrix to position and orient the object in space.

## Table of Classes/Structs/Enums

- **RenderObject**: Represents a rendering entity with a mesh, material, and transform.

## Detailed Methods Section

### RenderObject Struct

**Full Signature:**
```cpp
struct RenderObject
{
    MeshHandle mesh;
    MaterialHandle material; // -1 for no texture
    Transform transform;
};
```

**Return Type Explanation:**
- **MeshHandle**: A handle to the mesh that defines the geometric shape of the `RenderObject`.
- **MaterialHandle**: A handle to the material that determines how the object is rendered, including its color and texture. The value `-1` indicates that no texture should be applied.
- **Transform**: A structure representing the transformation matrix for the `RenderObject`, which includes position, rotation, and scale.

**Internal Logic/Purpose:**
The `RenderObject` struct serves as a fundamental building block for rendering in the scene. It encapsulates all necessary information about an object's appearance and spatial properties:
- **Mesh**: Defines the geometry of the object.
- **Material**: Controls how the mesh is rendered, including its visual characteristics.
- **Transform**: Allows the object to be positioned, rotated, and scaled within the scene.

This struct provides a simple yet powerful way to manage rendering entities in the scene, facilitating efficient rendering and easy manipulation of objects.