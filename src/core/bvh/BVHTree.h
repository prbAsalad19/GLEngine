#pragma once
#include "core/CoreConfig.h"
#include "core/bvh/BVHNode.h"
#include "core/bvh/aabb.h"
#include "core/resourcemanager/ResourceManager.h"
#include "core/scene/RenderObject.h"
#include <cstdint>
#include <vector>

class BVHTree
{
public:
    BVHTree(ResourceManager& resources) : m_resources(resources) {};

    void build(const std::vector<RenderObject>& objects);

    const std::vector<BVHNode>& getNodes() const { return m_nodes; }
    const std::vector<uint32_t>& getIndices() const { return m_indices; }

private:
    std::vector<BVHNode> m_nodes;
    std::vector<uint32_t> m_indices;

    void buildRecursive(uint32_t nodeIndex, 
        uint32_t first, 
        uint32_t count, 
        const std::vector<RenderObject>& objects);

    AABB computeAABB(uint32_t first, 
        uint32_t count, 
        const std::vector<RenderObject>& objects); //get the AABB of all the objects

    void chooseSplitSAH(uint32_t first,
                              uint32_t count,
                              const std::vector<RenderObject>& objects,
                              const AABB& nodeAABB,
                              int& outAxis,
                              float& outSplitPos);

    ResourceManager& m_resources;
};
