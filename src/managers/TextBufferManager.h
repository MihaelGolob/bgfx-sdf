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
    explicit TextBufferManager(FontManager *_fontManager);

    ~TextBufferManager();

    TextBufferHandle createTextBuffer(FontType _type, BufferType::Enum _bufferType);

    void destroyTextBuffer(TextBufferHandle _handle);

    void submitTextBuffer(TextBufferHandle _handle, bgfx::ViewId _id, int32_t _depth = 0);

    void setPenPosition(TextBufferHandle _handle, float _x, float _y);

    /// Append an ASCII/utf-8 string to the buffer using current pen position and color.
    void appendText(TextBufferHandle _handle, FontHandle _fontHandle, const char *_string, const char *_end = nullptr);

    /// Append a wide char unicode string to the buffer using current pen position and color.
    void
    appendText(TextBufferHandle _handle, FontHandle _fontHandle, const wchar_t *_string, const wchar_t *_end = nullptr);

    /// Append a whole face of the atlas cube, mostly used for debugging and visualizing atlas.
    void appendAtlasFace(TextBufferHandle _handle, uint16_t _faceIndex);

    /// Clear the text buffer and reset its state (pen/color).
    void clearTextBuffer(TextBufferHandle _handle);

    /// Return the rectangular size of the current text buffer (including all its content).
    [[nodiscard]] TextRectangle getRectangle(TextBufferHandle _handle) const;

private:
    struct BufferCache {
        uint16_t indexBufferHandleIdx;
        uint16_t vertexBufferHandleIdx;
        TextBuffer *textBuffer;
        BufferType::Enum bufferType;
        FontType fontType;
    };

    BufferCache *m_textBuffers;
    bx::HandleAllocT<MAX_TEXT_BUFFER_COUNT> m_textBufferHandles;
    FontManager *m_fontManager;
    bgfx::VertexLayout m_vertexLayout;
    bgfx::UniformHandle s_texColor{};
    bgfx::ProgramHandle m_basicProgram{};
    bgfx::ProgramHandle m_sdfProgram{};
};
