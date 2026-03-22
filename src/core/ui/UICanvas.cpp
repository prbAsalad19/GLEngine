#include "UICanvas.h"

UICanvas::UICanvas() 
{
}

UICanvas::~UICanvas() 
{
}

void UICanvas::bindFloat(const std::string& key, std::function<float()> fn) 
{
	numericRegistry[key] = fn;
	resolveBindings();
}

void UICanvas::bindFloat(const std::string& key, float* ptr)
{
	numericRegistry[key] = [ptr]() { return *ptr; };
	resolveBindings();
}

void UICanvas::bindString(const std::string& key, std::function<std::string()> fn) 
{
	stringRegistry[key] = fn;
}

void UICanvas::bindString(const std::string& key, std::string* ptr)
{
	stringRegistry[key] = [ptr]() { return *ptr; };
	resolveBindings();
}

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
}

void UICanvas::parseColor(const std::string& hex, float color[4])
{
	// rimuove il # iniziale
	std::string h = hex.substr(1);

	color[0] = std::stoi(h.substr(0, 2), nullptr, 16) / 255.0f; // R
	color[1] = std::stoi(h.substr(2, 2), nullptr, 16) / 255.0f; // G
	color[2] = std::stoi(h.substr(4, 2), nullptr, 16) / 255.0f; // B
	color[3] = std::stoi(h.substr(6, 2), nullptr, 16) / 255.0f; // A
}

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
