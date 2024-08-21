//
// Created by MihaelGolob on 22. 04. 2024.
//

/*
 * Copyright 2013 Jeremie Roy. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx/blob/master/LICENSE
 */

#include <bgfx/embedded_shader.h>

#include "TextBufferManager.h"
#include "../font_processing/CubeAtlas.h"

// shaders
#include "../shaders/vertex/vs_font_basic.bin.h"
#include "../shaders/fragment/fs_font_basic.bin.h"
#include "../shaders/fragment/fs_font_sdf.bin.h"
#include "../shaders/fragment/fs_font_msdf.bin.h"
#include "../shaders/fragment/fs_color.bin.h"

static const bgfx::EmbeddedShader s_embedded_shaders_[] =
        {
                BGFX_EMBEDDED_SHADER(vs_font_basic),
                BGFX_EMBEDDED_SHADER(fs_font_basic),
                BGFX_EMBEDDED_SHADER(fs_font_sdf),
                BGFX_EMBEDDED_SHADER(fs_font_msdf),
                BGFX_EMBEDDED_SHADER(fs_color),
                BGFX_EMBEDDED_SHADER_END()
        };

TextBufferManager::TextBufferManager(FontManager *font_manager) : font_manager_(font_manager) {
    text_buffers_ = new BufferCache[MAX_TEXT_BUFFER_COUNT];

    bgfx::RendererType::Enum type = bgfx::getRendererType();

    // create shader programs
    basic_program_ = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "vs_font_basic"),
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "fs_font_basic"), true
    );

    sdf_program_ = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "vs_font_basic"),
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "fs_font_sdf"), true
    );

    msdf_program_ = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "vs_font_basic"),
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "fs_font_msdf"), true
    );

    color_program_ = bgfx::createProgram(
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "vs_font_basic"),
            bgfx::createEmbeddedShader(s_embedded_shaders_, type, "fs_color"), true
    );

    vertex_layout_
            .begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Int16, true)
            .add(bgfx::Attrib::TexCoord1, 4, bgfx::AttribType::Int16, true)
            .add(bgfx::Attrib::TexCoord2, 4, bgfx::AttribType::Int16, true)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .add(bgfx::Attrib::Color1, 4, bgfx::AttribType::Uint8, true)
            .end();

    tex_color_ = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
}

TextBufferManager::~TextBufferManager() {
    BX_ASSERT(text_buffer_handles_.getNumHandles() == 0, "All the text buffers must be destroyed before destroying the manager")
    delete[] text_buffers_;

    bgfx::destroy(tex_color_);

    // destroy shader programs
    bgfx::destroy(basic_program_);
    bgfx::destroy(sdf_program_);
    bgfx::destroy(msdf_program_);
    bgfx::destroy(color_program_);
}

TextBufferHandle TextBufferManager::CreateTextBuffer(FontType type, BufferType::Enum buffer_type) {
    uint16_t text_idx = text_buffer_handles_.alloc();
    BufferCache &bc = text_buffers_[text_idx];

    bc.text_buffer = new TextBuffer(font_manager_);
    bc.font_type = type;
    bc.buffer_type = buffer_type;
    bc.index_buffer_handle_idx = bgfx::kInvalidHandle;
    bc.vertex_buffer_handle_idx = bgfx::kInvalidHandle;

    TextBufferHandle ret = {text_idx};
    return ret;
}

void TextBufferManager::DestroyTextBuffer(TextBufferHandle handle) {
    BX_ASSERT(isValid(handle), "Invalid handle used")

    BufferCache &bc = text_buffers_[handle.idx];
    text_buffer_handles_.free(handle.idx);
    delete bc.text_buffer;
    bc.text_buffer = nullptr;

    if (bc.vertex_buffer_handle_idx == bgfx::kInvalidHandle) {
        return;
    }

    switch (bc.buffer_type) {
        case BufferType::Static: {
            bgfx::IndexBufferHandle ibh {};
            bgfx::VertexBufferHandle vbh {};
            ibh.idx = bc.index_buffer_handle_idx;
            vbh.idx = bc.vertex_buffer_handle_idx;
            bgfx::destroy(ibh);
            bgfx::destroy(vbh);
            
            break;
        }
        case BufferType::Dynamic: {
            bgfx::DynamicIndexBufferHandle ibh {};
            bgfx::DynamicVertexBufferHandle vbh {};
            ibh.idx = bc.index_buffer_handle_idx;
            vbh.idx = bc.vertex_buffer_handle_idx;
            bgfx::destroy(ibh);
            bgfx::destroy(vbh);

            break;
        }
        case BufferType::Transient: // destroyed every frame
            break;
    }
}

