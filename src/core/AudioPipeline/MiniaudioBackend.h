#pragma once
#include "miniaudio.c"
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

    // ping-pong
    static constexpr uint32_t MAX_FRAMES = 4096;

    // due buffer stereo — MAX_FRAMES frame * 2 canali
    float                m_buffers[2][MAX_FRAMES * 2];
    std::atomic<int>     m_frontIndex { 0 };
    uint32_t             m_bufferFrameCount { 0 };

    static void dataCallback(ma_device*  device,
                             void*       pOutput,
                             const void* pInput,
                             ma_uint32   frameCount);
};

