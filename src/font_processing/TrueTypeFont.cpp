//
// Created by MihaelGolob on 30. 04. 2024.
//

//#define DEBUG_LOG_GLYPH_BUFFER // uncomment to enable debug logging of glyph buffer
//#define DEBUG_LOG_SDF_BUFFER // uncomment to enable SDf debug logging of glyph buffer

#include <stb_truetype.h>

#include "TrueTypeFont.h"
#include "../utilities.h"
#include "../sdf/sdf.h"

TrueTypeFont::TrueTypeFont() : font_(), padding_(6) { }

void TrueTypeFont::Init(const uint8_t *buffer, uint32_t buffer_size, int32_t font_index, uint32_t pixel_height, int16_t padding) {
    BX_WARN((buffer_size > 256 && buffer_size < 100000000), "(FontIndex %d) TrueType buffer size is suspicious (%d)", font_index, buffer_size)
    BX_UNUSED(buffer_size)

    int offset = stbtt_GetFontOffsetForIndex(buffer, font_index);

    stbtt_InitFont(&font_, buffer, offset);

    scale_ = stbtt_ScaleForMappingEmToPixels(&font_, (float) pixel_height);

    padding_ = padding;
}

FontInfo TrueTypeFont::GetFontInfo() {
    int ascent;
    int descent;
    int line_gap;
    stbtt_GetFontVMetrics(&font_, &ascent, &descent, &line_gap);

    float scale = scale_;

    int x0, y0, x1, y1;
    stbtt_GetFontBoundingBox(&font_, &x0, &y0, &x1, &y1);

    FontInfo out_font_info {};
    out_font_info.scale = 1.0f;
    out_font_info.ascender = bx::round(ascent * scale);
    out_font_info.descender = bx::round(descent * scale);
    out_font_info.line_gap = bx::round(line_gap * scale);
    out_font_info.max_advance_width = bx::round((y1 - y0) * scale);

    out_font_info.underline_position = (x1 - x0) * scale - ascent;
    out_font_info.underline_thickness = (x1 - x0) * scale / 24.f;
    return out_font_info;
}

bool TrueTypeFont::BakeGlyphAlpha(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *out_buffer) {
    int32_t ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&font_, &ascent, &descent, &line_gap);

    int32_t advance, lsb;
    stbtt_GetCodepointHMetrics(&font_, code_point, &advance, &lsb);

    const float scale = scale_;
    int32_t x0, y0, x1, y1;
    stbtt_GetCodepointBitmapBox(&font_, code_point, scale, scale, &x0, &y0, &x1, &y1);

    const int32_t ww = x1 - x0;
    const int32_t hh = y1 - y0;

    glyph_info.offset_x = (float) x0;
    glyph_info.offset_y = (float) y0;
    glyph_info.width = (float) ww;
    glyph_info.height = (float) hh;
    glyph_info.advance_x = bx::round(((float) advance) * scale);
    glyph_info.advance_y = bx::round(((float) (ascent + descent + line_gap)) * scale);

    uint32_t bpp = 1;
    uint32_t dst_pitch = ww * bpp;

    stbtt_MakeCodepointBitmap(&font_, out_buffer, ww, hh, dst_pitch, scale, scale, code_point);

    return true;
}

bool TrueTypeFont::BakeGlyphSdf(CodePoint code_point, GlyphInfo &out_glyph_info, uint8_t *out_buffer) {
    int32_t ascent, descent, line_gap;
    stbtt_GetFontVMetrics(&font_, &ascent, &descent, &line_gap);

    int32_t advance, lsb;
    stbtt_GetCodepointHMetrics(&font_, code_point, &advance, &lsb);

    const float scale = scale_;
    int32_t x0, y0, x1, y1;
    stbtt_GetCodepointBitmapBox(&font_, code_point, scale, scale, &x0, &y0, &x1, &y1);

    const int32_t glyph_width = x1 - x0;
    const int32_t glyph_height = y1 - y0;

    out_glyph_info.offset_x = (float) x0;
    out_glyph_info.offset_y = (float) y0;
    out_glyph_info.width = (float) glyph_width;
    out_glyph_info.height = (float) glyph_height;
    out_glyph_info.advance_x = bx::round(((float) advance) * scale);
    out_glyph_info.advance_y = bx::round(((float) (ascent + descent + line_gap)) * scale);

    int bpp = 1;
    int dst_pitch = glyph_width * bpp;

    stbtt_MakeCodepointBitmap(&font_, out_buffer, glyph_width, glyph_height, dst_pitch, scale, scale, code_point);

#ifdef DEBUG_LOG_GLYPH_BUFFER
    std::cout << "Glyph " << (char)_codePoint << " buffer size is (" << glyphWidth << "," << glyphHeight << ")"  << std::endl;
    
    for (int i = 0; i < glyphHeight; i++) {
        for (int j = 0; j < glyphWidth; j++) {
            auto x = _outBuffer[glyphWidth*i + j];
            if (x != 0) {
                printf("%4d", x);
            } else {
                std::cout << "    ";
            }
        }
        std::cout << std::endl;
    }
#endif
    
    if (glyph_width * glyph_height <= 0) return false;

    int new_glyph_width = glyph_width + 2 * padding_;
    int new_glyph_height = glyph_height + 2 * padding_;

//    BX_ASSERT(newGlyphHeight * newGlyphWidth < 128 * 128, "Glyph buffer overflow (size %d)", newGlyphHeight * newGlyphWidth)

    auto buffer = (uint8_t*)malloc(new_glyph_height * new_glyph_width * sizeof(uint8_t));
    bx::memSet(buffer, 0, new_glyph_height * new_glyph_width * sizeof(uint8_t));

    // copy the original glyph to the center of the new buffer
    for (int i = padding_; i < new_glyph_height - padding_; i++) {
        bx::memCopy(buffer + i * new_glyph_width + padding_, out_buffer + (i - padding_) * glyph_width, glyph_width);
    }

    if (!BuildSignedDistanceField(out_buffer, buffer, new_glyph_width, new_glyph_height, std::min(padding_, padding_))) {
        BX_ASSERT(false, "Failed to build SDF for glyph %c", code_point)
    }

#ifdef DEBUG_LOG_SDF_BUFFER
    std::cout << "SDF glyph " << (char)_codePoint << " buffer size is (" << glyphWidth << "," << glyphHeight << ")"  << std::endl;

    for (int i = 0; i < glyphHeight; i++) {
        for (int j = 0; j < glyphWidth; j++) {
            auto x = _outBuffer[glyphWidth*i + j];
            if (x != 0) {
                printf("%4d", x);
            } else {
                std::cout << "    ";
            }
        }
        std::cout << std::endl;
    }
#endif

    free(buffer);

    out_glyph_info.offset_x -= (float)padding_;
    out_glyph_info.offset_y -= (float)padding_;
    out_glyph_info.width = (float)new_glyph_width;
    out_glyph_info.height = (float)new_glyph_height;
    
    return true;
}
