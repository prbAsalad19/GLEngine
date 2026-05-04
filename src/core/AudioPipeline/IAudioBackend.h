#pragma once
#include <cstdint>

class IAudioBackend
{
public:

    virtual ~IAudioBackend() = default;

    virtual bool init(uint32_t sampleRate, uint32_t channels, uint32_t bufferSizeFrames) = 0;

    virtual void shutdown() = 0;

    virtual void submitBuffer(const float* data, uint32_t frameCount) = 0;

    virtual bool isRunning() const = 0;

    virtual uint32_t getAvailableWriteSpace() const = 0;
    virtual uint32_t getCapacity() const = 0;

private:

};
