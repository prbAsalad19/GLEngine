#pragma once
#include "core/resourcemanager/ResourceHandle.h"
#include "core/scene/Transform.h"
#include <cstdint>

enum class ObjectTier : uint8_t
{
    Static,
    QuasiStatic,
    DynamicSlow,
    DynamicFast
};

struct RenderObject
{
    MeshHandle mesh;
    MaterialHandle material; //-1 for no texture
    Transform transform;
    ObjectTier     tier = ObjectTier::Static;
};