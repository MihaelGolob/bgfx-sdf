//
// Created by MihaelGolob on 30. 04. 2024.
//

#include "TextBuffer.h"
#include "CubeAtlas.h"
#include "utf8.h"
#include "FontInfo.h"
#include "../managers/FontManager.h"

#include <bgfx/embedded_shader.h>
#include <cwchar>  // wcslen

#define MAX_BUFFERED_CHARACTERS (8192 - 5)

TextBuffer::TextBuffer(FontManager *_fontManager)
        : m_styleFlags(STYLE_NORMAL), m_textColor(UINT32_MAX), m_backgroundColor(UINT32_MAX),
          m_overlineColor(UINT32_MAX), m_underlineColor(UINT32_MAX), m_strikeThroughColor(UINT32_MAX),
          m_outlineWidth(3.0f), m_outlineColor(0x000000ff), m_dropShadowColor(0x0000005a), m_dropShadowSoftener(1.0f),
          m_penX(0), m_penY(0), m_originX(0), m_originY(0), m_lineAscender(0), m_lineDescender(0), m_lineGap(0),
          m_previousCodePoint(0), m_fontManager(_fontManager),
          m_vertexBuffer(new TextVertex[MAX_BUFFERED_CHARACTERS * 4]),
          m_indexBuffer(new uint16_t[MAX_BUFFERED_CHARACTERS * 6]),
          m_styleBuffer(new uint8_t[MAX_BUFFERED_CHARACTERS * 4]), m_indexCount(0), m_lineStartIndex(0),
          m_vertexCount(0) {
    m_rectangle.width = 0;
    m_rectangle.height = 0;
    m_dropShadowOffset[0] = 0.00f;
    m_dropShadowOffset[1] = 0.00f;
}

TextBuffer::~TextBuffer() {
    delete[] m_vertexBuffer;
    delete[] m_indexBuffer;
    delete[] m_styleBuffer;
}

void TextBuffer::appendText(FontHandle _fontHandle, const char *_string, const char *_end) {
    if (m_vertexCount == 0) {
        m_originX = m_penX;
        m_originY = m_penY;
        m_lineDescender = 0;
        m_lineAscender = 0;
        m_lineGap = 0;
        m_previousCodePoint = 0;
    }

    CodePoint codepoint = 0;
    uint32_t state = 0;

    if (_end == nullptr) {
        _end = _string + bx::strLen(_string);
    }
    BX_ASSERT(_end >= _string, "")

    for (; *_string && _string < _end; ++_string) {
        if (utf8_decode(&state, (uint32_t *) &codepoint, *_string) == UTF8_ACCEPT) {
            appendGlyph(_fontHandle, codepoint, false);
        }
    }

    BX_ASSERT(state == UTF8_ACCEPT, "The string is not well-formed")
}

void TextBuffer::appendText(FontHandle _fontHandle, const wchar_t *_string, const wchar_t *_end) {
    if (m_vertexCount == 0) {
        m_originX = m_penX;
        m_originY = m_penY;
        m_lineDescender = 0;
        m_lineAscender = 0;
        m_lineGap = 0;
        m_previousCodePoint = 0;
    }

    if (_end == nullptr) {
        _end = _string + wcslen(_string);
    }
    BX_ASSERT(_end >= _string, "")

    for (const wchar_t *_current = _string; _current < _end; ++_current) {
        uint32_t _codePoint = *_current;
        appendGlyph(_fontHandle, _codePoint, false);
    }
}

