#include "OpenGLFontAtlas.h"
#include <iostream>

OpenGLFontAtlas::OpenGLFontAtlas(const FontAtlas& atlas)
    : m_cpuAtlas(atlas)
{
    if (!atlas.isValid())
    {
        std::cout << "[OpenGLFontAtlas] CPU atlas is not valid, skipping GPU upload.\n";
        return;
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RGBA,
        atlas.getAtlasWidth(),
        atlas.getAtlasHeight(),
        0,
        GL_RGBA, GL_UNSIGNED_BYTE,
        atlas.getPixels());

    // Linear filtering — critical for antialiased text at any size.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << "[OpenGLFontAtlas] GPU texture created, ID = " << m_texture << "\n";
}

OpenGLFontAtlas::~OpenGLFontAtlas()
{
    if (m_texture)
        glDeleteTextures(1, &m_texture);
}

void OpenGLFontAtlas::bind(int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}