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

void UIQuadBatch::buildFromCanvas(const UICanvas& canvas)
{
    clear();
    for (const auto& el : canvas.getElements())
    {
        push(el);
    }
    //std::cout << "[UIQuadBatch] Built " << vertices.size() << " vertices, "
    //    << indices.size() << " indices\n";
}