void TextBuffer::appendAtlasFace(uint16_t _faceIndex) {
    if (m_vertexCount / 4 >= MAX_BUFFERED_CHARACTERS) {
        return;
    }

    float x0 = m_penX;
    float y0 = m_penY;
    float x1 = x0 + (float) m_fontManager->getAtlas()->getTextureSize();
    float y1 = y0 + (float) m_fontManager->getAtlas()->getTextureSize();

    m_fontManager->getAtlas()->packFaceLayerUV(_faceIndex, (uint8_t *) m_vertexBuffer,
                                               sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u),
                                               sizeof(TextVertex)
    );

    setVertex(m_vertexCount + 0, x0, y0, m_backgroundColor);
    setVertex(m_vertexCount + 1, x0, y1, m_backgroundColor);
    setVertex(m_vertexCount + 2, x1, y1, m_backgroundColor);
    setVertex(m_vertexCount + 3, x1, y0, m_backgroundColor);

    m_indexBuffer[m_indexCount + 0] = m_vertexCount + 0;
    m_indexBuffer[m_indexCount + 1] = m_vertexCount + 1;
    m_indexBuffer[m_indexCount + 2] = m_vertexCount + 2;
    m_indexBuffer[m_indexCount + 3] = m_vertexCount + 0;
    m_indexBuffer[m_indexCount + 4] = m_vertexCount + 2;
    m_indexBuffer[m_indexCount + 5] = m_vertexCount + 3;
    m_vertexCount += 4;
    m_indexCount += 6;
}

void TextBuffer::clearTextBuffer() {
    m_penX = 0;
    m_penY = 0;
    m_originX = 0;
    m_originY = 0;

    m_vertexCount = 0;
    m_indexCount = 0;
    m_lineStartIndex = 0;
    m_lineAscender = 0;
    m_lineDescender = 0;
    m_lineGap = 0;
    m_previousCodePoint = 0;
    m_rectangle.width = 0;
    m_rectangle.height = 0;
}

