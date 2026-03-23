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

    void bindButtonFunction(const std::string& id, std::function<void()> func) { buttonFunctionRegistry[id] = func; }
    void onButtonClick(const std::string& id) {
        auto it = buttonFunctionRegistry.find(id);
        if (it != buttonFunctionRegistry.end()) {
            it->second(); 
        }
    }

    float       getValue(const std::string& elementId) const;
    std::string getString(const std::string& elementId) const;
    const std::vector<UIElement>& getElements() const { return elements; }
    bool hasClickableButtons() { return hasButtons; }
    void processClickAt(double xpos, double ypos);

	void loadUI(const std::string& filePath);

private:
    bool hasButtons = false;

	std::vector<UIElement> elements;

    UIElementType parseType(const std::string& typeStr);
    void parseColor(const std::string& hex, float color[4]);
    void resolveBindings();

    std::vector<std::string> buttonsList;

    std::unordered_map<std::string, std::function<float()>>       numericRegistry;
    std::unordered_map<std::string, std::function<std::string()>> stringRegistry;
    std::unordered_map<std::string, std::function<void()>> buttonFunctionRegistry;

    std::unordered_map<std::string, std::function<float()>>       activeNumeric;
    std::unordered_map<std::string, std::function<std::string()>> activeString;
};
