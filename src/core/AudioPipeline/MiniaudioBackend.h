#pragma once
#include "miniaudio.h"
#include "IAudioBackend.h"
#include <atomic>

class MiniaudioBackend : public IAudioBackend
{
public:

    bool init(uint32_t sampleRate,
              uint32_t channels,
              uint32_t bufferSizeFrames) override;

    void shutdown() override;
    void submitBuffer(const float* data, uint32_t frameCount) override;
    bool isRunning() const override;

    uint32_t getAvailableWriteSpace() const override
    {
        uint32_t writeF = m_writeFrames.load(std::memory_order_relaxed);
        uint32_t readF = m_readFrames.load(std::memory_order_acquire);
        return CAPACITY - (writeF - readF);
    }

    uint32_t getCapacity() const override
    {
        return CAPACITY;
    }

private:
    ma_device   m_device;
    bool        m_running = false;

    static constexpr uint32_t CAPACITY = 16384; // Frames

    float                m_buffer[CAPACITY * 2];
    std::atomic<uint32_t> m_writeFrames { 0 };
    std::atomic<uint32_t> m_readFrames { 0 };
    uint32_t             m_bufferFrameCount { 0 };

    static void dataCallback(ma_device*  device,
                             void*       pOutput,
                             const void* pInput,
                             ma_uint32   frameCount);
};

