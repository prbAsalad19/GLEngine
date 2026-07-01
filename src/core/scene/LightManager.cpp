#include "LightManager.h"
#include <cmath>
#include <iostream>

LightManager::LightManager(uint32_t capacity)
{
    m_slots.resize(capacity);
    m_freeList.reserve(capacity);
    for (uint32_t i = capacity; i-- > 0; )   // push in reverse so slot 0 comes out first
        m_freeList.push_back(i);

    m_gpuLights.reserve(capacity);
    m_gpuToSlot.reserve(capacity);
}

LightHandle LightManager::add(const LightDesc& desc)
{
    if (m_freeList.empty())
    {
        std::cout << "[LightManager] At capacity (" << m_slots.size() << "), cannot add light.\n";
        return LightHandle{};
    }

    uint32_t s = m_freeList.back();
    m_freeList.pop_back();

    uint32_t gpuIdx = static_cast<uint32_t>(m_gpuLights.size());

    m_slots[s].active     = true;
    m_slots[s].gpuIndex   = gpuIdx;

    m_gpuLights.push_back(toGPU(desc));
    m_cpuDescs.push_back(desc);
    m_isPrimary.push_back(desc.isPrimary);
    m_gpuToSlot.push_back({ s });

    ++m_activeCount;
    m_dirty = true;

    m_activeHandles.push_back(LightHandle(s, m_slots[s].generation));
    return LightHandle(s, m_slots[s].generation);
}

void LightManager::update(LightHandle handle, const LightDesc& desc)
{
    if (!isValid(handle)) return;

    uint32_t gpuIdx = m_slots[handle.slot].gpuIndex;
    m_gpuLights[gpuIdx] = toGPU(desc);
    m_cpuDescs[gpuIdx]  = desc;
    m_isPrimary[gpuIdx] = desc.isPrimary;
    m_dirty = true;
}

void LightManager::remove(LightHandle handle)
{
    if (!isValid(handle)) return;

    uint32_t s      = handle.slot;
    uint32_t gpuIdx = m_slots[s].gpuIndex;

    // Swap with last GPU entry to keep the vector contiguous
    uint32_t lastGpu = static_cast<uint32_t>(m_gpuLights.size()) - 1;

    if (gpuIdx != lastGpu)
    {
        // Move last entry into the removed slot
        m_gpuLights[gpuIdx]  = m_gpuLights[lastGpu];
        m_cpuDescs[gpuIdx]   = m_cpuDescs[lastGpu];
        m_isPrimary[gpuIdx]  = m_isPrimary[lastGpu];
        m_gpuToSlot[gpuIdx]  = m_gpuToSlot[lastGpu];

        // Update the slot that was pointing to lastGpu
        uint32_t movedSlot = m_gpuToSlot[gpuIdx].slotIndex;
        m_slots[movedSlot].gpuIndex = gpuIdx;
    }

    m_gpuLights.pop_back();
    m_cpuDescs.pop_back();
    m_isPrimary.pop_back();
    m_gpuToSlot.pop_back();

    // Invalidate slot
    m_slots[s].active   = false;
    m_slots[s].gpuIndex = UINT32_MAX;
    m_slots[s].generation++;

    m_freeList.push_back(s);
    --m_activeCount;
    m_dirty = true;

    m_activeHandles.erase(
    std::remove_if(m_activeHandles.begin(), m_activeHandles.end(),
        [&](const LightHandle& h) { return h.slot == s; }),
    m_activeHandles.end());
}

// ─────────────────────────────────────────────────────────────────────────────

bool LightManager::isValid(LightHandle handle) const
{
    return handle.slot < m_slots.size()
        && m_slots[handle.slot].active
        && m_slots[handle.slot].generation == handle.generation;
}

GPULight LightManager::toGPU(const LightDesc& desc) const
{
    GPULight g{};

    g.position[0] = desc.position.entries[0];
    g.position[1] = desc.position.entries[1];
    g.position[2] = desc.position.entries[2];
    g.radius      = desc.radius;

    g.color[0]    = desc.color.entries[0];
    g.color[1]    = desc.color.entries[1];
    g.color[2]    = desc.color.entries[2];
    g.intensity   = desc.intensity;

    g.type        = static_cast<uint32_t>(desc.type);

    if (desc.type == LightType::Spot)
    {
        // Normalize direction defensively
        Vector3 d = Vector3::normalize(desc.direction);
        g.direction[0] = d.entries[0];
        g.direction[1] = d.entries[1];
        g.direction[2] = d.entries[2];

        constexpr float DEG2RAD = 3.14159265f / 180.0f;
        g.outerAngleCos = cosf(desc.outerAngle * DEG2RAD);
        g.innerAngleCos = cosf(desc.innerAngle * DEG2RAD);
    }
    else
    {
        // Point light sentinel — outerAngleCos negative flags point light in shader
        g.direction[0]  = 0.0f;
        g.direction[1]  = -1.0f;
        g.direction[2]  = 0.0f;
        g.outerAngleCos = -1.0f;
        g.innerAngleCos =  0.0f;
    }

    return g;
}