#include "ShadowAtlas.h"
#include <cstdint>
#include <memory>

uint32_t ShadowAtlas::addTile(uint32_t tileSize)
{
    if (m_nodes.empty()) 
    {
        Node root;
        root.tile.x = 0;
        root.tile.y = 0;
        root.tile.size = ATLAS_SIZE;
        root.tile.occupied = false;
        m_nodes.push_back(root);
    }


    std::vector<uint32_t> nodesToVisit;
    nodesToVisit.push_back(0);

    while(!nodesToVisit.empty())
    {
        uint32_t currentNodeIdx = nodesToVisit.back();
        nodesToVisit.pop_back();
        
        Node& node = m_nodes[currentNodeIdx];
        
        if(node.tile.occupied || node.tile.size < tileSize)
        {
            continue;
        }

        if (node.children[0] == -1 && node.tile.size == tileSize)
        {
            node.tile.occupied = true;
            return currentNodeIdx;
        }

        if (node.children[0] == -1)
        {
            uint32_t half = node.tile.size / 2;
            uint32_t baseIdx = m_nodes.size();

            Node topLeft, topRight, bottomLeft, bottomRight;

            topLeft.tile     = { node.tile.x,        node.tile.y,        half, false };
            topRight.tile    = { node.tile.x + half, node.tile.y,        half, false };
            bottomLeft.tile  = { node.tile.x,        node.tile.y + half, half, false };
            bottomRight.tile = { node.tile.x + half, node.tile.y + half, half, false };

            topLeft.parent = currentNodeIdx;
            topRight.parent = currentNodeIdx;
            bottomLeft.parent = currentNodeIdx;
            bottomRight.parent = currentNodeIdx;

            m_nodes.push_back(topLeft);
            m_nodes.push_back(topRight);
            m_nodes.push_back(bottomLeft);
            m_nodes.push_back(bottomRight);

            Node& textNode = m_nodes[currentNodeIdx];

            textNode.children[0] = baseIdx + 0;
            textNode.children[1] = baseIdx + 1;
            textNode.children[2] = baseIdx + 2;
            textNode.children[3] = baseIdx + 3;
        }

        Node& parentNode = m_nodes[currentNodeIdx];
        for (int i = 0; i < 4; ++i)
        {
            if (parentNode.children[i] != -1)
            {
                nodesToVisit.push_back(parentNode.children[i]);
            }
        }
    }

    return uint32_t(-1);
}

void ShadowAtlas::freeTile(uint32_t idx)
{
    if (idx >= m_nodes.size()) return;

    m_nodes[idx].tile.occupied = false;
    int32_t currentParentIdx = m_nodes[idx].parent;

    while (currentParentIdx != -1)
    {
        Node& parentNode = m_nodes[currentParentIdx];
        bool canMerge = true;

        for (int i = 0; i < 4; ++i)
        {
            int32_t childIdx = parentNode.children[i];

            if (childIdx == -1 || m_nodes[childIdx].tile.occupied || m_nodes[childIdx].children[0] != -1)
            {
                canMerge = false;
                break;
            }
        }

        if (!canMerge)
        {
            break;
        }

        parentNode.children[0] = -1;
        parentNode.children[1] = -1;
        parentNode.children[2] = -1;
        parentNode.children[3] = -1;

        currentParentIdx = parentNode.parent;
    }
}