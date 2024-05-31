//
// Created by MihaelGolob on 22. 04. 2024.
//

/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include <bgfx/embedded_shader.h>

#include "TextBufferManager.h"
#include "../FontAtlas/CubeAtlas.h"

// shaders
#include "../shaders/vertex/vs_font_basic.bin.h"
#include "../shaders/fragment/fs_font_basic.bin.h"
#include "../shaders/fragment/fs_font_sdf.bin.h"

static const bgfx::EmbeddedShader s_embeddedShaders[] =
        {
                BGFX_EMBEDDED_SHADER(vs_font_basic),
                BGFX_EMBEDDED_SHADER(fs_font_basic),
                BGFX_EMBEDDED_SHADER(fs_font_sdf),
                BGFX_EMBEDDED_SHADER_END()
        };

TextBufferManager::TextBufferManager(FontManager *_fontManager) : m_fontManager(_fontManager) {
    m_textBuffers = new BufferCache[MAX_TEXT_BUFFER_COUNT];

    bgfx::RendererType::Enum type = bgfx::getRendererType();

    // create shader programs
    m_basicProgram = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_font_basic"),
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_font_basic"), true
    );
    
    m_sdfProgram = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "vs_font_basic"),
            bgfx::createEmbeddedShader(s_embeddedShaders, type, "fs_font_sdf"), true
    );

    m_vertexLayout
            .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Int16, true)
            .add(bgfx::Attrib::TexCoord1, 4, bgfx::AttribType::Int16, true)
            .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Int16, true)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
            .end();

    s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
}

TextBufferManager::~TextBufferManager() {
    BX_ASSERT(
            m_textBufferHandles.getNumHandles() == 0,
            "All the text buffers must be destroyed before destroying the manager"
    );
    delete[] m_textBuffers;

    bgfx::destroy(s_texColor);

    // destroy shader programs
    bgfx::destroy(m_basicProgram);
    bgfx::destroy(m_sdfProgram);
}

TextBufferHandle TextBufferManager::createTextBuffer(FontType _type, BufferType::Enum _bufferType) {
    uint16_t textIdx = m_textBufferHandles.alloc();
    BufferCache &bc = m_textBuffers[textIdx];

    bc.textBuffer = new TextBuffer(m_fontManager);
    bc.fontType = _type;
    bc.bufferType = _bufferType;
    bc.indexBufferHandleIdx = bgfx::kInvalidHandle;
    bc.vertexBufferHandleIdx = bgfx::kInvalidHandle;

    TextBufferHandle ret = {textIdx};
    return ret;
}

void TextBufferManager::destroyTextBuffer(TextBufferHandle _handle) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");

    BufferCache &bc = m_textBuffers[_handle.idx];
    m_textBufferHandles.free(_handle.idx);
    delete bc.textBuffer;
    bc.textBuffer = NULL;

    if (bc.vertexBufferHandleIdx == bgfx::kInvalidHandle) {
        return;
    }

    switch (bc.bufferType) {
        case BufferType::Static: {
            bgfx::IndexBufferHandle ibh;
            bgfx::VertexBufferHandle vbh;
            ibh.idx = bc.indexBufferHandleIdx;
            vbh.idx = bc.vertexBufferHandleIdx;
            bgfx::destroy(ibh);
            bgfx::destroy(vbh);
        }

            break;

        case BufferType::Dynamic:
            bgfx::DynamicIndexBufferHandle ibh;
            bgfx::DynamicVertexBufferHandle vbh;
            ibh.idx = bc.indexBufferHandleIdx;
            vbh.idx = bc.vertexBufferHandleIdx;
            bgfx::destroy(ibh);
            bgfx::destroy(vbh);

            break;

        case BufferType::Transient: // destroyed every frame
            break;
    }
}

