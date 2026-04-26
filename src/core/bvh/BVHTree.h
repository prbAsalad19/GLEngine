#pragma once
#include "core/CoreConfig.h"
#include "core/bvh/BVHNode.h"
#include "core/bvh/aabb.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/RenderObject.h"
#include "core/scene/Frustum.h"
#include <cstdint>
#include <vector>

enum class BVHType : uint8_t
{
    Static,
    QuasiStatic,
    DynamicSlow,
};

class BVHTree
{
public:
    BVHTree(ResourceManager& resources, BVHType type = BVHType::Static)
        : m_resources(resources), m_type(type) {}

    void build(const std::vector<RenderObject>& objects);
    void refit(const std::vector<RenderObject>& objects);
    void update(const std::vector<RenderObject>& objects);

    const std::vector<BVHNode>&  getNodes()   const { return m_nodes; }
    const std::vector<uint32_t>& getIndices() const { return m_indices; }

    void query(const Frustum& frustum,
               const std::vector<RenderObject>& objects,
               std::vector<uint32_t>& outVisible) const;

    void queryRecursive(uint32_t nodeIndex,
                        const Frustum& frustum,
                        const std::vector<RenderObject>& objects,
                        std::vector<uint32_t>& outVisible) const;

private:
    std::vector<BVHNode>  m_nodes;
    std::vector<uint32_t> m_indices;

    BVHType m_type                = BVHType::Static;
    int     m_framesSinceRebuild  = 0;
    int     m_rebuildInterval     = 60;
    float   m_initialCost         = 0.0f;

    void buildRecursive(uint32_t nodeIndex,
                        uint32_t first,
                        uint32_t count,
                        const std::vector<RenderObject>& objects);

    AABB refitRecursive(uint32_t nodeIndex,
                        const std::vector<RenderObject>& objects);

    AABB computeAABB(uint32_t first,
                     uint32_t count,
                     const std::vector<RenderObject>& objects);

    void chooseSplitSAH(uint32_t first,
                        uint32_t count,
                        const std::vector<RenderObject>& objects,
                        const AABB& nodeAABB,
                        int& outAxis,
                        float& outSplitPos);

    float computeSAHCost() const;

    ResourceManager& m_resources;
};
