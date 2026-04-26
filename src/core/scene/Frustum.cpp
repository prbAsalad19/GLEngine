#include "Frustum.h"

void Frustum::extractFromMatrix(const mat4& m)
{
    // matrice column-major: entries[col*4 + row]
    // riga r, colonna c = entries[c*4 + r]

    auto row = [&](int r) -> Vector4 {
        return { m.entries[0*4+r], m.entries[1*4+r], m.entries[2*4+r], m.entries[3*4+r] };
    };

    Vector4 r0 = row(0);
    Vector4 r1 = row(1);
    Vector4 r2 = row(2);
    Vector4 r3 = row(3);

    planes[0] = { r3.x + r0.x, r3.y + r0.y, r3.z + r0.z, r3.w + r0.w }; // left
    planes[1] = { r3.x - r0.x, r3.y - r0.y, r3.z - r0.z, r3.w - r0.w }; // right
    planes[2] = { r3.x + r1.x, r3.y + r1.y, r3.z + r1.z, r3.w + r1.w }; // bottom
    planes[3] = { r3.x - r1.x, r3.y - r1.y, r3.z - r1.z, r3.w - r1.w }; // top
    planes[4] = { r3.x + r2.x, r3.y + r2.y, r3.z + r2.z, r3.w + r2.w }; // near
    planes[5] = { r3.x - r2.x, r3.y - r2.y, r3.z - r2.z, r3.w - r2.w }; // far

    for (auto& p : planes)
    {
        float len = sqrtf(p.x*p.x + p.y*p.y + p.z*p.z);
        if (len > 1e-6f) { p.x /= len; p.y /= len; p.z /= len; p.w /= len; }
    }

    // std::cout << "=== FRUSTUM PLANES ===\n";
    // for (int i = 0; i < 6; i++)
    //     std::cout << "piano " << i << ": " 
    //             << planes[i].x << " " << planes[i].y << " " 
    //             << planes[i].z << " " << planes[i].w << "\n";
}

bool Frustum::intersectsAABB(const AABB& aabb) const
{
    for (const auto& p : planes)
    {
        // positive vertex: il punto dell'AABB più nella direzione della normale
        Vector3 pv;
        pv.x = (p.x >= 0.0f) ? aabb.bounds[1].x : aabb.bounds[0].x;
        pv.y = (p.y >= 0.0f) ? aabb.bounds[1].y : aabb.bounds[0].y;
        pv.z = (p.z >= 0.0f) ? aabb.bounds[1].z : aabb.bounds[0].z;

        // se il positive vertex è dietro il piano, l'AABB è fuori
        if (p.x*pv.x + p.y*pv.y + p.z*pv.z + p.w < 0.0f)
            return false;
    }
    return true;
}
