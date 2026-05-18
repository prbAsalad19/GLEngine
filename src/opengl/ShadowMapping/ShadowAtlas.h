#pragma once
#include "opengl/OpenGLConfig.h"
#include "core/math/algebricOp.h"
#include "opengl/ShadowMapping/ShadowCaster.h"
#include <memory>

constexpr int ATLAS_SIZE = 4096;

struct ShadowAtlas
{
private:
    struct Tile
    {
        uint32_t x, y;
        uint32_t size;
        bool occupied;
    };

    struct Node
    {
        Tile tile;
        uint32_t parent = -1;
        int32_t children[4] = {-1, -1, -1, -1};
    };

    std::vector<Node> m_nodes;

    float borderColor[4] = {1.0f, 1.0f, 1.0f, 1.0f };

public:
    uint32_t m_resolutions[4] =
    {
        256,
        512,
        1024,
        2048
    };

    GLuint texture;
    GLuint fbo;
    uint32_t tileSize;

    void init();
    // new implementation ===== ->
    Node getNode(uint32_t idx) { return m_nodes[idx]; };
    uint32_t addTile(uint32_t tileSize);
    void freeTile(uint32_t idx);
};
