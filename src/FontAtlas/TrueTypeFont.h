//
// Created by MihaelGolob on 30. 04. 2024.
//

#pragma once

#include <stb_truetype.h>

#include "FontInfo.h"
#include "GlyphInfo.h"
#include "NumberTypes.h"

typedef int CodePoint;

class TrueTypeFont {
public:
    TrueTypeFont();

    ~TrueTypeFont() = default;

    /// Initialize from  an external buffer
    /// @remark The ownership of the buffer is external, and you must ensure it stays valid up to this object lifetime
    /// @return true if the initialization succeed
    bool init(const uint8_t *_buffer, uint32_t _bufferSize, int32_t _fontIndex, uint32_t _pixelHeight,
              int16_t _widthPadding, int16_t _heightPadding);

    /// return the font descriptor of the current font
    FontInfo getFontInfo();

    /// raster a glyph as 8bit alpha to a memory buffer
    /// update the GlyphInfo according to the raster strategy
    /// @ remark buffer min size: glyphInfo.m_width * glyphInfo * height * sizeof(char)
    bool bakeGlyphAlpha(CodePoint _codePoint, GlyphInfo &_outGlyphInfo, uint8_t *_outBuffer);

    /// raster a glyph as 8bit signed distance to a memory buffer
    /// update the GlyphInfo according to the raster strategy
    /// @ remark buffer min size: glyphInfo.m_width * glyphInfo * height * sizeof(char)
    bool bakeGlyphDistance(CodePoint _codePoint, GlyphInfo &_outGlyphInfo, uint8_t *_outBuffer);

private:
    friend class FontManager;

    stbtt_fontinfo m_font;
    float m_scale;

    int16_t m_widthPadding;
    int16_t m_heightPadding;
};