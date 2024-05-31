//
// Created by MihaelGolob on 30. 04. 2024.
//

//#define DEBUG_LOG_GLYPH_BUFFER // uncomment to enable debug logging of glyph buffer
//#define DEBUG_LOG_SDF_BUFFER // uncomment to enable SDf debug logging of glyph buffer

#include <stb_truetype.h>

#include "TrueTypeFont.h"
#include "../utilities.h"
#include "../sdf/sdf.h"

TrueTypeFont::TrueTypeFont() : m_font(), m_padding(6) { }

bool TrueTypeFont::init(const uint8_t *_buffer, uint32_t _bufferSize, int32_t _fontIndex, uint32_t _pixelHeight, int16_t _padding) {
    BX_WARN((_bufferSize > 256 && _bufferSize < 100000000), "(FontIndex %d) TrueType buffer size is suspicious (%d)", _fontIndex, _bufferSize)
    BX_UNUSED(_bufferSize)

    int offset = stbtt_GetFontOffsetForIndex(_buffer, _fontIndex);

    stbtt_InitFont(&m_font, _buffer, offset);

    m_scale = stbtt_ScaleForMappingEmToPixels(&m_font, (float) _pixelHeight);

    m_padding = _padding;
    return true;
}
#pragma clang diagnostic pop

FontInfo TrueTypeFont::getFontInfo() {
    int ascent;
    int descent;
    int lineGap;
    stbtt_GetFontVMetrics(&m_font, &ascent, &descent, &lineGap);

    float scale = m_scale;

    int x0, y0, x1, y1;
    stbtt_GetFontBoundingBox(&m_font, &x0, &y0, &x1, &y1);

    FontInfo outFontInfo {};
    outFontInfo.scale = 1.0f;
    outFontInfo.ascender = bx::round(ascent * scale);
    outFontInfo.descender = bx::round(descent * scale);
    outFontInfo.lineGap = bx::round(lineGap * scale);
    outFontInfo.maxAdvanceWidth = bx::round((y1 - y0) * scale);

    outFontInfo.underlinePosition = (x1 - x0) * scale - ascent;
    outFontInfo.underlineThickness = (x1 - x0) * scale / 24.f;
    return outFontInfo;
}

bool TrueTypeFont::bakeGlyphAlpha(CodePoint _codePoint, GlyphInfo &_glyphInfo, uint8_t *_outBuffer) {
    int32_t ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&m_font, &ascent, &descent, &lineGap);

    int32_t advance, lsb;
    stbtt_GetCodepointHMetrics(&m_font, _codePoint, &advance, &lsb);

    const float scale = m_scale;
    int32_t x0, y0, x1, y1;
    stbtt_GetCodepointBitmapBox(&m_font, _codePoint, scale, scale, &x0, &y0, &x1, &y1);

    const int32_t ww = x1 - x0;
    const int32_t hh = y1 - y0;

    _glyphInfo.offset_x = (float) x0;
    _glyphInfo.offset_y = (float) y0;
    _glyphInfo.width = (float) ww;
    _glyphInfo.height = (float) hh;
    _glyphInfo.advance_x = bx::round(((float) advance) * scale);
    _glyphInfo.advance_y = bx::round(((float) (ascent + descent + lineGap)) * scale);

    uint32_t bpp = 1;
    uint32_t dstPitch = ww * bpp;

    stbtt_MakeCodepointBitmap(&m_font, _outBuffer, ww, hh, dstPitch, scale, scale, _codePoint);

    return true;
}

bool TrueTypeFont::BakeGlyphSDF(CodePoint _codePoint, GlyphInfo &_outGlyphInfo, uint8_t *_outBuffer) {
    int32_t ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&m_font, &ascent, &descent, &lineGap);

    int32_t advance, lsb;
    stbtt_GetCodepointHMetrics(&m_font, _codePoint, &advance, &lsb);

    const float scale = m_scale;
    int32_t x0, y0, x1, y1;
    stbtt_GetCodepointBitmapBox(&m_font, _codePoint, scale, scale, &x0, &y0, &x1, &y1);

    const int32_t glyphWidth = x1 - x0;
    const int32_t glyphHeight = y1 - y0;

    _outGlyphInfo.offset_x = (float) x0;
    _outGlyphInfo.offset_y = (float) y0;
    _outGlyphInfo.width = (float) glyphWidth;
    _outGlyphInfo.height = (float) glyphHeight;
    _outGlyphInfo.advance_x = bx::round(((float) advance) * scale);
    _outGlyphInfo.advance_y = bx::round(((float) (ascent + descent + lineGap)) * scale);

    int bpp = 1;
    int dstPitch = glyphWidth * bpp;

    stbtt_MakeCodepointBitmap(&m_font, _outBuffer, glyphWidth, glyphHeight, dstPitch, scale, scale, _codePoint);

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
    
    if (glyphWidth * glyphHeight <= 0) return false;

    int newGlyphWidth = glyphWidth + 2 * m_padding;
    int newGlyphHeight = glyphHeight + 2 * m_padding;

//    BX_ASSERT(newGlyphHeight * newGlyphWidth < 128 * 128, "Glyph buffer overflow (size %d)", newGlyphHeight * newGlyphWidth)

    auto buffer = (uint8_t*)malloc(newGlyphHeight * newGlyphWidth * sizeof(uint8_t));
    bx::memSet(buffer, 0, newGlyphHeight * newGlyphWidth * sizeof(uint8_t));

    // copy the original glyph to the center of the new buffer
    for (int i = m_padding; i < newGlyphHeight - m_padding; i++) {
        bx::memCopy(buffer + i * newGlyphWidth + m_padding, _outBuffer + (i - m_padding) * glyphWidth, glyphWidth);
    }

    if (!BuildSignedDistanceField(_outBuffer, buffer, newGlyphWidth, newGlyphHeight, std::min(m_padding, m_padding))) {
        BX_ASSERT(false, "Failed to build SDF for glyph %c", _codePoint)
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

    _outGlyphInfo.offset_x -= (float)m_padding;
    _outGlyphInfo.offset_y -= (float)m_padding;
    _outGlyphInfo.width = (float)newGlyphWidth;
    _outGlyphInfo.height = (float)newGlyphHeight;
    
    return true;
}
