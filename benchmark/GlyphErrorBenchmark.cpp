//
// Created by MihaelGolob on 22. 08. 2024.
//
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "GlyphErrorBenchmark.h"

#include <utilities.h>
#include <window/Window.h>
#include <managers/FontManager.h>
#include <stb_image_write.h>

GlyphErrorBenchmark::GlyphErrorBenchmark(FontManager *font_manager, Window* window, TrueTypeHandle font_file_handle) :
        font_manager_(font_manager), window_(window), font_file_handle_(font_file_handle) {
    
    render_texture_ = bgfx::createTexture2D(texture_width_, texture_height_, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);
    if (!bgfx::isValid(render_texture_)) PrintError("Failed to create render texture");

    frame_buffer_texture_ = bgfx::createTexture2D(texture_width_, texture_height_, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
    if (!bgfx::isValid(frame_buffer_texture_)) PrintError("Failed to create frame buffer texture");

    frame_buffer_ = bgfx::createFrameBuffer(1, &frame_buffer_texture_);

    window_->SetUpdateLoop([&]() { Update(); });
    window_->GetRenderer()->SetManualMode(true);

    out_buffer_ = new uint8_t[1024 * 1024 * 4];
    glyph_buffer_ = new uint8_t[128 * 128];
    
    GenerateGlyph();
}

GlyphErrorBenchmark::~GlyphErrorBenchmark() {
    window_->GetRenderer()->SetManualMode(false);
    bgfx::destroy(render_texture_);
    delete[] out_buffer_;
    delete[] glyph_buffer_;
}

void GlyphErrorBenchmark::GenerateGlyph() {
    const auto font = font_manager_->CreateFontByPixelSize(font_file_handle_, 0, 100, FontType::Bitmap, 0);
    AtlasRegion::Type atlas_type;
    GlyphInfo info{};
    font_manager_->GenerateGlyph(font, 'A', glyph_buffer_, atlas_type, info);
    font_manager_->DestroyFont(font);

    stbi_write_png("glyph.png", info.width, info.height, 1, glyph_buffer_, info.width);
}

void GlyphErrorBenchmark::RunBenchmark() {
    window_->StartUpdate();
}

void GlyphErrorBenchmark::Update() {
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewFrameBuffer(0, frame_buffer_);
    bgfx::touch(0);
    
    bgfx::touch(1);
    bgfx::blit(1, render_texture_, 0, 0, frame_buffer_texture_, 0, 0, texture_width_, texture_height_);

    const auto current_frame = bgfx::frame();
    WriteBufferToImageIfReady(current_frame);
}

void GlyphErrorBenchmark::WriteBufferToImageIfReady(uint32_t current_frame) {
    if (read_frame_ == 0) {
        read_frame_ = bgfx::readTexture(render_texture_, out_buffer_);
        return;
    }
    if (read_frame_ == 0 && current_frame < read_frame_) return; 
    
//    stbi_flip_vertically_on_write(1);
    stbi_write_png("render_texture.png", texture_width_, texture_height_, 4, out_buffer_, texture_width_*4);
    
    read_frame_ = 0;
}
