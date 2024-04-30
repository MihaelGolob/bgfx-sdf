//
// Created by MihaelGolob on 30. 04. 2024.
//

#include <stb_truetype.h>

#include "TrueTypeFont.h"
#include "../utilities.h"

TrueTypeFont::TrueTypeFont() : m_font(), m_widthPadding(6), m_heightPadding(6) { }

bool TrueTypeFont::init(const uint8_t *_buffer, uint32_t _bufferSize, int32_t _fontIndex, uint32_t _pixelHeight,
                        int16_t _widthPadding, int16_t _heightPadding) {
    BX_WARN((_bufferSize > 256 && _bufferSize < 100000000), "(FontIndex %d) TrueType buffer size is suspicious (%d)",
            _fontIndex, _bufferSize);
    BX_WARN((_pixelHeight > 4 && _pixelHeight < 128), "(FontIndex %d) TrueType pixel height is suspicious (%d)",
            _fontIndex, _pixelHeight);
    BX_UNUSED(_bufferSize);

    int offset = stbtt_GetFontOffsetForIndex(_buffer, _fontIndex);

    stbtt_InitFont(&m_font, _buffer, offset);

    m_scale = stbtt_ScaleForMappingEmToPixels(&m_font, (float) _pixelHeight);

    m_widthPadding = _widthPadding;
    m_heightPadding = _heightPadding;
    return true;
}

FontInfo TrueTypeFont::getFontInfo() {
    int ascent;
    int descent;
    int lineGap;
    stbtt_GetFontVMetrics(&m_font, &ascent, &descent, &lineGap);

    float scale = m_scale;

    int x0, y0, x1, y1;
    stbtt_GetFontBoundingBox(&m_font, &x0, &y0, &x1, &y1);

    FontInfo outFontInfo;
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