#pragma once
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"

enum class UIElementType : uint8_t
{
	Label,
	ProgressBar,
	Panel,
	Icon
};

enum class UIGeometryMode : uint8_t
{
	Simple,  // position + size + rotation
	Free     // 4 punti espliciti
};

struct UIGeometry
{
    UIGeometryMode mode;
    union
    {
        struct { Vector2 position; Vector2 size; float rotation; } simple;
        struct { Vector2 points[4]; } free;
    };
};

struct UIElement
{
    std::string   id;
    UIElementType type;
    float         color[4];
    std::string   valueKey;
    UIGeometry    geometry;

    // Used by Label elements.
    // For dynamic text, leave this empty and use valueKey + bindString().
    std::string   text;
    float         fontSize = 0.0f;  // 0 = use atlas default size
};