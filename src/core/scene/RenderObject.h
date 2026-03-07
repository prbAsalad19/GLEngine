#include "core/resourcemanager/ResourceHandle.h"
#include "core/scene/Transform.h"

struct RenderObject
{
    ResourceHandle<MeshTag> mesh;
    ResourceHandle<MaterialTag> material; //-1 for no texture
    Transform transform;
};