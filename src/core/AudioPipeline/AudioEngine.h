#pragma once
#include <iostream>
#include <vector>
#include "core/resourcemanager/ResourceManager.h"
#include "AudioClip.h"
#include "core/math/algebricOp.h"
#include "IAudioBackend.h"

enum class AudioSourceType : uint32_t
{
    Point = 0,
    Spot = 1
};

struct AudioSource
{
    AudioClipHandle clip;
    Vector3         position;
    Vector3         velocity;    // for the doppler effect
    float           volume      = 1.0f;
    float           minDistance = 1.0f;
    float           maxDistance = 50.0f;
    bool            looping     = false;
    bool            playing     = false;

    // internal state 
    float           playhead    = 0.0f;  // position in frame (float for the resampling)
};

struct Listener
{
    Vector3 position;
    Vector3 forward;
    Vector3 up;
};

class AudioEngine
{
public:
    AudioEngine(Listener listener, ResourcePool<AudioClipTag, AudioClip>& clipPool) : m_listener(listener), m_clipPool(clipPool) {}

    void init(std::unique_ptr<IAudioBackend> backend, uint32_t bufferFrames)
    {
        m_bufferFrames = bufferFrames;
        m_backend = std::move(backend);
        m_mixBuffer.resize(m_bufferFrames * 2, 0.0f);
        m_backend->init(48000, 2, bufferFrames);
    }

    AudioSourceHandle createSource(const AudioClipHandle& clip,
                                const Vector3& position = {0.0f, 0.0f, 0.0f},
                                float volume = 1.0f,
                                bool looping = false,
                                float minDistance = 1.0f,
                                float maxDistance = 50.0f);
    
    void update(float deltaTime);

    void setListener(const Listener& listener) { m_listener = listener; }

    void play(AudioSourceHandle handle)
    {
        AudioSource* source = m_audioSourcePool.get(handle);
        if (source) source->playing = true;
    }

    void stop(AudioSourceHandle handle)
    {
        AudioSource* source = m_audioSourcePool.get(handle);
        if (source) { source->playing = false; source->playhead = 0.0f; }
    }    

    void pause(AudioSourceHandle handle)
    {
        AudioSource* source = m_audioSourcePool.get(handle);
        if (source) source->playing = false;
    }

private:
    ResourcePool<AudioClipTag, AudioClip>& m_clipPool;
    ResourcePool<AudioSourceTag, AudioSource> m_audioSourcePool;

    std::unique_ptr<IAudioBackend> m_backend;
    std::vector<float>             m_mixBuffer;
    uint32_t                       m_bufferFrames = 1024;

    Listener m_listener;
    float    m_timeAccumulator = 0.0f;
};
