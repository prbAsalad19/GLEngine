#pragma once
#include "opengl/OpenGLConfig.h"
#include "core/math/algebricOp.h"

struct ShadowCaster
{
    uint32_t lightIndex;
    uint32_t tileX, tileY;
    uint32_t tileSize;
    mat4 lightSpaceMatrix;
};

struct GPUShadowCaster
{
    mat4     lightSpaceMatrix;  // 64 byte
    float    atlasRect[4];      // x, y, w, h in UV — 16 byte
    uint32_t lightIndex;        // quale luce — 4 byte
    float    bias;              // 4 byte
    float    _pad[2];           // 8 byte — totale 96 byte
};
