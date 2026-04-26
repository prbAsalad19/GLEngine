#include "BVHTree.h"
#include "core/bvh/aabb.h"
#include "core/resourcemanager/ResourceManager.h"
#include <cstdint>
#include <strings.h>

// AABB BVHTree::computeAABB(uint32_t first, 
//         uint32_t count, 
//         const std::vector<RenderObject>& objects)
// {
//     AABB result;
//     for (uint32_t i = 0; i < count; i++)
//     {
//         AABB objAABB = m_resources.getMeshAABB(objects[first + i].mesh);
//         result = AABB::merge(result, objAABB);
//     }
//     return result;
// }
AABB BVHTree::computeAABB(uint32_t first,
        uint32_t count,
        const std::vector<RenderObject>& objects)
{
    AABB result;
    for (uint32_t i = 0; i < count; i++)
    {
        AABB meshAABB = m_resources.getMeshAABB(objects[m_indices[first + i]].mesh);
        mat4 model    = objects[m_indices[first + i]].transform.getMatrix();
        AABB worldAABB = AABB::transform(meshAABB, model);

        if (objects[m_indices[first + i]].tier == ObjectTier::QuasiStatic)
        {
            float expansion = worldAABB.surfaceArea() * 0.05f;
            result = AABB::merge(result, worldAABB.expanded(expansion));
        }
        else 
        {
            result = AABB::merge(result, worldAABB);
        }
    }
    return result;
}

void BVHTree::buildRecursive(uint32_t nodeIndex, 
        uint32_t first, 
        uint32_t count, 
        const std::vector<RenderObject>& objects)
{
    m_nodes[nodeIndex].aabb = computeAABB(first, count, objects);

    if (count <= 2)
    {
        m_nodes[nodeIndex].leftChild = -1;
        m_nodes[nodeIndex].firstObject =  first;
        m_nodes[nodeIndex].objectCount = count;
        return;
    }

    // to be changed when implementing the SAH, for now we just split on the longest axis
    int   axis;
    float splitPos;
    chooseSplitSAH(first, count, objects, m_nodes[nodeIndex].aabb, axis, splitPos);
    //===================================================================================

    auto mid = std::partition(
        m_indices.begin() + first,
        m_indices.begin() + first + count,
        [&](uint32_t i) {
            return m_resources.getMeshAABB(objects[i].mesh).center().entries[axis] < splitPos;
        }
    );

    uint32_t midIdx = static_cast<uint32_t>(mid - m_indices.begin());
    if (midIdx == first || midIdx == first + count)
        midIdx = first + count / 2;

    uint32_t leftIdx = static_cast<uint32_t>(m_nodes.size());
    m_nodes.push_back({});
    m_nodes.push_back({});
    m_nodes[nodeIndex].leftChild = static_cast<int32_t>(leftIdx);

    buildRecursive(leftIdx,     first,   midIdx - first,              objects);
    buildRecursive(leftIdx + 1, midIdx,  (first + count) - midIdx,    objects);
}

float BVHTree::computeSAHCost() const
{
    float cost = 0.0f;
    for (const auto& node : m_nodes)
    {
        if (node.isLeaf())
            cost += node.aabb.surfaceArea() * node.objectCount;
        else
            cost += node.aabb.surfaceArea();
    }
    return cost;
}

void BVHTree::build(const std::vector<RenderObject>& objects)
{
    m_nodes.clear();
    m_indices.clear();

    if (objects.empty()) return;

    m_indices.resize(objects.size());
    for (uint32_t i = 0; i < objects.size(); i++)
        m_indices[i] = i;

    m_nodes.push_back({});
    
    buildRecursive(0, 0, static_cast<uint32_t>(objects.size()), objects);

    m_initialCost = computeSAHCost();
    m_framesSinceRebuild = 0;
}

void BVHTree::query(const Frustum& frustum,
                    const std::vector<RenderObject>& objects,
                    std::vector<uint32_t>& outVisible) const
{
    if (m_nodes.empty()) return;
    queryRecursive(0, frustum, objects, outVisible);
}

void BVHTree::queryRecursive(uint32_t nodeIndex,
                              const Frustum& frustum,
                              const std::vector<RenderObject>& objects,
                              std::vector<uint32_t>& outVisible) const
{
    const BVHNode& node = m_nodes[nodeIndex];

    // testa l'AABB del nodo contro il frustum
    if (!frustum.intersectsAABB(node.aabb))
        return; // tutto il sottoalbero è fuori, scarta

    if (node.isLeaf())
    {
        // aggiungi gli oggetti di questa foglia alla lista visibile
        for (uint32_t i = 0; i < node.objectCount; i++)
            outVisible.push_back(m_indices[node.firstObject + i]);
        return;
    }

    // nodo interno — traversa i figli
    queryRecursive(node.leftChild,     frustum, objects, outVisible);
    queryRecursive(node.leftChild + 1, frustum, objects, outVisible);
}

