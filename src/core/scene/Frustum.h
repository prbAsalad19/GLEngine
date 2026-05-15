#pragma once
#include "core/math/algebricOp.h"
#include "core/bvh/aabb.h"

struct Frustum
{
    void extractFromMatrix(const mat4& viewProjection);
    bool intersectsAABB(const AABB& aabb) const;
    bool intersectsSphere(const Vector3 pos, const float radius) const;

private:
    Vector4 planes[6];
};
