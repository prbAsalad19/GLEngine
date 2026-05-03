#include "MiniaudioBackend.h"
#include <iostream>

bool MiniaudioBackend::init(uint32_t sampleRate, uint32_t channels, uint32_t bufferSizeFrames)
{
    m_bufferFrameCount = bufferSizeFrames;

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = ma_format_f32;
    deviceConfig.playback.channels = channels;
    deviceConfig.sampleRate        = sampleRate;
    deviceConfig.dataCallback      = MiniaudioBackend::dataCallback;
    deviceConfig.pUserData        = this;

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

    int backIndex = 1 - m_frontIndex.load();
    memcpy(m_buffers[backIndex], data, frameCount * 2 * sizeof(float));
    m_bufferFrameCount = frameCount;
    m_frontIndex.store(backIndex);
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
    
    uint32_t framesToCopy = std::min(frameCount, self->m_bufferFrameCount);
    uint32_t bytesToCopy  = framesToCopy * 2 * sizeof(float);
    uint32_t totalBytes   = frameCount  * 2 * sizeof(float);
    
    int front = self->m_frontIndex.load();
    
    memset(pOutput, 0, totalBytes);             // silenzio come base
    memcpy(pOutput, self->m_buffers[front], bytesToCopy); // copia quello che hai
}

bool MiniaudioBackend::isRunning() const
{
    return m_running;
}