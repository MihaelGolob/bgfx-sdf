//
// Created by MihaelGolob on 30. 04. 2024.
//

#pragma once

#include "NumberTypes.h"
#include "FontHandles.h"

class FontManager;

typedef int CodePoint;

/// type of vertex and index buffer to use with a TextBuffer
struct BufferType {
    enum Enum {
        Static,
        Dynamic,
        Transient,
    };
};

/// special style effect (can be combined)
enum TextStyleFlags {
    STYLE_NORMAL = 0,
    STYLE_OVERLINE = 1,
    STYLE_UNDERLINE = 1 << 1,
    STYLE_STRIKE_THROUGH = 1 << 2,
    STYLE_BACKGROUND = 1 << 3,
};

struct TextRectangle {
    float width, height;
};

class TextBuffer {
public:

    /// TextBuffer is bound to a fontManager for glyph retrieval
    /// @remark the ownership of the manager is not taken
    TextBuffer(FontManager *_fontManager);

    ~TextBuffer();

    void setPenPosition(float _x, float _y) {
        m_penX = _x;
        m_penY = _y;
    }

    /// Append an ASCII/utf-8 string to the buffer using current pen
    /// position and color.
    void appendText(FontHandle _fontHandle, const char *_string, const char *_end = nullptr);

    /// Append a wide char unicode string to the buffer using current pen
    /// position and color.
    void appendText(FontHandle _fontHandle, const wchar_t *_string, const wchar_t *_end = nullptr);

    /// Append a whole face of the atlas cube, mostly used for debugging
    /// and visualizing atlas.
    void appendAtlasFace(uint16_t _faceIndex);

    /// Clear the text buffer and reset its state (pen/color)
    void clearTextBuffer();

    /// Get pointer to the vertex buffer to submit it to the graphic card.
    const uint8_t *getVertexBuffer() {
        return (uint8_t *) m_vertexBuffer;
    }

    /// Number of vertex in the vertex buffer.
    uint32_t getVertexCount() const {
        return m_vertexCount;
    }

    /// Size in bytes of a vertex.
    uint32_t getVertexSize() const {
        return sizeof(TextVertex);
    }

    /// get a pointer to the index buffer to submit it to the graphic
    const uint16_t *getIndexBuffer() const {
        return m_indexBuffer;
    }

    /// number of index in the index buffer
    uint32_t getIndexCount() const {
        return m_indexCount;
    }

    /// Size in bytes of an index.
    uint32_t getIndexSize() const {
        return sizeof(uint16_t);
    }

    uint32_t getTextColor() const {
        return toABGR(m_textColor);
    }

    TextRectangle getRectangle() const {
        return m_rectangle;
    }

private:
    void appendGlyph(FontHandle _handle, CodePoint _codePoint, bool shadow);

    void verticalCenterLastLine(float _txtDecalY, float _top, float _bottom);

    static uint32_t toABGR(uint32_t _rgba) {
        return (((_rgba >> 0) & 0xff) << 24)
               | (((_rgba >> 8) & 0xff) << 16)
               | (((_rgba >> 16) & 0xff) << 8)
               | (((_rgba >> 24) & 0xff) << 0);
    }

    void setVertex(uint32_t _i, float _x, float _y, uint32_t _rgba, uint8_t _style = STYLE_NORMAL) {
        m_vertexBuffer[_i].x = _x;
        m_vertexBuffer[_i].y = _y;
        m_vertexBuffer[_i].rgba = _rgba;
        m_styleBuffer[_i] = _style;
    }

    void setOutlineColor(uint32_t _i, uint32_t _rgbaOutline) {
        m_vertexBuffer[_i].rgbaOutline = _rgbaOutline;
    }

    struct TextVertex {
        float x, y;
        int16_t u, v, w, t;
        int16_t u1, v1, w1, t1;
        int16_t u2, v2, w2, t2;
        uint32_t rgba;
        uint32_t rgbaOutline;
    };

    uint32_t m_styleFlags;

    // color states
    uint32_t m_textColor;

    uint32_t m_backgroundColor;
    uint32_t m_overlineColor;
    uint32_t m_underlineColor;
    uint32_t m_strikeThroughColor;

    // outline state
    float m_outlineWidth;
    uint32_t m_outlineColor;

    // drop shadow state
    float m_dropShadowOffset[2];
    uint32_t m_dropShadowColor;
    float m_dropShadowSoftener;

    //position states
    float m_penX;
    float m_penY;

    float m_originX;
    float m_originY;

    float m_lineAscender;
    float m_lineDescender;
    float m_lineGap;

    CodePoint m_previousCodePoint;

    TextRectangle m_rectangle;
    FontManager *m_fontManager;

    TextVertex *m_vertexBuffer;
    uint16_t *m_indexBuffer;
    uint8_t *m_styleBuffer;

    uint32_t m_indexCount;
    uint32_t m_lineStartIndex;
    uint16_t m_vertexCount;
};
