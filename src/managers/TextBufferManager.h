//
// Created by MihaelGolob on 22. 04. 2024.
//

/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */
#pragma once

#include "FontManager.h"
#include "../font_processing/TextBuffer.h"
#include "../font_processing/FontInfo.h"

BGFX_HANDLE(TextBufferHandle)

#define MAX_TEXT_BUFFER_COUNT 64

class TextBuffer;

class TextBufferManager {
public:
    explicit TextBufferManager(FontManager *font_manager);

    ~TextBufferManager();

    TextBufferHandle CreateTextBuffer(FontType type, BufferType::Enum buffer_type);

    void DestroyTextBuffer(TextBufferHandle handle);

    void SubmitTextBuffer(TextBufferHandle handle, bgfx::ViewId id, int32_t depth = 0);

    void SetPenPosition(TextBufferHandle handle, float x, float y);

    /// Append an ASCII/utf-8 string to the buffer using current pen position and color.
    void AppendText(TextBufferHandle handle, FontHandle font_handle, const char *string, const char *end = nullptr);

    /// Append a wide char unicode string to the buffer using current pen position and color.
    void
    AppendText(TextBufferHandle handle, FontHandle font_handle, const wchar_t *string, const wchar_t *end = nullptr);

    /// Append a whole face of the atlas cube, mostly used for debugging and visualizing atlas.
    void AppendAtlasFace(TextBufferHandle handle, uint16_t face_index);

    /// Clear the text buffer and reset its state (pen/color).
    void ClearTextBuffer(TextBufferHandle handle);

    /// Return the rectangular size of the current text buffer (including all its content).
    [[nodiscard]] TextRectangle GetRectangle(TextBufferHandle handle) const;

private:
    struct BufferCache {
        uint16_t index_buffer_handle_idx;
        uint16_t vertex_buffer_handle_idx;
        TextBuffer *text_buffer;
        BufferType::Enum buffer_type;
        FontType font_type;
    };

    BufferCache *text_buffers_;
    bx::HandleAllocT<MAX_TEXT_BUFFER_COUNT> text_buffer_handles_;
    FontManager *font_manager_;
    bgfx::VertexLayout vertex_layout_;
    bgfx::UniformHandle tex_color_{};
    bgfx::ProgramHandle basic_program_{};
    bgfx::ProgramHandle sdf_program_{};
    bgfx::ProgramHandle msdf_program_{};
    bgfx::ProgramHandle color_program_{};
};
