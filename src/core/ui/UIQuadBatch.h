#pragma once
#include "core/CoreConfig.h"
#include "UIElement.h"
#include "UICanvas.h"

struct UIVertex
{
	Vector2 position;
	Vector2 uv;
	float   color[4];
};

class UIQuadBatch {
public:
	UIQuadBatch();
	~UIQuadBatch();
	void push(const UIElement& element);
	void buildFromCanvas(const UICanvas& canvas);
	void clear();

	// Getters
	const std::vector<UIVertex>& getVertices() const { return vertices; }
	const std::vector<unsigned int>& getIndices() const { return indices; }
private:
	std::vector<UIVertex> vertices; // x, y, u, v, r, g, b, a
	std::vector<unsigned int> indices; // 6 per quad
};
