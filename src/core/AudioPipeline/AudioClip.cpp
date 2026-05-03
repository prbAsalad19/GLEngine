#include "AudioClip.h"
#include "miniaudio.c"
#include <vector>

AudioClip AudioClip::load(const std::string& filename)
{
    AudioClip clip;

    ma_decoder_config config = ma_decoder_config_init(
        ma_format_f32,  // formato output: float 32-bit
        2,              // canali output: stereo
        48000           // sample rate output: 48000 Hz
    );

    ma_decoder decoder;
    ma_result result = ma_decoder_init_file(filename.c_str(), &config, &decoder);
    if (result != MA_SUCCESS)
    {
        clip.m_valid = false;
        return clip; 
    }

    std::vector<float> samples;
    constexpr ma_uint64 CHUNK_SIZE = 4096; // frame per chunk
    float chunkBuffer[CHUNK_SIZE * 2];     // stereo quindi *2
    ma_uint64 totalFrames = 0;

    ma_uint64 frameCount = 0;
    ma_decoder_get_length_in_pcm_frames(&decoder, &frameCount);
    if (frameCount > 0)
    {
        samples.reserve(frameCount * 2); // stereo quindi *2
    }

    while (true)
    {
        ma_uint64 framesThisChunk;
        ma_decoder_read_pcm_frames(&decoder, chunkBuffer, CHUNK_SIZE, &framesThisChunk);
        
        if (framesThisChunk == 0) break; // fine del file
        
        // appendi al vettore
        samples.insert(samples.end(), 
            chunkBuffer, 
            chunkBuffer + framesThisChunk * 2); // *2 stereo
        
        totalFrames += framesThisChunk;
    }

    ma_decoder_uninit(&decoder);

    clip.m_samples    = std::move(samples);
    clip.m_sampleRate = 48000;
    clip.m_channels   = 2;
    clip.m_frameCount = static_cast<uint32_t>(totalFrames);
    clip.m_valid      = totalFrames > 0;

    return clip;
}
