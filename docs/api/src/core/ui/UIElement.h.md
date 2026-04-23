# UIElement.h

## Brief Overview
The `UIElement.h` file contains definitions for the core structures and enums used in a user interface (UI) system. It includes definitions for different types of UI elements, their geometries, and properties.

## Table of Classes/Structs/Enums

### Enums
1. **UIElementType**
   - Enumerates different types of UI elements:
     - `Label`
     - `ProgressBar`
     - `Panel`
     - `Icon`

2. **UIGeometryMode**
   - Defines the geometry mode for UI elements:
     - `Simple`: Position, size, and rotation.
     - `Free`: Four explicit points.

### Structs
1. **UIGeometry**
   - Represents the geometric properties of a UI element.
   - Contains a mode (`mode`) that determines whether to use a simple or free geometry.
   - Uses a union to hold either a simple structure (position, size, rotation) or a free structure (four points).

2. **UIElement**
   - Represents a basic UI element.
   - Includes properties such as:
     - `id`: Unique identifier for the element.
     - `type`: Type of the UI element.
     - `color`: RGBA color of the element.
     - `valueKey`: Key to bind values to this element.
     - `geometry`: Geometric properties of the element.
   - Additional properties specific to certain types of elements:
     - `text`: For label elements, dynamic text content (if not using `valueKey`).
     - `fontSize`: Font size for label elements (0 indicates use of default atlas size).

## Detailed Methods Section

### Full Signature
```cpp
enum class UIElementType : uint8_t { Label, ProgressBar, Panel, Icon };
```

**Return Type Explanation:**
- This is an enum definition. Enums do not have a return type in the traditional sense; they are used to define a set of named constants.

**Internal Logic/Purpose:**
- Defines four types of UI elements (`Label`, `ProgressBar`, `Panel`, and `Icon`). Each type can be represented as a constant within this enum, facilitating type checks and comparisons.

### Full Signature
```cpp
enum class UIGeometryMode : uint8_t { Simple, Free };
```

**Return Type Explanation:**
- Similar to the UIElementType enum, this defines two modes for geometry (`Simple` and `Free`) using an enumeration. Each mode is represented as a constant within the enum.

**Internal Logic/Purpose:**
- Specifies two modes for defining the geometric properties of UI elements (`Simple`, which uses position, size, and rotation; and `Free`, which uses four explicit points).

### Full Signature
```cpp
struct UIGeometry { ... };
```

**Return Type Explanation:**
- This is a struct definition. Structs in C++ are used to group related data together.

**Internal Logic/Purpose:**
- Represents the geometric properties of a UI element.
  - `mode`: Determines whether to use a simple or free geometry.
  - `union`: Holds either a simple structure (position, size, rotation) or a free structure (four points). This allows for flexible geometry definitions based on the chosen mode.

### Full Signature
```cpp
struct UIElement { ... };
```

**Return Type Explanation:**
- This is another struct definition. Structs are used to group related data together.

**Internal Logic/Purpose:**
- Represents a basic UI element.
  - `id`: Unique identifier for the element, facilitating easy referencing and management in the UI system.
  - `type`: Specifies the type of the UI element (`Label`, `ProgressBar`, `Panel`, or `Icon`), allowing for type-specific handling and behavior.
  - `color`: RGBA color value defining the appearance of the element.
  - `valueKey`: Key to bind values to this element, useful for dynamic updates without modifying the element's properties directly.
  - `geometry`: Geometric properties of the element, using the `UIGeometry` struct.
  - Additional properties specific to certain types of elements:
    - `text`: For label elements, dynamic text content (if not using `valueKey`).
    - `fontSize`: Font size for label elements (0 indicates use of default atlas size).

This reference provides a clear understanding of the structure and purpose of each element in the UI system, ensuring consistent implementation and maintenance across different parts of the application.