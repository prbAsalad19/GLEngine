#pragma once
#include "core/CoreConfig.h"
#include "aabb.h"

class BVHNode
{
public:
    AABB aabb;
    int32_t leftChild;
    int32_t firstObject;
    uint32_t objectCount;

    bool isLeaf() const {return leftChild == -1; }
};