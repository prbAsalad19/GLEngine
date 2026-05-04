#include "MiniaudioBackend.h"
#include "miniaudio.h"
#include <iostream>
#include <cstring>

bool MiniaudioBackend::init(uint32_t sampleRate, uint32_t channels, uint32_t bufferSizeFrames)
{
    m_bufferFrameCount = bufferSizeFrames;
    m_writeFrames.store(0);
    m_readFrames.store(0);
    memset(m_buffer, 0, sizeof(m_buffer));

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
    if (!m_running) {
        return;
    }

    uint32_t writeF = m_writeFrames.load(std::memory_order_relaxed);
    uint32_t readF = m_readFrames.load(std::memory_order_acquire);
    uint32_t available = CAPACITY - (writeF - readF);
    
    if (available >= frameCount)
    {
        for(uint32_t i = 0; i < frameCount; ++i) {
            uint32_t idx = (writeF + i) % CAPACITY;
            m_buffer[idx * 2]     = data[i * 2];
            m_buffer[idx * 2 + 1] = data[i * 2 + 1];
        }
        m_writeFrames.store(writeF + frameCount, std::memory_order_release);
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
    
    uint32_t writeF = self->m_writeFrames.load(std::memory_order_acquire);
    uint32_t readF = self->m_readFrames.load(std::memory_order_relaxed);
    uint32_t available = writeF - readF;
    
    uint32_t framesToCopy = std::min((uint32_t)frameCount, available);
    float* out = static_cast<float*>(pOutput);
    
    for(uint32_t i = 0; i < framesToCopy; ++i) {
        uint32_t idx = (readF + i) % CAPACITY;
        out[i * 2]     = self->m_buffer[idx * 2];
        out[i * 2 + 1] = self->m_buffer[idx * 2 + 1];
    }
    
    if (framesToCopy < frameCount) {
        uint32_t remaining = frameCount - framesToCopy;
        memset(out + framesToCopy * 2, 0, remaining * 2 * sizeof(float));
    }
    
    self->m_readFrames.store(readF + framesToCopy, std::memory_order_release);
}

bool MiniaudioBackend::isRunning() const
{
    return m_running;
}