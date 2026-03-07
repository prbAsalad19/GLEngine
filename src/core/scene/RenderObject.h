#include "core/resourcemanager/ResourceHandle.h"
#include "core/scene/Transform.h"

struct RenderObject
{
    MeshHandle mesh;
    MaterialHandle material; //-1 for no texture
    Transform transform;
};