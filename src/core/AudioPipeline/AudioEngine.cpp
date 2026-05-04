#include "AudioEngine.h"

void AudioEngine::update(float deltaTime)
{
    m_timeAccumulator += deltaTime;
    float bufferDuration = static_cast<float>(m_bufferFrames) / 48000.0f;

    while (m_timeAccumulator >= bufferDuration)
    {
        std::fill(m_mixBuffer.begin(), m_mixBuffer.end(), 0.0f);

        m_audioSourcePool.forEach([&](AudioSource& source)
        {
            if (!source.playing) return;

            AudioClip* clip = m_clipPool.get(source.clip);
            if (!clip || !clip->isValid()) return;

            const auto& samples = clip->getSamples();
            uint32_t frameIndex = static_cast<uint32_t>(source.playhead);

            for (uint32_t i = 0; i < m_bufferFrames; i++)
            {
                uint32_t sampleFrame = (frameIndex + i) % clip->getFrameCount();

                float sample = 0.0f;
                if (clip->getChannelCount() == 2)
                {
                    float L = samples[sampleFrame * 2 + 0];
                    float R = samples[sampleFrame * 2 + 1];
                    sample = (L + R) * 0.5f;
                }
                else
                {
                    sample = samples[sampleFrame];
                }

                sample *= source.volume;
                m_mixBuffer[i * 2 + 0] += sample;
                m_mixBuffer[i * 2 + 1] += sample;
            }

            // avanza di esattamente m_bufferFrames — non deltaTime
            source.playhead += static_cast<float>(m_bufferFrames);
            
            if (source.playhead >= clip->getFrameCount())
            {
                if (source.looping)
                    source.playhead = std::fmod(source.playhead,
                                      static_cast<float>(clip->getFrameCount()));
                else
                    source.playing = false;
            }
        });

        for (float& s : m_mixBuffer) {
            if (s > 1.0f)       s = 1.0f;
            else if (s < -1.0f) s = -1.0f;
        }

        m_backend->submitBuffer(m_mixBuffer.data(), m_bufferFrames);
        m_timeAccumulator -= bufferDuration;
    }
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
