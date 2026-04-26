#pragma once
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include <limits>

class AABB
{
public:
    AABB() {
        bounds[0] = {std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
        bounds[1] = {-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max(),-std::numeric_limits<float>::max()};
    }
    Vector3 bounds[2];

    Vector3 center() const { return Vector3::midpoint(bounds[0], bounds[1]); }
    Vector3 min() const { return  bounds[0]; }
    Vector3 max() const { return  bounds[1]; }

    static AABB merge(const AABB& a, const AABB& b)
    {
        AABB result;
        result.bounds[0] = Vector3::Vmin(a.bounds[0], b.bounds[0]);
        result.bounds[1] = Vector3::Vmax(a.bounds[1], b.bounds[1]);  
        return result;
    }

    float surfaceArea() const
    {
        Vector3 d;
        d.entries[0] = std::abs(bounds[0].entries[0] - bounds[1].entries[0]);
        d.entries[1] = std::abs(bounds[0].entries[1] - bounds[1].entries[1]);
        d.entries[2] = std::abs(bounds[0].entries[2] - bounds[1].entries[2]);

        return 2.0f * (d.entries[0]*d.entries[1] 
            + d.entries[1]*d.entries[2] 
            + d.entries[2]*d.entries[0]);
    }

    void expand(const Vector3& p)
    {
        bounds[0] = Vector3::Vmin(bounds[0], p);
        bounds[1] = Vector3::Vmax(bounds[1], p);
    }

    static AABB transform(const AABB& aabb, const mat4& matrix)
    {
        // trasforma tutti e 8 i vertici del box e ricostruisci l'AABB
        AABB result;
        float minX = aabb.bounds[0].x, minY = aabb.bounds[0].y, minZ = aabb.bounds[0].z;
        float maxX = aabb.bounds[1].x, maxY = aabb.bounds[1].y, maxZ = aabb.bounds[1].z;

        Vector3 corners[8] = {
            {minX, minY, minZ}, {maxX, minY, minZ},
            {minX, maxY, minZ}, {maxX, maxY, minZ},
            {minX, minY, maxZ}, {maxX, minY, maxZ},
            {minX, maxY, maxZ}, {maxX, maxY, maxZ}
        };

        for (const auto& c : corners)
        {
            // moltiplica per la matrice (solo xyz, w=1)
            Vector3 t;
            t.x = matrix.entries[0]*c.x + matrix.entries[4]*c.y + matrix.entries[8]*c.z  + matrix.entries[12];
            t.y = matrix.entries[1]*c.x + matrix.entries[5]*c.y + matrix.entries[9]*c.z  + matrix.entries[13];
            t.z = matrix.entries[2]*c.x + matrix.entries[6]*c.y + matrix.entries[10]*c.z + matrix.entries[14];
            result.expand(t);
        }
        return result;
    }
};