void TextBuffer::appendGlyph(FontHandle _handle, CodePoint _codePoint, bool shadow) {
    if (_codePoint == L'\t') {
        for (uint32_t ii = 0; ii < 4; ++ii) {
            appendGlyph(_handle, L' ', shadow);
        }
        return;
    }

    const GlyphInfo *glyph = m_fontManager->getGlyphInfo(_handle, _codePoint);
    BX_WARN(nullptr != glyph, "Glyph not found (font handle %d, code point %d)", _handle.idx, _codePoint)
    if (nullptr == glyph) {
        m_previousCodePoint = 0;
        return;
    }

    if (m_vertexCount / 4 >= MAX_BUFFERED_CHARACTERS) {
        m_previousCodePoint = 0;
        return;
    }

    const FontInfo &font = m_fontManager->getFontInfo(_handle);

    if (_codePoint == L'\n') {
        m_penX = m_originX;
        m_penY += m_lineGap + m_lineAscender - m_lineDescender;
        m_lineGap = font.lineGap;
        m_lineDescender = font.descender;
        m_lineAscender = font.ascender;
        m_lineStartIndex = m_vertexCount;
        m_previousCodePoint = 0;
        return;
    }

    //is there a change of font size that require the text on the left to be centered again ?
    if (font.ascender > m_lineAscender || (font.descender < m_lineDescender)) {
        if (font.descender < m_lineDescender) {
            m_lineDescender = font.descender;
            m_lineGap = font.lineGap;
        }

        float txtDecals = (font.ascender - m_lineAscender);
        m_lineAscender = font.ascender;
        m_lineGap = font.lineGap;
        verticalCenterLastLine((txtDecals), (m_penY - m_lineAscender), (m_penY + m_lineAscender - m_lineDescender + m_lineGap));
    }

    float kerning = m_fontManager->getKerning(_handle, m_previousCodePoint, _codePoint);
    m_penX += kerning;

    const GlyphInfo &blackGlyph = m_fontManager->getBlackGlyph();
    const Atlas *atlas = m_fontManager->getAtlas();
    const AtlasRegion &atlasRegion = atlas->getRegion(glyph->regionIndex);

    if (shadow) {
        if (atlasRegion.getType() != AtlasRegion::TYPE_BGRA8) {
            float extraXOffset = m_dropShadowOffset[0];
            float extraYOffset = m_dropShadowOffset[1];

            float x0 = m_penX + (glyph->offset_x) + extraXOffset;
            float y0 = (m_penY + m_lineAscender + (glyph->offset_y) + extraYOffset);
            float x1 = (x0 + glyph->width);
            float y1 = (y0 + glyph->height);

            bx::memSet(&m_vertexBuffer[m_vertexCount], 0, sizeof(TextVertex) * 4);

            atlas->packUV(glyph->regionIndex, (uint8_t *) m_vertexBuffer, sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u2), sizeof(TextVertex));

            uint32_t adjustedDropShadowColor = ((((m_dropShadowColor & 0xff000000) >> 8) * (m_textColor >> 24)) & 0xff000000) | (m_dropShadowColor & 0x00ffffff);
            setVertex(m_vertexCount + 0, x0, y0, adjustedDropShadowColor);
            setVertex(m_vertexCount + 1, x0, y1, adjustedDropShadowColor);
            setVertex(m_vertexCount + 2, x1, y1, adjustedDropShadowColor);
            setVertex(m_vertexCount + 3, x1, y0, adjustedDropShadowColor);

            m_indexBuffer[m_indexCount + 0] = m_vertexCount + 0;
            m_indexBuffer[m_indexCount + 1] = m_vertexCount + 1;
            m_indexBuffer[m_indexCount + 2] = m_vertexCount + 2;
            m_indexBuffer[m_indexCount + 3] = m_vertexCount + 0;
            m_indexBuffer[m_indexCount + 4] = m_vertexCount + 2;
            m_indexBuffer[m_indexCount + 5] = m_vertexCount + 3;
            m_vertexCount += 4;
            m_indexCount += 6;
        }

        m_penX += glyph->advance_x;
        if (m_penX > m_rectangle.width) {
            m_rectangle.width = m_penX;
        }

        if ((m_penY + m_lineAscender - m_lineDescender + m_lineGap) > m_rectangle.height) {
            m_rectangle.height = (m_penY + m_lineAscender - m_lineDescender + m_lineGap);
        }

        m_previousCodePoint = _codePoint;

        return;
    }

    if (m_styleFlags & STYLE_BACKGROUND
        && m_backgroundColor & 0xff000000) {
        float x0 = (m_penX - kerning);
        float y0 = (m_penY);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = (m_penY + m_lineAscender - m_lineDescender + m_lineGap);

        atlas->packUV(blackGlyph.regionIndex, (uint8_t *) m_vertexBuffer, sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u), sizeof(TextVertex));

        const uint16_t vertexCount = m_vertexCount;
        setVertex(vertexCount + 0, x0, y0, m_backgroundColor, STYLE_BACKGROUND);
        setVertex(vertexCount + 1, x0, y1, m_backgroundColor, STYLE_BACKGROUND);
        setVertex(vertexCount + 2, x1, y1, m_backgroundColor, STYLE_BACKGROUND);
        setVertex(vertexCount + 3, x1, y0, m_backgroundColor, STYLE_BACKGROUND);

        m_indexBuffer[m_indexCount + 0] = vertexCount + 0;
        m_indexBuffer[m_indexCount + 1] = vertexCount + 1;
        m_indexBuffer[m_indexCount + 2] = vertexCount + 2;
        m_indexBuffer[m_indexCount + 3] = vertexCount + 0;
        m_indexBuffer[m_indexCount + 4] = vertexCount + 2;
        m_indexBuffer[m_indexCount + 5] = vertexCount + 3;
        m_vertexCount += 4;
        m_indexCount += 6;
    }

    if (m_styleFlags & STYLE_UNDERLINE && m_underlineColor & 0xFF000000) {
        float x0 = (m_penX - kerning);
        float y0 = (m_penY + m_lineAscender - m_lineDescender * 0.5f);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = y0 + font.underlineThickness;

        atlas->packUV(blackGlyph.regionIndex, (uint8_t *) m_vertexBuffer, sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u), sizeof(TextVertex));

        setVertex(m_vertexCount + 0, x0, y0, m_underlineColor, STYLE_UNDERLINE);
        setVertex(m_vertexCount + 1, x0, y1, m_underlineColor, STYLE_UNDERLINE);
        setVertex(m_vertexCount + 2, x1, y1, m_underlineColor, STYLE_UNDERLINE);
        setVertex(m_vertexCount + 3, x1, y0, m_underlineColor, STYLE_UNDERLINE);

        m_indexBuffer[m_indexCount + 0] = m_vertexCount + 0;
        m_indexBuffer[m_indexCount + 1] = m_vertexCount + 1;
        m_indexBuffer[m_indexCount + 2] = m_vertexCount + 2;
        m_indexBuffer[m_indexCount + 3] = m_vertexCount + 0;
        m_indexBuffer[m_indexCount + 4] = m_vertexCount + 2;
        m_indexBuffer[m_indexCount + 5] = m_vertexCount + 3;
        m_vertexCount += 4;
        m_indexCount += 6;
    }

    if (m_styleFlags & STYLE_OVERLINE && m_overlineColor & 0xFF000000) {
        float x0 = (m_penX - kerning);
        float y0 = (m_penY);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = y0 + font.underlineThickness;

        m_fontManager->getAtlas()->packUV(blackGlyph.regionIndex, (uint8_t *) m_vertexBuffer,
                                          sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u),
                                          sizeof(TextVertex)
        );

        setVertex(m_vertexCount + 0, x0, y0, m_overlineColor, STYLE_OVERLINE);
        setVertex(m_vertexCount + 1, x0, y1, m_overlineColor, STYLE_OVERLINE);
        setVertex(m_vertexCount + 2, x1, y1, m_overlineColor, STYLE_OVERLINE);
        setVertex(m_vertexCount + 3, x1, y0, m_overlineColor, STYLE_OVERLINE);

        m_indexBuffer[m_indexCount + 0] = m_vertexCount + 0;
        m_indexBuffer[m_indexCount + 1] = m_vertexCount + 1;
        m_indexBuffer[m_indexCount + 2] = m_vertexCount + 2;
        m_indexBuffer[m_indexCount + 3] = m_vertexCount + 0;
        m_indexBuffer[m_indexCount + 4] = m_vertexCount + 2;
        m_indexBuffer[m_indexCount + 5] = m_vertexCount + 3;
        m_vertexCount += 4;
        m_indexCount += 6;
    }

    if (m_styleFlags & STYLE_STRIKE_THROUGH && m_strikeThroughColor & 0xFF000000) {
        float x0 = (m_penX - kerning);
        float y0 = (m_penY + 0.666667f * font.ascender);
        float x1 = ((float) x0 + (glyph->advance_x));
        float y1 = y0 + font.underlineThickness;

        atlas->packUV(blackGlyph.regionIndex, (uint8_t *) m_vertexBuffer, sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u), sizeof(TextVertex));
        setVertex(m_vertexCount + 0, x0, y0, m_strikeThroughColor, STYLE_STRIKE_THROUGH);
        setVertex(m_vertexCount + 1, x0, y1, m_strikeThroughColor, STYLE_STRIKE_THROUGH);
        setVertex(m_vertexCount + 2, x1, y1, m_strikeThroughColor, STYLE_STRIKE_THROUGH);
        setVertex(m_vertexCount + 3, x1, y0, m_strikeThroughColor, STYLE_STRIKE_THROUGH);

        m_indexBuffer[m_indexCount + 0] = m_vertexCount + 0;
        m_indexBuffer[m_indexCount + 1] = m_vertexCount + 1;
        m_indexBuffer[m_indexCount + 2] = m_vertexCount + 2;
        m_indexBuffer[m_indexCount + 3] = m_vertexCount + 0;
        m_indexBuffer[m_indexCount + 4] = m_vertexCount + 2;
        m_indexBuffer[m_indexCount + 5] = m_vertexCount + 3;
        m_vertexCount += 4;
        m_indexCount += 6;
    }

    if (!shadow && atlasRegion.getType() == AtlasRegion::TYPE_BGRA8) { bx::memSet(&m_vertexBuffer[m_vertexCount], 0, sizeof(TextVertex) * 4);

        atlas->packUV(glyph->regionIndex, (uint8_t *) m_vertexBuffer, sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u1), sizeof(TextVertex));

        float glyphScale = glyph->bitmapScale;
        float glyphWidth = glyph->width * glyphScale;
        float glyphHeight = glyph->height * glyphScale;
        float x0 = m_penX + (glyph->offset_x);
        float y0 = (m_penY + (font.ascender + -font.descender - glyphHeight) / 2);
        float x1 = (x0 + glyphWidth);
        float y1 = (y0 + glyphHeight);

        setVertex(m_vertexCount + 0, x0, y0, m_textColor);
        setVertex(m_vertexCount + 1, x0, y1, m_textColor);
        setVertex(m_vertexCount + 2, x1, y1, m_textColor);
        setVertex(m_vertexCount + 3, x1, y0, m_textColor);
    } else if (!shadow) {
        bx::memSet(&m_vertexBuffer[m_vertexCount], 0, sizeof(TextVertex) * 4);

        atlas->packUV(glyph->regionIndex, (uint8_t *) m_vertexBuffer, sizeof(TextVertex) * m_vertexCount + offsetof(TextVertex, u), sizeof(TextVertex));
        float x0 = m_penX + (glyph->offset_x);
        float y0 = (m_penY + m_lineAscender + (glyph->offset_y));
        float x1 = (x0 + glyph->width);
        float y1 = (y0 + glyph->height);

        setVertex(m_vertexCount + 0, x0, y0, m_textColor);
        setVertex(m_vertexCount + 1, x0, y1, m_textColor);
        setVertex(m_vertexCount + 2, x1, y1, m_textColor);
        setVertex(m_vertexCount + 3, x1, y0, m_textColor);

        setOutlineColor(m_vertexCount + 0, m_outlineColor);
        setOutlineColor(m_vertexCount + 1, m_outlineColor);
        setOutlineColor(m_vertexCount + 2, m_outlineColor);
        setOutlineColor(m_vertexCount + 3, m_outlineColor);
    }

    m_indexBuffer[m_indexCount + 0] = m_vertexCount + 0;
    m_indexBuffer[m_indexCount + 1] = m_vertexCount + 1;
    m_indexBuffer[m_indexCount + 2] = m_vertexCount + 2;
    m_indexBuffer[m_indexCount + 3] = m_vertexCount + 0;
    m_indexBuffer[m_indexCount + 4] = m_vertexCount + 2;
    m_indexBuffer[m_indexCount + 5] = m_vertexCount + 3;
    m_vertexCount += 4;
    m_indexCount += 6;

    m_penX += glyph->advance_x;
    if (m_penX > m_rectangle.width) {
        m_rectangle.width = m_penX;
    }

    if ((m_penY + m_lineAscender - m_lineDescender + m_lineGap) > m_rectangle.height) {
        m_rectangle.height = (m_penY + m_lineAscender - m_lineDescender + m_lineGap);
    }

    m_previousCodePoint = _codePoint;
}

void TextBuffer::verticalCenterLastLine(float _dy, float _top, float _bottom) {
    for (uint32_t ii = m_lineStartIndex; ii < m_vertexCount; ii += 4) {
        if (m_styleBuffer[ii] == STYLE_BACKGROUND) {
            m_vertexBuffer[ii + 0].y = _top;
            m_vertexBuffer[ii + 1].y = _bottom;
            m_vertexBuffer[ii + 2].y = _bottom;
            m_vertexBuffer[ii + 3].y = _top;
        } else {
            m_vertexBuffer[ii + 0].y += _dy;
            m_vertexBuffer[ii + 1].y += _dy;
            m_vertexBuffer[ii + 2].y += _dy;
            m_vertexBuffer[ii + 3].y += _dy;
        }
    }
}