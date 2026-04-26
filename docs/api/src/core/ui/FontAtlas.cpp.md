## FontAtlas.cpp

### Brief Overview
`FontAtlas.cpp` is a source file that implements the `FontAtlas` class, which handles loading and managing font atlases for rendering text in a graphical application. This includes reading TrueType fonts, baking glyphs into an atlas, and providing metrics for each glyph.

### Table of Classes/Structs/Enums
- **FontAtlas**: Manages the creation and management of font atlases.
- **GlyphMetrics**: Stores metrics for individual glyphs.

### Detailed Methods Section

#### 1. `~FontAtlas()`
**Full Signature:**
```cpp
FontAtlas::~FontAtlas()
```

**Return Type Explanation:** None (void)

**Internal Logic/Purpose:**
- Deletes dynamically allocated memory for the atlas pixels to prevent memory leaks.
- Ensures that all resources are properly released when a `FontAtlas` object is destroyed.

#### 2. `bool load(const std::string& filepath, float pixelHeight)`
**Full Signature:**
```cpp
bool FontAtlas::load(const std::string& filepath, float pixelHeight)
```

**Return Type Explanation:** Boolean indicating success or failure of the loading process.

**Internal Logic/Purpose:**
- Opens and reads a TrueType font file from the specified `filepath`.
- Initializes an atlas with dimensions starting at 256x256 and doubles in size if necessary until the baking process succeeds.
- Bakes glyphs into the atlas using the STB TrueType library.
- Converts the single-channel bitmap to RGBA format for easy tinting.
- Calculates glyph metrics such as UV coordinates, width, height, bearing, and advance.
- Stores the atlas pixels and glyph metrics internally.

#### 3. `const GlyphMetrics& getGlyph(char c) const`
**Full Signature:**
```cpp
const GlyphMetrics& FontAtlas::getGlyph(char c) const
```

**Return Type Explanation:** Constant reference to `GlyphMetrics` for the specified character.

**Internal Logic/Purpose:**
- Retrieves the glyph metrics for a given character.
- Returns fallback metrics if the character is not found in the atlas.

#### 4. `float measureText(const std::string& text) const`
**Full Signature:**
```cpp
float FontAtlas::measureText(const std::string& text) const
```

**Return Type Explanation:** The total width of the input text, based on glyph advances.

**Internal Logic/Purpose:**
- Iterates through each character in the input string.
- Sums up the advance values of the corresponding glyphs to calculate the total width of the text.