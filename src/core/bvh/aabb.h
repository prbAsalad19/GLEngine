#pragma once
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include <limits>

class AABB
{
public:
    AABB() {
        bounds[0] = {std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()};
        bounds[1] = {std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min()};
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
};
