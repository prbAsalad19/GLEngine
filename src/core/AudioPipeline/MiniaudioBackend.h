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

private:
    ma_device   m_device;
    bool        m_running = false;

    static constexpr uint32_t MAX_FRAMES = 4096;
    static constexpr uint32_t NUM_BUFFERS = 8;

    float                m_buffers[NUM_BUFFERS][MAX_FRAMES * 2];
    std::atomic<int>     m_writeIndex { 0 };
    std::atomic<int>     m_readIndex { 0 };
    uint32_t             m_bufferFrameCount { 0 };

    static void dataCallback(ma_device*  device,
                             void*       pOutput,
                             const void* pInput,
                             ma_uint32   frameCount);
};

