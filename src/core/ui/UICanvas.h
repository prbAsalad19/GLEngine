#pragma once
#include "core/CoreConfig.h"
#include "UIElement.h"
#include "json/json.hpp"

using json = nlohmann::json;

class UICanvas {
public:
    UICanvas();
    ~UICanvas();

	void bindFloat(const std::string& key, std::function<float()> fn);
    void bindString(const std::string& key, std::function<std::string()> fn);
    void bindFloat(const std::string& key, float* ptr);
    void bindString(const std::string& key, std::string* ptr);
    float       getValue(const std::string& elementId) const;
    std::string getString(const std::string& elementId) const;

	void loadUI(const std::string& filePath);

private:
	std::vector<UIElement> elements;

    UIElementType parseType(const std::string& typeStr);
    void parseColor(const std::string& hex, float color[4]);
    void resolveBindings();

    std::unordered_map<std::string, std::function<float()>>       numericRegistry;
    std::unordered_map<std::string, std::function<std::string()>> stringRegistry;

    std::unordered_map<std::string, std::function<float()>>       activeNumeric;
    std::unordered_map<std::string, std::function<std::string()>> activeString;
};
