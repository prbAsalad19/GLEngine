## UICanvas.h

### Brief Overview

`UICanvas` is a class designed to manage and interact with UI elements. It allows binding values to keys for dynamic data updates, loading UI configurations from JSON files, and retrieving element data. The class uses templates for generic data handling and provides methods to bind numeric and string values.

### Table of Classes/Structs/Enums

- **UICanvas**: Manages UI elements and bindings.
- **UIElement**: Represents a single UI component.
- **UIElementType**: Enumerates different types of UI elements (e.g., Button, Text).

### Detailed Methods Section

#### Method: `UICanvas`

**Full Signature**
```cpp
UICanvas()
```
**Return Type Explanation**
N/A - Constructor, no return value.

**Internal Logic/Purpose**
Initializes a new instance of the UICanvas class. Sets up any necessary internal state or variables for managing UI elements and bindings.

#### Method: `~UICanvas`

**Full Signature**
```cpp
~UICanvas()
```
**Return Type Explanation**
N/A - Destructor, no return value.

**Internal Logic/Purpose**
Cleans up resources associated with the UICanvas instance when it is destroyed. This includes releasing any dynamically allocated memory or closing file handles if applicable.

#### Method: `bindFloat`

**Full Signature**
```cpp
void bindFloat(const std::string& key, std::function<float()> fn)
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Binds a numeric function to a key. The provided function will be called when the value is requested, and its result will be associated with the specified key.

**Full Signature**
```cpp
void bindFloat(const std::string& key, float* ptr)
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Binds a numeric pointer to a key. The value at the provided pointer will be used as the current value for the specified key.

#### Method: `bindString`

**Full Signature**
```cpp
void bindString(const std::string& key, std::function<std::string()> fn)
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Binds a string function to a key. The provided function will be called when the value is requested, and its result will be associated with the specified key.

**Full Signature**
```cpp
void bindString(const std::string& key, std::string* ptr)
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Binds a string pointer to a key. The value at the provided pointer will be used as the current value for the specified key.

#### Method: `getValue`

**Full Signature**
```cpp
float getValue(const std::string& elementId) const
```
**Return Type Explanation**
`float` - The numeric value associated with the specified element ID, or 0.0 if not found.

**Internal Logic/Purpose**
Retrieves the current numeric value for a UI element identified by `elementId`. If the binding is to a function, it will call the function and return its result. If bound to a pointer, it returns the value at that pointer.

#### Method: `getString`

**Full Signature**
```cpp
std::string getString(const std::string& elementId) const
```
**Return Type Explanation**
`std::string` - The string value associated with the specified element ID, or an empty string if not found.

**Internal Logic/Purpose**
Retrieves the current string value for a UI element identified by `elementId`. If the binding is to a function, it will call the function and return its result. If bound to a pointer, it returns the value at that pointer.

#### Method: `getElements`

**Full Signature**
```cpp
const std::vector<UIElement>& getElements() const
```
**Return Type Explanation**
`const std::vector<UIElement>&` - A constant reference to the vector of UI elements managed by this canvas.

**Internal Logic/Purpose**
Provides access to the internal list of UI elements, allowing other parts of the application to query or modify them as needed.

#### Method: `loadUI`

**Full Signature**
```cpp
void loadUI(const std::string& filePath)
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Loads UI configurations from a specified JSON file. Parses the file and constructs the internal list of UI elements based on the data provided in the JSON format.

#### Method: `parseType`

**Full Signature**
```cpp
UIElementType parseType(const std::string& typeStr)
```
**Return Type Explanation**
`UIElementType` - The enumerated type corresponding to the specified string representation of a UI element type.

**Internal Logic/Purpose**
Converts a string representation of a UI element type (e.g., "Button", "Text") into its corresponding enumerated value. This is useful for parsing JSON configurations that specify element types.

#### Method: `parseColor`

**Full Signature**
```cpp
void parseColor(const std::string& hex, float color[4])
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Parses a hexadecimal string representing an RGBA color and stores the corresponding red, green, blue, and alpha values in an array. This is used to convert color specifications from JSON into usable numeric formats.

#### Method: `resolveBindings`

**Full Signature**
```cpp
void resolveBindings()
```
**Return Type Explanation**
N/A - Returns void, indicating no return value.

**Internal Logic/Purpose**
Resolves and applies any pending bindings (numeric or string) to the UI elements. This method ensures that all bound values are up-to-date with their source functions or pointers.