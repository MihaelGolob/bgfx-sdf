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
    void Init(const uint8_t *buffer, uint32_t buffer_size, int32_t font_index, uint32_t pixel_height, int16_t padding);

    /// return the font descriptor of the current font
    FontInfo GetFontInfo();
    
    GlyphInfo GetGlyphInfo(CodePoint code_point);

    /// raster a glyph as 8bit alpha to a memory buffer
    /// update the GlyphInfo according to the raster strategy
    /// @ remark buffer min size: glyphInfo.m_width * glyphInfo * height * sizeof(char)
    bool BakeGlyphAlpha(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *out_buffer);

    // raster a glyph as 8bit signed distance field to a memory buffer
    bool BakeGlyphSdf(CodePoint code_point, GlyphInfo &out_glyph_info, uint8_t *out_buffer);

private:
    friend class FontManager;

    stbtt_fontinfo font_;
    float scale_{};

    int16_t padding_;
};