void TextBufferManager::submitTextBuffer(TextBufferHandle _handle, bgfx::ViewId _id, int32_t _depth) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");

    BufferCache &bc = m_textBuffers[_handle.idx];

    uint32_t indexSize = bc.textBuffer->getIndexCount() * bc.textBuffer->getIndexSize();
    uint32_t vertexSize = bc.textBuffer->getVertexCount() * bc.textBuffer->getVertexSize();

    if (0 == indexSize || 0 == vertexSize) {
        return;
    }

    bgfx::setTexture(0, s_texColor, m_fontManager->getAtlas()->getTextureHandle());

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
    // todo mihael: add support for other font types (sdf, msdf, ...)
    // Load the correct shader program based on the font type
    switch (bc.fontType) {
        case FontType::Bitmap:
            program = m_basicProgram;
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
            break;
        case FontType::SDF: {
            program = m_sdfProgram;
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
            break;
        }
        default: 
            BX_ASSERT(false, "Shader for font type %d not found", bc.fontType);
            break;
    }

    switch (bc.bufferType) {
        case BufferType::Static: {
            bgfx::IndexBufferHandle ibh;
            bgfx::VertexBufferHandle vbh;

            if (bgfx::kInvalidHandle == bc.vertexBufferHandleIdx) {
                ibh = bgfx::createIndexBuffer(
                        bgfx::copy(bc.textBuffer->getIndexBuffer(), indexSize)
                );

                vbh = bgfx::createVertexBuffer(
                        bgfx::copy(bc.textBuffer->getVertexBuffer(), vertexSize), m_vertexLayout
                );

                bc.vertexBufferHandleIdx = vbh.idx;
                bc.indexBufferHandleIdx = ibh.idx;
            } else {
                vbh.idx = bc.vertexBufferHandleIdx;
                ibh.idx = bc.indexBufferHandleIdx;
            }

            bgfx::setVertexBuffer(0, vbh, 0, bc.textBuffer->getVertexCount());
            bgfx::setIndexBuffer(ibh, 0, bc.textBuffer->getIndexCount());
        }
            break;

        case BufferType::Dynamic: {
            bgfx::DynamicIndexBufferHandle ibh;
            bgfx::DynamicVertexBufferHandle vbh;

            if (bgfx::kInvalidHandle == bc.vertexBufferHandleIdx) {
                ibh = bgfx::createDynamicIndexBuffer(
                        bgfx::copy(bc.textBuffer->getIndexBuffer(), indexSize)
                );

                vbh = bgfx::createDynamicVertexBuffer(
                        bgfx::copy(bc.textBuffer->getVertexBuffer(), vertexSize), m_vertexLayout
                );

                bc.indexBufferHandleIdx = ibh.idx;
                bc.vertexBufferHandleIdx = vbh.idx;
            } else {
                ibh.idx = bc.indexBufferHandleIdx;
                vbh.idx = bc.vertexBufferHandleIdx;

                bgfx::update(
                        ibh, 0, bgfx::copy(bc.textBuffer->getIndexBuffer(), indexSize)
                );

                bgfx::update(
                        vbh, 0, bgfx::copy(bc.textBuffer->getVertexBuffer(), vertexSize)
                );
            }

            bgfx::setVertexBuffer(0, vbh, 0, bc.textBuffer->getVertexCount());
            bgfx::setIndexBuffer(ibh, 0, bc.textBuffer->getIndexCount());
        }
            break;

        case BufferType::Transient: {
            bgfx::TransientIndexBuffer tib;
            bgfx::TransientVertexBuffer tvb;
            bgfx::allocTransientIndexBuffer(&tib, bc.textBuffer->getIndexCount());
            bgfx::allocTransientVertexBuffer(&tvb, bc.textBuffer->getVertexCount(), m_vertexLayout);
            bx::memCopy(tib.data, bc.textBuffer->getIndexBuffer(), indexSize);
            bx::memCopy(tvb.data, bc.textBuffer->getVertexBuffer(), vertexSize);
            bgfx::setVertexBuffer(0, &tvb, 0, bc.textBuffer->getVertexCount());
            bgfx::setIndexBuffer(&tib, 0, bc.textBuffer->getIndexCount());
        }
            break;
    }

    bgfx::submit(_id, program, _depth);
}

void TextBufferManager::setPenPosition(TextBufferHandle _handle, float _x, float _y) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");
    BufferCache &bc = m_textBuffers[_handle.idx];
    bc.textBuffer->setPenPosition(_x, _y);
}

void TextBufferManager::appendText(TextBufferHandle _handle, FontHandle _fontHandle, const char *_string, const char *_end) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");
    BufferCache &bc = m_textBuffers[_handle.idx];
    bc.textBuffer->appendText(_fontHandle, _string, _end);
}

void TextBufferManager::appendText(TextBufferHandle _handle, FontHandle _fontHandle, const wchar_t *_string,
                                   const wchar_t *_end) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");
    BufferCache &bc = m_textBuffers[_handle.idx];
    bc.textBuffer->appendText(_fontHandle, _string, _end);
}

void TextBufferManager::appendAtlasFace(TextBufferHandle _handle, uint16_t _faceIndex) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");
    BufferCache &bc = m_textBuffers[_handle.idx];
    bc.textBuffer->appendAtlasFace(_faceIndex);
}

void TextBufferManager::clearTextBuffer(TextBufferHandle _handle) {
    BX_ASSERT(isValid(_handle), "Invalid handle used");
    BufferCache &bc = m_textBuffers[_handle.idx];
    bc.textBuffer->clearTextBuffer();
}

TextRectangle TextBufferManager::getRectangle(TextBufferHandle _handle) const {
    BX_ASSERT(isValid(_handle), "Invalid handle used");
    BufferCache &bc = m_textBuffers[_handle.idx];
    return bc.textBuffer->getRectangle();
}

