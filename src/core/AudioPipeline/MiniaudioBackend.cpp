#include "MiniaudioBackend.h"
#include "miniaudio.h"
#include <iostream>
#include <cstring>

bool MiniaudioBackend::init(uint32_t sampleRate, uint32_t channels, uint32_t bufferSizeFrames)
{
    m_bufferFrameCount = bufferSizeFrames;
    m_writeIndex.store(0);
    m_readIndex.store(0);

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_f32;
    deviceConfig.playback.channels = channels;
    deviceConfig.sampleRate        = sampleRate;
    deviceConfig.dataCallback      = MiniaudioBackend::dataCallback;
    deviceConfig.pUserData        = this;
    deviceConfig.periodSizeInFrames = bufferSizeFrames;

    if (ma_device_init(NULL, &deviceConfig, &m_device) != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio device\n";
        return false;
    }

    if (ma_device_start(&m_device) != MA_SUCCESS) {
        std::cerr << "Failed to start audio device\n";
        ma_device_uninit(&m_device);
        return false;
    }

    m_running = true;
    return true;
}

void MiniaudioBackend::submitBuffer(const float* data, uint32_t frameCount)
{
    if (!m_running || frameCount > m_bufferFrameCount) {
        std::cerr << "Audio backend not running or frame count exceeds buffer size\n";
        return;
    }

    int writeIdx = m_writeIndex.load(std::memory_order_relaxed);
    int nextWriteIdx = (writeIdx + 1) % NUM_BUFFERS;
    
    // Check if the queue is full
    if (nextWriteIdx != m_readIndex.load(std::memory_order_acquire))
    {
        memcpy(m_buffers[writeIdx], data, frameCount * 2 * sizeof(float));
        m_writeIndex.store(nextWriteIdx, std::memory_order_release);
    }
    else
    {
        // Queue full, dropping buffer to avoid stutter/stall
    }
}

void MiniaudioBackend::shutdown()
{
    if (m_running) {
        ma_device_uninit(&m_device);
        m_running = false;
    }
}

void MiniaudioBackend::dataCallback(ma_device* device, void* pOutput, 
                                     const void* pInput, ma_uint32 frameCount)
{
    MiniaudioBackend* self = static_cast<MiniaudioBackend*>(device->pUserData);
    
    uint32_t totalBytes = frameCount * 2 * sizeof(float);
    
    int readIdx = self->m_readIndex.load(std::memory_order_relaxed);
    if (readIdx != self->m_writeIndex.load(std::memory_order_acquire))
    {
        uint32_t framesToCopy = std::min(frameCount, self->m_bufferFrameCount);
        uint32_t bytesToCopy  = framesToCopy * 2 * sizeof(float);
        
        memset(pOutput, 0, totalBytes); // Fill with silence first if we copy less
        memcpy(pOutput, self->m_buffers[readIdx], bytesToCopy);
        
        self->m_readIndex.store((readIdx + 1) % NUM_BUFFERS, std::memory_order_release);
    }
    else
    {
        // Queue empty, output silence
        memset(pOutput, 0, totalBytes);
    }
}

bool MiniaudioBackend::isRunning() const
{
    return m_running;
}