void BVHTree::chooseSplitSAH(uint32_t first,
                              uint32_t count,
                              const std::vector<RenderObject>& objects,
                              const AABB& nodeAABB,
                              int& outAxis,
                              float& outSplitPos)
{
    constexpr int NUM_BUCKETS = 8;

    struct Bucket
    {
        AABB  aabb;
        int   count = 0;
    };

    float bestCost     = std::numeric_limits<float>::max();
    int   bestAxis     = 0;
    float bestSplitPos = 0.0f;

    for (int axis = 0; axis < 3; axis++)
    {
        float axisMin = nodeAABB.bounds[0].entries[axis];
        float axisMax = nodeAABB.bounds[1].entries[axis];
        float extent  = axisMax - axisMin;

        if (extent < 1e-6f) continue; // asse piatto, salta

        // --- popola i bucket ---
        Bucket buckets[NUM_BUCKETS];

        for (uint32_t i = first; i < first + count; i++)
        {
            AABB objAABB = m_resources.getMeshAABB(objects[m_indices[i]].mesh);
            float center = objAABB.center().entries[axis];

            int b = static_cast<int>(NUM_BUCKETS * (center - axisMin) / extent);
            b = std::clamp(b, 0, NUM_BUCKETS - 1);

            buckets[b].aabb  = (buckets[b].count == 0) 
                               ? objAABB 
                               : AABB::merge(buckets[b].aabb, objAABB);
            buckets[b].count++;
        }

        // --- valuta ogni piano di split ---
        for (int i = 1; i < NUM_BUCKETS; i++)
        {
            AABB  leftAABB;  int leftCount  = 0;
            AABB  rightAABB; int rightCount = 0;

            for (int j = 0;          j < i;           j++)
            {
                if (buckets[j].count == 0) continue;
                leftAABB  = (leftCount  == 0) ? buckets[j].aabb : AABB::merge(leftAABB,  buckets[j].aabb);
                leftCount += buckets[j].count;
            }
            for (int j = i; j < NUM_BUCKETS; j++)
            {
                if (buckets[j].count == 0) continue;
                rightAABB  = (rightCount == 0) ? buckets[j].aabb : AABB::merge(rightAABB, buckets[j].aabb);
                rightCount += buckets[j].count;
            }

            if (leftCount == 0 || rightCount == 0) continue;

            float cost = leftAABB.surfaceArea()  * leftCount
                       + rightAABB.surfaceArea() * rightCount;

            if (cost < bestCost)
            {
                bestCost     = cost;
                bestAxis     = axis;
                bestSplitPos = axisMin + extent * (i / static_cast<float>(NUM_BUCKETS));
            }
        }
    }

    outAxis     = bestAxis;
    outSplitPos = bestSplitPos;
}

AABB BVHTree::refitRecursive(uint32_t nodeIndex,
                              const std::vector<RenderObject>& objects)
{
    BVHNode& node = m_nodes[nodeIndex];

    if (node.isLeaf())
    {
        // ricalcola l'AABB della foglia dagli oggetti reali in world space
        AABB result;
        for (uint32_t i = 0; i < node.objectCount; i++)
        {
            uint32_t objIdx  = m_indices[node.firstObject + i];
            AABB meshAABB    = m_resources.getMeshAABB(objects[objIdx].mesh);
            AABB worldAABB   = AABB::transform(meshAABB, objects[objIdx].transform.getMatrix());
            result           = AABB::merge(result, worldAABB);
        }
        node.aabb = result;
        return result;
    }

    // nodo interno — prima refit i figli, poi aggiorna questo nodo
    AABB left  = refitRecursive(node.leftChild,     objects);
    AABB right = refitRecursive(node.leftChild + 1, objects);

    node.aabb = AABB::merge(left, right);
    return node.aabb;
}

void BVHTree::refit(const std::vector<RenderObject>& objects)
{
    if (m_nodes.empty()) return;
    refitRecursive(0, objects);
}

void BVHTree::update(const std::vector<RenderObject>& objects)
{
    if (m_type == BVHType::Static || m_type == BVHType::QuasiStatic) return;

    m_framesSinceRebuild++;

    bool intervalExpired = m_framesSinceRebuild >= m_rebuildInterval;
    bool qualityDegraded = computeSAHCost() > m_initialCost * 2.0f;

    if (intervalExpired || qualityDegraded)
    {
        build(objects);
        // build già resetta m_initialCost e m_framesSinceRebuild
    }
    else
    {
        refit(objects);
    }
}
