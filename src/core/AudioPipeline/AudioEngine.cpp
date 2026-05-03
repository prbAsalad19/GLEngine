#include "AudioEngine.h"

void AudioEngine::update(float deltaTime)
{
    std::fill(m_mixBuffer.begin(), m_mixBuffer.end(), 0.0f);

    m_audioSourcePool.forEach([&](AudioSource& source)
    {
        if (!source.playing) return;

        // Update playhead
        AudioClip* clip = m_clipPool.get(source.clip);
        if (!clip || !clip->isValid()) return;

        source.playhead += deltaTime * static_cast<float>(clip->getSampleRate());
        if (source.playhead >= clip->getFrameCount())
        {
            if (source.looping)
                source.playhead = std::fmod(source.playhead, static_cast<float>(clip->getFrameCount()));
            else
            {
                source.playing = false;
                return;
            }
        }

        const auto& samples = clip->getSamples();
        uint32_t frameIndex = static_cast<uint32_t>(source.playhead);

        for (uint32_t i = 0; i < m_bufferFrames; i++)
        {
            // gestisci il wrap per il looping
            uint32_t sampleFrame = (frameIndex + i) % clip->getFrameCount();
            
            float sample = 0.0f;
            
            if (clip->getChannelCount() == 2)
            {
                // stereo — media dei due canali per avere mono
                float L = samples[sampleFrame * 2 + 0];
                float R = samples[sampleFrame * 2 + 1];
                sample = (L + R) * 0.5f;
            }
            else
            {
                // mono
                sample = samples[sampleFrame];
            }
            
            sample *= source.volume;
            
            // accumula su entrambi i canali uguale (no panning ancora)
            m_mixBuffer[i * 2 + 0] += sample;  // L
            m_mixBuffer[i * 2 + 1] += sample;  // R
        }
    });

    for (float& s : m_mixBuffer)
    s = std::clamp(s, -1.0f, 1.0f);

    m_backend->submitBuffer(m_mixBuffer.data(), m_bufferFrames);
}

void AudioEngine::init(std::unique_ptr<IAudioBackend> backend, uint32_t bufferFrames)
{
    m_bufferFrames = bufferFrames;
    m_backend = std::move(backend);
    m_mixBuffer.resize(m_bufferFrames * 2, 0.0f);
    m_backend->init(48000, 2, bufferFrames);
}

AudioSourceHandle AudioEngine::createSource(const AudioClipHandle& clip,
                            const Vector3& position,
                            float volume,
                            bool looping,
                            float minDistance,
                            float maxDistance)
{
    AudioSource source;
    source.clip = clip;
    source.position = position;
    source.volume = volume;
    source.looping = looping;
    source.minDistance = minDistance;
    source.maxDistance = maxDistance;

    return m_audioSourcePool.insert("source_" + std::to_string(m_audioSourcePool.size()), std::make_unique<AudioSource>(source));
}
