#pragma once
#include "opengl/OpenGLConfig.h"
#include "core/ui/FontAtlas.h"

// GPU-side font atlas.
// Uploads the FontAtlas RGBA bitmap to an OpenGL texture.
// Linear filtering is used for sub-pixel smoothness.
// The texture is bound as a regular sampler2D in the UI shader.

class OpenGLFontAtlas
{
public:
    explicit OpenGLFontAtlas(const FontAtlas& atlas);
    ~OpenGLFontAtlas();

    OpenGLFontAtlas(const OpenGLFontAtlas&) = delete;
    OpenGLFontAtlas& operator=(const OpenGLFontAtlas&) = delete;

    void bind(int unit) const;

    // Expose CPU metrics — no need to duplicate them here.
    const FontAtlas& getCPUAtlas() const { return m_cpuAtlas; }

private:
    const FontAtlas& m_cpuAtlas;
    GLuint           m_texture = 0;
};