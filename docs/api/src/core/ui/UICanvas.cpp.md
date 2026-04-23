### UICanvas.cpp

#### Brief Overview
This source file defines the implementation of the `UICanvas` class, which is responsible for managing UI elements and resolving bindings based on data from a JSON file. The class provides methods to bind numeric and string values to UI elements, parse UI element types, load UI configurations from a file, and resolve these bindings.

#### Table of Classes/Structs/Enums
1. **UICanvas**
2. **UIElementType** (enum)
3. **UIGeometryMode** (enum)
4. **UIElement**
5. **NumericRegistry**
6. **StringRegistry**

### Detailed Methods Section

#### UICanvas::UICanvas()
- **Full Signature**: `UICanvas()`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Default constructor for the `UICanvas` class. Initializes any necessary data structures.

```cpp
UICanvas::UICanvas() 
{
}
```

#### UICanvas::~UICanvas()
- **Full Signature**: `~UICanvas()`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Destructor for the `UICanvas` class. Cleans up any allocated resources.

```cpp
UICanvas::~UICanvas() 
{
}
```

#### UICanvas::bindFloat(const std::string& key, std::function<float()> fn)
- **Full Signature**: `void bindFloat(const std::string& key, std::function<float()> fn)`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Binds a numeric value to a UI element using a function. Resolves bindings after updating the registry.

```cpp
void UICanvas::bindFloat(const std::string& key, std::function<float()> fn) 
{
	numericRegistry[key] = fn;
	resolveBindings();
}
```

#### UICanvas::bindFloat(const std::string& key, float* ptr)
- **Full Signature**: `void bindFloat(const std::string& key, float* ptr)`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Binds a numeric value to a UI element using a pointer. Resolves bindings after updating the registry.

```cpp
void UICanvas::bindFloat(const std::string& key, float* ptr)
{
	numericRegistry[key] = [ptr]() { return *ptr; };
	resolveBindings();
}
```

#### UICanvas::bindString(const std::string& key, std::function<std::string()> fn)
- **Full Signature**: `void bindString(const std::string& key, std::function<std::string()> fn)`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Binds a string value to a UI element using a function. Resolves bindings after updating the registry.

```cpp
void UICanvas::bindString(const std::string& key, std::function<std::string()> fn) 
{
	stringRegistry[key] = fn;
	resolveBindings();
}
```

#### UICanvas::bindString(const std::string& key, std::string* ptr)
- **Full Signature**: `void bindString(const std::string& key, std::string* ptr)`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Binds a string value to a UI element using a pointer. Resolves bindings after updating the registry.

```cpp
void UICanvas::bindString(const std::string& key, std::string* ptr)
{
	stringRegistry[key] = [ptr]() { return *ptr; };
	resolveBindings();
}
```

#### UICanvas::parseType(const std::string& typeStr)
- **Full Signature**: `UIElementType parseType(const std::string& typeStr)`
- **Return Type Explanation**: Returns the corresponding `UIElementType` based on the input string.
- **Internal Logic/Purpose**: Parses a string to determine the type of UI element.

```cpp
UIElementType UICanvas::parseType(const std::string& typeStr)
{
	if (typeStr == "label")
	{
		return UIElementType::Label;
	}
	if (typeStr == "progressbar") {
		return UIElementType::ProgressBar;
	}
	if (typeStr == "panel") {
		return UIElementType::Panel;
	}
	if (typeStr == "icon") {
		return UIElementType::Icon;
	}
	
	std::cout << "[UICanvas] Unknown element type: " << typeStr << "\n";
	return UIElementType::Label;
}
```

#### UICanvas::loadUI(const std::string& filepath)
- **Full Signature**: `void loadUI(const std::string& filepath)`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Loads and parses a JSON file to configure UI elements.

