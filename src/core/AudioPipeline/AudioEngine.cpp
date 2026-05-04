#include "AudioEngine.h"
#include "core/math/algebricOp.h"
#include <algorithm>

void AudioEngine::update(float /*deltaTime*/)
{
    // Generiamo l'audio in base allo spazio libero nella coda, in modo da mantenere
    // sempre un buffer costante e mascherare eventuali stuttering del gioco, 
    // slegandoci dal tempo della CPU.
    
    uint32_t targetQueuedFrames = m_bufferFrames * 3; // Manteniamo 3 buffer in coda (~50ms latency)
    uint32_t maxFreeSpace = m_backend->getCapacity();
    uint32_t targetFreeSpace = maxFreeSpace > targetQueuedFrames ? maxFreeSpace - targetQueuedFrames : 0;

    while (m_backend->getAvailableWriteSpace() >= targetFreeSpace + m_bufferFrames)
    {
        std::fill(m_mixBuffer.begin(), m_mixBuffer.end(), 0.0f);

        m_audioSourcePool.forEach([&](AudioSource& source)
        {
            if (!source.playing) return;

            AudioClip* clip = m_clipPool.get(source.clip);
            if (!clip || !clip->isValid()) return;

            const auto& samples = clip->getSamples();
            uint32_t channels = clip->getChannelCount();
            uint32_t frameCount = clip->getFrameCount();

            // Calcoliamo il Doppler una volta per questo buffer
            float doppler = calculateDoppler(source);
            
            // Calcoliamo volume e panning (se vuoi aggiungere spazializzazione)
            float volL = 1.0f, volR = 1.0f;
            calculatePanning(source, volL, volR);
            float masterVol = source.volume * calculateAttenuation(source);

            for (uint32_t i = 0; i < m_bufferFrames; i++)
            {
                // 1. Posizione esatta del "cursore" per questo specifico frame del buffer
                // Usiamo un float per non perdere la precisione decimale
                float currentPos = source.playhead + (static_cast<float>(i) * doppler);

                // 2. Troviamo i due campioni adiacenti per l'interpolazione
                uint32_t i0 = static_cast<uint32_t>(std::floor(currentPos)) % frameCount;
                uint32_t i1 = (i0 + 1) % frameCount;

                // 3. 't' è la parte decimale (es. se sei a 10.3, t è 0.3)
                float t = currentPos - std::floor(currentPos);

                float finalSample = 0.0f;

                if (channels == 2)
                {
                    // Prendiamo i campioni (già mixati in mono o processati)
                    float s0 = (samples[i0 * 2] + samples[i0 * 2 + 1]) * 0.5f;
                    float s1 = (samples[i1 * 2] + samples[i1 * 2 + 1]) * 0.5f;
                    // Interpolazione Lineare (LERP)
                    finalSample = s0 + t * (s1 - s0);
                }
                else
                {
                    float s0 = samples[i0];
                    float s1 = samples[i1];
                    finalSample = s0 + t * (s1 - s0);
                }

                // 4. Applichiamo volume, attenuazione e panning al mix buffer
                m_mixBuffer[i * 2 + 0] += finalSample * masterVol * volL;
                m_mixBuffer[i * 2 + 1] += finalSample * masterVol * volR;
            }

            // 5. Avanziamo il playhead reale per il prossimo update
            source.playhead += static_cast<float>(m_bufferFrames) * doppler;
            
            // 6. Gestione Loop / Fine riproduzione
            if (source.playhead >= static_cast<float>(frameCount))
            {
                if (source.looping) {
                    source.playhead = std::fmod(source.playhead, static_cast<float>(frameCount));
                } else {
                    source.playing = false;
                    source.playhead = 0.0f;
                }
            }
        });

        for (float& s : m_mixBuffer) {
            if (s > 1.0f)       s = 1.0f;
            else if (s < -1.0f) s = -1.0f;
        }

        m_backend->submitBuffer(m_mixBuffer.data(), m_bufferFrames);
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
    source.velocity = { 0.0f, 0.0f, 0.0f }; // Inizializzazione ESPLICITA per evitare memoria corrotta (NaN)
    source.volume = volume;
    source.looping = looping;
    source.minDistance = minDistance;
    source.maxDistance = maxDistance;
    source.playhead = 0.0f;

    return m_audioSourcePool.insert("source_" + std::to_string(m_audioSourcePool.size()), std::make_unique<AudioSource>(source));
}

float AudioEngine::calculateAttenuation(const AudioSource& source) const
{
    return std::clamp(source.minDistance / Vector3::dist(m_listener.position, source.position), 0.0f, 1.0f);
}

void AudioEngine::calculatePanning(const AudioSource& source, float& outL, float& outR) const
{
    // direzione dal listener alla sorgente
    Vector2 toSource = {
        source.position.x - m_listener.position.x,
        source.position.y - m_listener.position.y
    };
    toSource.normalize();

    Vector2 listenerForward = {
        m_listener.forward.x,
        m_listener.forward.y
    };
    listenerForward.normalize();

    Vector2 listenerRight = { listenerForward.y, -listenerForward.x };

    float dot = Vector2::dot(toSource, listenerRight);
    float angle = (dot + 1.0f) * (PI / 4.0f); // mappa [-1,1] → [0, PI/2]

    outL = MIN_PAN + (1.0f - MIN_PAN) * cosf(angle);
    outR = MIN_PAN + (1.0f - MIN_PAN) * sinf(angle);

    //std::cout << m_listener.forward.x << " - " << m_listener.forward.y << " - " << m_listener.forward.z << std::endl;
    //std::cout << outL << "       " << outR << std::endl;
}

float AudioEngine::calculateDoppler(const AudioSource& source) const
{
    return (m_speedOfSound + Vector3::mag(m_listener.velocity)) 
                        / (m_speedOfSound + Vector3::mag(source.velocity));    
}