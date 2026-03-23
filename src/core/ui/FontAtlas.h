#pragma once
#include "core/CoreConfig.h"

// Metrics for a single baked glyph.
// All pixel coordinates refer to the atlas bitmap.
struct GlyphMetrics
{
    float uvX0, uvY0;   // top-left  UV in [0,1]
    float uvX1, uvY1;   // bottom-right UV in [0,1]
    float bearingX;     // horizontal offset from cursor to glyph left edge (pixels)
    float bearingY;     // vertical offset from baseline to glyph top edge (pixels)
    float advance;      // horizontal advance to next cursor position (pixels)
    float width;        // glyph width  in pixels
    float height;       // glyph height in pixels
};

// CPU-side font atlas.
// Loads a TTF file with stb_truetype, bakes the printable ASCII range
// (codepoints 32-126) into a single RGBA bitmap, and stores per-glyph metrics.
//
// Usage:
//   FontAtlas atlas;
//   if (atlas.load("fonts/roboto.ttf", 32.0f)) { ... }
//
// The atlas bitmap is owned by this object and freed on destruction.
// Call getPixels() / getAtlasWidth() / getAtlasHeight() to upload to GPU.

class FontAtlas
{
public:
    FontAtlas() = default;
    ~FontAtlas();

    // Non-copyable (owns heap bitmap).
    FontAtlas(const FontAtlas&) = delete;
    FontAtlas& operator=(const FontAtlas&) = delete;

    // Load a TTF file and bake glyphs at the given pixel height.
    // Returns false on failure (file not found, bake error).
    bool load(const std::string& filepath, float pixelHeight);

    bool isValid() const { return m_pixels != nullptr; }

    // Atlas bitmap — single-channel (luminance/alpha).
    // Dimensions are always power-of-two.
    const unsigned char* getPixels()      const { return m_pixels; }
    int                  getAtlasWidth()  const { return m_atlasW; }
    int                  getAtlasHeight() const { return m_atlasH; }

    // Per-glyph metrics. Codepoints 32-126 are valid indices via:
    //   metrics[codepoint - FIRST_CHAR]
    static constexpr int FIRST_CHAR = 32;
    static constexpr int GLYPH_COUNT = 95;  // 126 - 32 + 1

    const GlyphMetrics& getGlyph(char c) const;

    float getLineHeight()  const { return m_lineHeight; }
    float getPixelHeight() const { return m_pixelHeight; }

    // Measure the pixel width of a string at the loaded size.
    float measureText(const std::string& text) const;

private:
    unsigned char* m_pixels = nullptr;
    int            m_atlasW = 0;
    int            m_atlasH = 0;
    float          m_lineHeight = 0.0f;
    float          m_pixelHeight = 0.0f;

    GlyphMetrics m_glyphs[GLYPH_COUNT] = {};

    static GlyphMetrics s_fallback;  // returned for out-of-range codepoints
};