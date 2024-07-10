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

struct TextRectangle {
    float width, height;
};

class TextBuffer {
public:

    /// TextBuffer is bound to a fontManager for glyph retrieval
    /// @remark the ownership of the manager is not taken
    explicit TextBuffer(FontManager *font_manager);

    ~TextBuffer();

    void SetPenPosition(float x, float y) {
        pen_x_ = x;
        pen_y_ = y;
    }

    /// Append an ASCII/utf-8 string to the buffer using current pen
    /// position and color.
    void AppendText(FontHandle font_handle, const char *string, const char *end = nullptr);

    /// Append a wide char unicode string to the buffer using current pen
    /// position and color.
    void AppendText(FontHandle font_handle, const wchar_t *string, const wchar_t *end = nullptr);

    /// Append a whole face of the atlas cube, mostly used for debugging
    /// and visualizing atlas.
    void AppendAtlasFace(uint16_t face_index);

    /// Clear the text buffer and reset its state (pen/color)
    void ClearTextBuffer();

    /// Get pointer to the vertex buffer to submit it to the graphic card.
    const uint8_t *GetVertexBuffer() {
        return (uint8_t *) vertex_buffer_;
    }

    /// Number of vertex in the vertex buffer.
    [[nodiscard]] uint32_t GetVertexCount() const {
        return vertex_count_;
    }

    /// Size in bytes of a vertex.
    [[nodiscard]] static uint32_t GetVertexSize() {
        return sizeof(TextVertex);
    }

    /// get a pointer to the index buffer to submit it to the graphic
    [[nodiscard]] const uint16_t *GetIndexBuffer() const {
        return index_buffer_;
    }

    /// number of index in the index buffer
    [[nodiscard]] uint32_t GetIndexCount() const {
        return index_count_;
    }

    /// Size in bytes of an index.
    [[nodiscard]] static uint32_t GetIndexSize() {
        return sizeof(uint16_t);
    }

    [[nodiscard]] uint32_t GetTextColor() const {
        return ToAbgr(text_color_);
    }

    [[nodiscard]] TextRectangle GetRectangle() const {
        return rectangle_;
    }

private:
    void AppendGlyph(FontHandle handle, CodePoint code_point, bool shadow);

    void VerticalCenterLastLine(float txt_decal_y, float top, float bottom);

    static uint32_t ToAbgr(uint32_t rgba) {
        return (((rgba >> 0) & 0xff) << 24)
               | (((rgba >> 8) & 0xff) << 16)
               | (((rgba >> 16) & 0xff) << 8)
               | (((rgba >> 24) & 0xff) << 0);
    }

    void SetVertex(uint32_t i, float x, float y, uint32_t rgba) {
        vertex_buffer_[i].x = x;
        vertex_buffer_[i].y = y;
        vertex_buffer_[i].rgba = rgba;
    }

    struct TextVertex {
        float x, y;
        int16_t u, v, w, t;
        int16_t u1, v1, w1, t1;
        int16_t u2, v2, w2, t2;
        uint32_t rgba;
        uint32_t rgba_outline;
    };

    // color states
    uint32_t text_color_;
    uint32_t background_color_;
    
    //position states
    float pen_x_;
    float pen_y_;

    float origin_x_;
    float origin_y_;

    float line_ascender_;
    float line_descender_;
    float line_gap_;

    CodePoint previous_code_point_;

    TextRectangle rectangle_{};
    FontManager *font_manager_;

    TextVertex *vertex_buffer_;
    uint16_t *index_buffer_;
    uint8_t *style_buffer_;

    uint32_t index_count_;
    uint32_t line_start_index_;
    uint16_t vertex_count_;
};
