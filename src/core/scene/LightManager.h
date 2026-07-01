#pragma once
#include "core/CoreConfig.h"
#include "core/math/algebricOp.h"
#include <cstdint>
#include <vector>
#include <functional>

// ─────────────────────────────────────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────────────────────────────────────

static constexpr uint32_t MAX_LIGHTS = 1024;

// ─────────────────────────────────────────────────────────────────────────────
//  LightHandle  — same slot/generation pattern as ResourceHandle
// ─────────────────────────────────────────────────────────────────────────────

struct LightHandle
{
    uint32_t slot;
    uint32_t generation;

    LightHandle() : slot(UINT32_MAX), generation(0) {}
    LightHandle(uint32_t s, uint32_t g) : slot(s), generation(g) {}

    bool isNull() const { return slot == UINT32_MAX; }
    bool operator==(const LightHandle& o) const { return slot == o.slot && generation == o.generation; }
    bool operator!=(const LightHandle& o) const { return !(*this == o); }
};

namespace std {
    template<>
    struct hash<LightHandle>
    {
        size_t operator()(const LightHandle& h) const noexcept
        {
            return std::hash<uint32_t>()(h.slot) ^ 
                   (std::hash<uint32_t>()(h.generation) << 1);
        }
    };
}

// ─────────────────────────────────────────────────────────────────────────────
//  Light types
// ─────────────────────────────────────────────────────────────────────────────

enum class LightType : uint32_t
{
    Point = 0,
    Spot  = 1,
    Directional = 2
};

// ─────────────────────────────────────────────────────────────────────────────
//  LightDesc  — CPU-friendly description, used for add/update
// ─────────────────────────────────────────────────────────────────────────────

struct LightDesc
{
    Vector3   position  = { 0.0f, 0.0f, 0.0f };
    Vector3   color     = { 1.0f, 1.0f, 1.0f };
    float     intensity = 1.0f;
    float     radius    = 10.0f;

    LightType type      = LightType::Point;
    bool isPrimary = false; //only for directional lights, the other are ingnored in the making of shadows

    // Spot only — ignored for point lights
    Vector3   direction  = { 0.0f, -1.0f, 0.0f };
    float     innerAngle = 30.0f;   // degrees
    float     outerAngle = 45.0f;   // degrees
};

// ─────────────────────────────────────────────────────────────────────────────
//  GPULight  — packed for std430 SSBO upload, 64 bytes, cache-aligned
// ─────────────────────────────────────────────────────────────────────────────

struct GPULight
{
    float    position[3];
    float    radius;            // 16

    float    color[3];
    float    intensity;         // 32

    float    direction[3];      // spot only
    float    outerAngleCos;     // cos(outerAngle); negative sentinel = point light   // 48

    uint32_t type;
    float    innerAngleCos;     // cos(innerAngle) for smooth falloff
    float    _pad[2];           // 64
};
static_assert(sizeof(GPULight) == 64, "GPULight must be 64 bytes for std430 alignment");

// ─────────────────────────────────────────────────────────────────────────────
//  LightManager
//
//  Owns all lights in the scene. The renderer reads getGPULights() each frame
//  and uploads to the light SSBO only when isDirty() is true.
//
//  Usage:
//    LightHandle h = lights.add({ .position = {1,2,3}, .radius = 5.0f });
//    lights.update(h, desc);
//    lights.remove(h);
// ─────────────────────────────────────────────────────────────────────────────

class LightManager
{
public:
    // Slot metadata for handle validation
    struct Slot
    {
        uint32_t generation = 0;
        bool     active     = false;
        uint32_t gpuIndex   = UINT32_MAX;   // index into m_gpuLights
    };
    explicit LightManager(uint32_t capacity = MAX_LIGHTS);

    // Add a light — returns a handle. Returns null handle if at capacity.
    LightHandle add(const LightDesc& desc);

    // Update an existing light. No-op if handle is invalid.
    void update(LightHandle handle, const LightDesc& desc);

    // Remove a light. No-op if handle is invalid.
    void remove(LightHandle handle);

    // Returns the contiguous GPU buffer — upload this to the SSBO.
    const std::vector<GPULight>& getGPULights() const { return m_gpuLights; }
    const Slot& getSlot(LightHandle handle) const { return m_slots[handle.slot]; }
    const std::vector<LightHandle>& getActiveHandles() const { return m_activeHandles; }
    const std::vector<bool>& getIsPrimary() const { return m_isPrimary; }
    bool isPrimary(uint32_t gpuIndex) const { return m_isPrimary[gpuIndex]; }
    const std::vector<LightDesc>& getCPUDescs() const { return m_cpuDescs; }

    // Number of currently active lights.
    uint32_t count() const { return m_activeCount; }

    // Dirty flag — true if any add/update/remove happened since last clearDirty().
    bool isDirty()    const { return m_dirty; }
    void clearDirty()       { m_dirty = false; }

private:
    // Maps gpuIndex → slot index, for compact removal
    struct GPUEntry
    {
        uint32_t slotIndex = UINT32_MAX;
    };

    std::vector<Slot>     m_slots;
    std::vector<uint32_t> m_freeList;

    std::vector<GPULight> m_gpuLights;   // always contiguous, no holes
    std::vector<bool> m_isPrimary;
    std::vector<GPUEntry> m_gpuToSlot;   // parallel to m_gpuLights
    std::vector<LightHandle> m_activeHandles;

    std::vector<LightDesc> m_cpuDescs;

    uint32_t m_activeCount = 0;
    bool     m_dirty       = false;

    bool     isValid(LightHandle handle) const;
    GPULight toGPU(const LightDesc& desc) const;
};