```cpp
void UICanvas::loadUI(const std::string& filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open()) {
		std::cout << "[UICanvas] Failed to open: " << filepath << "\n";
		return;
	}
	json data = json::parse(file);

	elements.clear();
		
	for (const auto& el : data["elements"])
	{
		UIElement element;

		element.id = el["id"].get<std::string>();
		element.type = parseType(el["type"].get<std::string>());

		std::string hex = el["color"].get<std::string>();
		parseColor(hex, element.color);

		if (el.contains("valueKey"))
		{
			element.valueKey = el["valueKey"].get<std::string>();
		}

		if (el.contains("text"))
			element.text = el["text"].get<std::string>();

		if (el.contains("fontSize"))
			element.fontSize = el["fontSize"].get<float>();


		if (el["geometry"].contains("points")) //free mode
		{
			element.geometry.mode = UIGeometryMode::Free;
			for (int i = 0; i < 4; ++i)
			{
				element.geometry.free.points[i].x = el["geometry"]["points"][i][0].get<float>();
				element.geometry.free.points[i].y = el["geometry"]["points"][i][1].get<float>();
			}
		}
		else //simple / 2 points mode
		{
			element.geometry.mode = UIGeometryMode::Simple;

			if (el["geometry"].contains("first") || el["geometry"].contains("last")) //2 point mode
			{
				element.geometry.simple.position.x = el["geometry"]["first"][0].get<float>();
				element.geometry.simple.position.y = el["geometry"]["first"][1].get<float>();

				element.geometry.simple.size.x = el["geometry"]["last"][0].get<float>() - element.geometry.simple.position.x;
				element.geometry.simple.size.y = el["geometry"]["last"][1].get<float>() - element.geometry.simple.position.y;

				element.geometry.simple.rotation = 0.0f;
			}
			else //simple mode
			{
				element.geometry.simple.position.x = el["geometry"]["position"][0].get<float>();
				element.geometry.simple.position.y = el["geometry"]["position"][1].get<float>();
				element.geometry.simple.size.x = el["geometry"]["size"][0].get<float>();
				element.geometry.simple.size.y = el["geometry"]["size"][1].get<float>();
				element.geometry.simple.rotation = el["geometry"].contains("rotation")
					? el["geometry"]["rotation"].get<float>()
					: 0.0f;
			}
		}

		elements.push_back(element);
	}
	resolveBindings();

	std::cout << "[UICanvas] Loaded " << elements.size() << " elements\n";
	for (const auto& el : elements)
	{
		std::cout << "  element: " << el.id
			<< " type: " << (int)el.type
			<< " color: " << el.color[0] << " " << el.color[1] << " " << el.color[2] << " " << el.color[3] << "\n";
		if (el.geometry.mode == UIGeometryMode::Simple)
			std::cout << "  pos: " << el.geometry.simple.position.x << " " << el.geometry.simple.position.y
			<< " size: " << el.geometry.simple.size.x << " " << el.geometry.simple.size.y << "\n";
	}
}
```

#### UICanvas::parseColor(const std::string& hex, float color[4])
- **Full Signature**: `void parseColor(const std::string& hex, float color[4])`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Parses a hexadecimal color string and converts it into RGBA values.

```cpp
void UICanvas::parseColor(const std::string& hex, float color[4])
{
	// rimuove il # iniziale
	std::string h = hex.substr(1);

	color[0] = std::stoi(h.substr(0, 2), nullptr, 16) / 255.0f; // R
	color[1] = std::stoi(h.substr(2, 2), nullptr, 16) / 255.0f; // G
	color[2] = std::stoi(h.substr(4, 2), nullptr, 16) / 255.0f; // B
	color[3] = std::stoi(h.substr(6, 2), nullptr, 16) / 255.0f; // A
}
```

#### UICanvas::resolveBindings()
- **Full Signature**: `void resolveBindings()`
- **Return Type Explanation**: None.
- **Internal Logic/Purpose**: Resolves bindings for numeric and string values based on the current data.

```cpp
void UICanvas::resolveBindings()
{
	activeNumeric.clear();
	activeString.clear();
	for (auto& el : elements)
	{
		if (!el.valueKey.empty())
		{
			if (numericRegistry.contains(el.valueKey))
			{
				activeNumeric[el.id] = numericRegistry[el.valueKey];
			}
			if (stringRegistry.contains(el.valueKey))
			{
				activeString[el.id] = stringRegistry[el.valueKey];
			}
		}
	}
}
```

#### UICanvas::getValue(const std::string& elementId) const
- **Full Signature**: `float getValue(const std::string& elementId) const`
- **Return Type Explanation**: Returns the numeric value for a given UI element ID.
- **Internal Logic/Purpose**: Retrieves the current value of a numeric bound to a UI element.

```cpp
float UICanvas::getValue(const std::string& elementId) const
{
	if (activeNumeric.contains(elementId))
	{
		return activeNumeric.at(elementId)();
	}
	return 0.0f;
}
```

#### UICanvas::getString(const std::string& elementId) const
- **Full Signature**: `std::string getString(const std::string& elementId) const`
- **Return Type Explanation**: Returns the string value for a given UI element ID.
- **Internal Logic/Purpose**: Retrieves the current value of a string bound to a UI element.

```cpp
std::string UICanvas::getString(const std::string& elementId) const
{
	if (activeString.contains(elementId))
	{
		return activeString.at(elementId)();
	}
	return "";
}
```