void TextBufferManager::SubmitTextBuffer(TextBufferHandle handle, bgfx::ViewId id, int32_t depth) {
    BX_ASSERT(isValid(handle), "Invalid handle used")

    BufferCache &bc = text_buffers_[handle.idx];

    uint32_t index_size = bc.text_buffer->GetIndexCount() * TextBuffer::GetIndexSize();
    uint32_t vertex_size = bc.text_buffer->GetVertexCount() * TextBuffer::GetVertexSize();

    if (0 == index_size || 0 == vertex_size) {
        return;
    }

    bgfx::setTexture(0, tex_color_, font_manager_->GetAtlas()->GetTextureHandle());

    bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
    // Load the correct shader program based on the font type
    switch (bc.font_type) {
        case FontType::Bitmap:
            program = basic_program_;
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
            break;
        case FontType::SdfFromVector:
        case FontType::SdfFromBitmap: {
            program = sdf_program_;
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
            break;
        }
        case FontType::MsdfOriginal:
        case FontType::Msdf: {
            program = msdf_program_;
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
            break;
        }
        case FontType::Color: {
            program = color_program_;
            bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
            break;
        }
        default: 
            BX_ASSERT(false, "Shader for font type %d not found", bc.font_type)
            break;
    }

    switch (bc.buffer_type) {
        case BufferType::Static: {
            bgfx::IndexBufferHandle ibh {};
            bgfx::VertexBufferHandle vbh {};

            if (bgfx::kInvalidHandle == bc.vertex_buffer_handle_idx) {
                ibh = bgfx::createIndexBuffer(bgfx::copy(bc.text_buffer->GetIndexBuffer(), index_size));
                vbh = bgfx::createVertexBuffer(bgfx::copy(bc.text_buffer->GetVertexBuffer(), vertex_size), vertex_layout_);

                bc.vertex_buffer_handle_idx = vbh.idx;
                bc.index_buffer_handle_idx = ibh.idx;
            } else {
                vbh.idx = bc.vertex_buffer_handle_idx;
                ibh.idx = bc.index_buffer_handle_idx;
            }

            bgfx::setVertexBuffer(0, vbh, 0, bc.text_buffer->GetVertexCount());
            bgfx::setIndexBuffer(ibh, 0, bc.text_buffer->GetIndexCount());
        }
            break;

        case BufferType::Dynamic: {
            bgfx::DynamicIndexBufferHandle ibh {};
            bgfx::DynamicVertexBufferHandle vbh {};

            if (bgfx::kInvalidHandle == bc.vertex_buffer_handle_idx) {
                ibh = bgfx::createDynamicIndexBuffer(bgfx::copy(bc.text_buffer->GetIndexBuffer(), index_size));
                vbh = bgfx::createDynamicVertexBuffer(bgfx::copy(bc.text_buffer->GetVertexBuffer(), vertex_size), vertex_layout_);

                bc.index_buffer_handle_idx = ibh.idx;
                bc.vertex_buffer_handle_idx = vbh.idx;
            } else {
                ibh.idx = bc.index_buffer_handle_idx;
                vbh.idx = bc.vertex_buffer_handle_idx;

                bgfx::update(ibh, 0, bgfx::copy(bc.text_buffer->GetIndexBuffer(), index_size));
                bgfx::update(vbh, 0, bgfx::copy(bc.text_buffer->GetVertexBuffer(), vertex_size));
            }

            bgfx::setVertexBuffer(0, vbh, 0, bc.text_buffer->GetVertexCount());
            bgfx::setIndexBuffer(ibh, 0, bc.text_buffer->GetIndexCount());
        }
            break;

        case BufferType::Transient: {
            bgfx::TransientIndexBuffer tib {};
            bgfx::TransientVertexBuffer tvb {};
            bgfx::allocTransientIndexBuffer(&tib, bc.text_buffer->GetIndexCount());
            bgfx::allocTransientVertexBuffer(&tvb, bc.text_buffer->GetVertexCount(), vertex_layout_);
            bx::memCopy(tib.data, bc.text_buffer->GetIndexBuffer(), index_size);
            bx::memCopy(tvb.data, bc.text_buffer->GetVertexBuffer(), vertex_size);
            bgfx::setVertexBuffer(0, &tvb, 0, bc.text_buffer->GetVertexCount());
            bgfx::setIndexBuffer(&tib, 0, bc.text_buffer->GetIndexCount());
        }
            break;
    }

    bgfx::submit(id, program, depth);
}

void TextBufferManager::SetPenPosition(TextBufferHandle handle, float x, float y) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    BufferCache &bc = text_buffers_[handle.idx];
    bc.text_buffer->SetPenPosition(x, y);
}

void TextBufferManager::AppendText(TextBufferHandle handle, FontHandle font_handle, const char *string, const char *end) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    BufferCache &bc = text_buffers_[handle.idx];
    bc.text_buffer->AppendText(font_handle, string, end);
}

void TextBufferManager::AppendText(TextBufferHandle handle, FontHandle font_handle, const wchar_t *string,
                                   const wchar_t *end) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    BufferCache &bc = text_buffers_[handle.idx];
    bc.text_buffer->AppendText(font_handle, string, end);
}

void TextBufferManager::AppendAtlasFace(TextBufferHandle handle, uint16_t face_index) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    BufferCache &bc = text_buffers_[handle.idx];
    bc.text_buffer->AppendAtlasFace(face_index);
}

void TextBufferManager::ClearTextBuffer(TextBufferHandle handle) {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    BufferCache &bc = text_buffers_[handle.idx];
    bc.text_buffer->ClearTextBuffer();
}

TextRectangle TextBufferManager::GetRectangle(TextBufferHandle handle) const {
    BX_ASSERT(isValid(handle), "Invalid handle used")
    BufferCache &bc = text_buffers_[handle.idx];
    return bc.text_buffer->GetRectangle();
}

