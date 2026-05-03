#pragma once
#include <vector>
#include <cstdint>
#include <string>

class AudioClip
{
public:

    static AudioClip load(const std::string& filename);
    
    bool isValid() const { return m_valid; }
    const std::vector<float>& getSamples() const { return m_samples; }
    uint32_t getSampleRate() const { return m_sampleRate; }
    uint32_t getChannelCount() const { return m_channels; }
    uint32_t getFrameCount() const { return m_frameCount; }
    float getDuration() const { return static_cast<float>(m_frameCount) / m_sampleRate; }

    void free() { m_samples.clear(); m_samples.shrink_to_fit(); m_valid = false; }

private:
    std::vector<float> m_samples;    
    uint32_t           m_sampleRate; 
    uint32_t           m_channels;   
    uint32_t           m_frameCount; 
    bool               m_valid;      
};
