### 1. # FontAtlas.h

This header file defines a class `FontAtlas` that is responsible for loading and managing a font atlas, which is a bitmap containing glyphs from a TTF (TrueType Font) file. The atlas is designed to be used in a software rendering context, such as games or graphical applications.

### 2. Brief Overview

The `FontAtlas` class provides functionality to load a TrueType font file, bake the printable ASCII range into a single RGBA bitmap, and store per-glyph metrics. This allows for efficient text rendering by providing quick access to glyph positions and dimensions without the need to repeatedly load individual glyphs from a font file.

### 3. Table of Classes/Structs/Enums

- **GlyphMetrics**: A struct that contains metrics for a single baked glyph.
- **FontAtlas**: The main class responsible for managing the font atlas.

### 4. Detailed Methods Section

#### GlyphMetrics Struct

```cpp
struct GlyphMetrics {
    float uvX0, uvY0;   // top-left UV in [0,1]
    float uvX1, uvY1;   // bottom-right UV in [0,1]
    float bearingX;     // horizontal offset from cursor to glyph left edge (pixels)
    float bearingY;     // vertical offset from baseline to glyph top edge (pixels)
    float advance;      // horizontal advance to next cursor position (pixels)
    float width;        // glyph width in pixels
    float height;       // glyph height in pixels
};
```

**Purpose**: 
- Stores detailed information about a single glyph, including its UV coordinates within the atlas bitmap and layout metrics.

#### FontAtlas Class

##### Constructor and Destructor

```cpp
FontAtlas() = default;
~FontAtlas();
```

**Purpose**: 
- Default constructor and destructor. The destructor frees any allocated resources.

##### Non-Copyable Members

```cpp
FontAtlas(const FontAtlas&) = delete;
FontAtlas& operator=(const FontAtlas&) = delete;
```

**Purpose**: 
- Prevents copying of `FontAtlas` objects to ensure that each instance manages its own resources.

##### load Method

```cpp
bool load(const std::string& filepath, float pixelHeight);
```

**Full Signature**:
```cpp
bool load(const std::string& filepath, float pixelHeight);
```

**Return Type Explanation**: 
- Returns `true` if the font atlas is successfully loaded and baked; otherwise, returns `false`.

**Internal Logic/Purpose**:
- Loads a TTF file using `stb_truetype`, bakes the printable ASCII range into a single RGBA bitmap, and stores per-glyph metrics.

##### isValid Method

```cpp
bool isValid() const { return m_pixels != nullptr; }
```

**Full Signature**:
```cpp
bool isValid() const;
```

**Return Type Explanation**: 
- Returns `true` if the font atlas is valid (i.e., it has been successfully loaded); otherwise, returns `false`.

##### getPixels Method

```cpp
const unsigned char* getPixels()      const { return m_pixels; }
```

**Full Signature**:
```cpp
const unsigned char* getPixels()      const;
```

**Return Type Explanation**: 
- Returns a pointer to the atlas bitmap's pixel data.

##### getAtlasWidth and getAtlasHeight Methods

```cpp
int                  getAtlasWidth()  const { return m_atlasW; }
int                  getAtlasHeight() const { return m_atlasH; }
```

**Full Signature**:
```cpp
int getAtlasWidth()  const;
int getAtlasHeight() const;
```

**Return Type Explanation**: 
- Returns the width and height of the atlas bitmap, respectively.

##### getGlyph Method

```cpp
const GlyphMetrics& getGlyph(char c) const;
```

**Full Signature**:
```cpp
const GlyphMetrics& getGlyph(char c) const;
```

**Return Type Explanation**: 
- Returns a reference to the metrics for the specified glyph. If the codepoint is out of range, it returns a fallback metrics.

##### getLineHeight and getPixelHeight Methods

```cpp
float getLineHeight()  const { return m_lineHeight; }
float getPixelHeight() const { return m_pixelHeight; }
```

**Full Signature**:
```cpp
float getLineHeight()  const;
float getPixelHeight() const;
```

**Return Type Explanation**: 
- Returns the line height and the pixel height of the font, respectively.

##### measureText Method

```cpp
float measureText(const std::string& text) const;
```

**Full Signature**:
```cpp
float measureText(const std::string& text) const;
```

**Return Type Explanation**: 
- Returns the total width in pixels of the specified string when rendered at the loaded size.

### Summary

The `FontAtlas` class is a crucial component for managing font atlases, providing efficient access to glyph metrics and bitmap data. It leverages external libraries like `stb_truetype` to handle the loading and baking of TTF files into a single atlas bitmap. The class ensures that resources are properly managed by preventing copying and freeing allocated resources upon destruction.