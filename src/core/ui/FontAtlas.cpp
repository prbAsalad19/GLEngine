#include "FontAtlas.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "core/ui/stb_truetype.h"

GlyphMetrics FontAtlas::s_fallback = {};

FontAtlas::~FontAtlas()
{
    delete[] m_pixels;
}

bool FontAtlas::load(const std::string& filepath, float pixelHeight)
{
    // ── 1. Read TTF file into memory ─────────────────────────────────────────
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cout << "[FontAtlas] Failed to open: " << filepath << "\n";
        return false;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> ttfBuffer(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(ttfBuffer.data()), size))
    {
        std::cout << "[FontAtlas] Failed to read: " << filepath << "\n";
        return false;
    }

    m_pixelHeight = pixelHeight;

    // ── 2. Choose atlas dimensions ───────────────────────────────────────────
    // Start at 256x256 and double until the bake succeeds.
    // For sizes ≤ 48px a 512x512 atlas is almost always sufficient.
    // For larger sizes (e.g. 96px) we may need 1024x1024.
    m_atlasW = 256;
    m_atlasH = 256;

    stbtt_bakedchar bakedChars[GLYPH_COUNT];

    while (true)
    {
        std::vector<unsigned char> tempBitmap(
            static_cast<size_t>(m_atlasW * m_atlasH), 0);

        int result = stbtt_BakeFontBitmap(
            ttfBuffer.data(), 0,
            pixelHeight,
            tempBitmap.data(),
            m_atlasW, m_atlasH,
            FIRST_CHAR, GLYPH_COUNT,
            bakedChars);

        if (result > 0)
        {
            // Success — convert single-channel bitmap to RGBA.
            // We store luminance in R and full white in RGB so the shader
            // can tint the text by multiplying with fragColor.rgb.
            // Alpha channel = luminance value for smooth antialiasing.
            size_t pixelCount = static_cast<size_t>(m_atlasW * m_atlasH);
            m_pixels = new unsigned char[pixelCount * 4];
            for (size_t i = 0; i < pixelCount; ++i)
            {
                m_pixels[i * 4 + 0] = 255;               // R
                m_pixels[i * 4 + 1] = 255;               // G
                m_pixels[i * 4 + 2] = 255;               // B
                m_pixels[i * 4 + 3] = tempBitmap[i];     // A (glyph coverage)
            }
            break;
        }

        // Atlas too small — double dimensions.
        if (m_atlasW >= 4096)
        {
            std::cout << "[FontAtlas] Atlas size limit reached for: "
                << filepath << "\n";
            return false;
        }
        m_atlasW *= 2;
        m_atlasH *= 2;
    }

    // ── 3. Build GlyphMetrics from stbtt_bakedchar ───────────────────────────
    float invW = 1.0f / static_cast<float>(m_atlasW);
    float invH = 1.0f / static_cast<float>(m_atlasH);

    stbtt_fontinfo info;
    stbtt_InitFont(&info, ttfBuffer.data(),
        stbtt_GetFontOffsetForIndex(ttfBuffer.data(), 0));

    float scale = stbtt_ScaleForPixelHeight(&info, pixelHeight);
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    m_lineHeight = (ascent - descent + lineGap) * scale;

    for (int i = 0; i < GLYPH_COUNT; ++i)
    {
        const stbtt_bakedchar& bc = bakedChars[i];
        GlyphMetrics& gm = m_glyphs[i];

        gm.uvX0 = bc.x0 * invW;
        gm.uvY0 = bc.y0 * invH;
        gm.uvX1 = bc.x1 * invW;
        gm.uvY1 = bc.y1 * invH;

        gm.width = static_cast<float>(bc.x1 - bc.x0);
        gm.height = static_cast<float>(bc.y1 - bc.y0);
        gm.bearingX = bc.xoff;
        gm.bearingY = bc.yoff;  // negative = above baseline
        gm.advance = bc.xadvance;
    }

    std::cout << "[FontAtlas] Loaded: " << filepath
        << " | size: " << pixelHeight
        << " | atlas: " << m_atlasW << "x" << m_atlasH << "\n";
    return true;
}

const GlyphMetrics& FontAtlas::getGlyph(char c) const
{
    int idx = static_cast<int>(c) - FIRST_CHAR;
    if (idx < 0 || idx >= GLYPH_COUNT) return s_fallback;
    return m_glyphs[idx];
}

float FontAtlas::measureText(const std::string& text) const
{
    float width = 0.0f;
    for (char c : text)
    {
        int idx = static_cast<int>(c) - FIRST_CHAR;
        if (idx < 0 || idx >= GLYPH_COUNT) continue;
        width += m_glyphs[idx].advance;
    }
    return width;
}