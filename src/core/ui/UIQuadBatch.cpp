#include "UIQuadBatch.h"


UIQuadBatch::UIQuadBatch()
{
}

UIQuadBatch::~UIQuadBatch()
{
}

void UIQuadBatch::clear()
{
	vertices.clear();
	indices.clear();
}

void UIQuadBatch::push(const UIElement& element)
{
    if (element.geometry.mode == UIGeometryMode::Free)
    {
        UIVertex v0, v1, v2, v3;

        v0.position = element.geometry.free.points[0];
        v1.position = element.geometry.free.points[1];
        v2.position = element.geometry.free.points[2];
        v3.position = element.geometry.free.points[3];

        v0.uv = { 0.0f, 0.0f };
        v1.uv = { 1.0f, 0.0f };
        v2.uv = { 1.0f, 1.0f };
        v3.uv = { 0.0f, 1.0f };

        for (int i = 0; i < 4; ++i)
            v0.color[i] = v1.color[i] = v2.color[i] = v3.color[i] = element.color[i];

        unsigned int base = static_cast<unsigned int>(vertices.size());
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v3);

        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        return;
    }

    float x = element.geometry.simple.position.x;
    float y = element.geometry.simple.position.y;
    float w = element.geometry.simple.size.x;
    float h = element.geometry.simple.size.y;
    float r = element.geometry.simple.rotation * PI / 180.0f;

    float cr = cosf(r), sr = sinf(r);

    UIVertex v0, v1, v2, v3;

    // posizioni — applichi la rotazione attorno all'origine
    v0.position = { x,                                y };
    v1.position = { x + cr * w,                         y + sr * w };
    v2.position = { x + cr * w - sr * h,                  y + sr * w + cr * h };
    v3.position = { x - sr * h,                  y + cr * h };

    // uv
    v0.uv = { 0.0f, 0.0f };
    v1.uv = { 1.0f, 0.0f };
    v2.uv = { 1.0f, 1.0f };
    v3.uv = { 0.0f, 1.0f };

    // colore — uguale per tutti e 4
    for (int i = 0; i < 4; ++i)
        v0.color[i] = v1.color[i] = v2.color[i] = v3.color[i] = element.color[i];

    // aggiungi i vertici
    unsigned int base = static_cast<unsigned int>(vertices.size());
    vertices.push_back(v0);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    // aggiungi gli indici — due triangoli CCW
    indices.push_back(base + 0);
    indices.push_back(base + 1);
    indices.push_back(base + 2);
    indices.push_back(base + 0);
    indices.push_back(base + 2);
    indices.push_back(base + 3);
}

void UIQuadBatch::pushLabel(const UIElement& element,
    const FontAtlas& atlas,
    const std::string& resolvedText)
{
    //std::cout << "[pushLabel] text='" << resolvedText
    //    << "' pos=" << element.geometry.simple.position.x
    //    << "," << element.geometry.simple.position.y << "\n";
    if (resolvedText.empty()) return;

    float cursorX = element.geometry.simple.position.x;
    float cursorY = element.geometry.simple.position.y;

    // baseline = top of the label box + ascent approximated as pixelHeight * 0.8
    // stb_truetype bearingY is already relative to baseline.
    float baseline = cursorY + atlas.getPixelHeight() * 0.8f;

    for (char c : resolvedText)
    {
        if (c == '\n')
        {
            cursorX = element.geometry.simple.position.x;
            baseline += atlas.getLineHeight();
            continue;
        }

        const GlyphMetrics& g = atlas.getGlyph(c);
        if (g.width == 0.0f || g.height == 0.0f)
        {
            cursorX += g.advance;
            continue;
        }

        // Pixel-space quad corners
        float x0 = cursorX + g.bearingX;
        float y0 = baseline + g.bearingY;          // bearingY is negative above baseline
        float x1 = x0 + g.width;
        float y1 = y0 + g.height;

        unsigned int baseIndex = static_cast<unsigned int>(vertices.size());

        // 4 vertices — color carries the text tint (RGBA)
        auto makeVert = [&](float px, float py, float u, float v) -> UIVertex
            {
                UIVertex vert;
                vert.position = { px, py };
                vert.uv = { u, v };
                vert.color[0] = element.color[0];
                vert.color[1] = element.color[1];
                vert.color[2] = element.color[2];
                vert.color[3] = element.color[3];
                return vert;
            };

        vertices.push_back(makeVert(x0, y0, g.uvX0, g.uvY0));  // top-left
        vertices.push_back(makeVert(x1, y0, g.uvX1, g.uvY0));  // top-right
        vertices.push_back(makeVert(x1, y1, g.uvX1, g.uvY1));  // bottom-right
        vertices.push_back(makeVert(x0, y1, g.uvX0, g.uvY1));  // bottom-left

        indices.push_back(baseIndex + 0);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);
        indices.push_back(baseIndex + 0);

        cursorX += g.advance;
    }
}

void UIQuadBatch::buildFromCanvas(const UICanvas& canvas, const FontAtlas& atlas)
{
    clear();
    for (const UIElement& el : canvas.getElements())
    {
        if (el.type == UIElementType::Label)
        {
            // Prefer dynamic string binding; fall back to static text field.
            std::string displayText = canvas.getString(el.id);
            if (displayText.empty())
                displayText = el.text;
            pushLabel(el, atlas, displayText);
        }
        else
        {
            push(el);
        }
    }
}