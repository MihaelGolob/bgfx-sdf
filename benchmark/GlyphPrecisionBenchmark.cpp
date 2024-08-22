//
// Created by MihaelGolob on 22. 08. 2024.
//

#include "GlyphPrecisionBenchmark.h"

#include <utilities.h>
#include <window/Window.h>
#include <managers/FontManager.h>

GlyphPrecisionBenchmark::GlyphPrecisionBenchmark(FontManager *font_manager, Window* window, TrueTypeHandle font_handle) :
        font_manager_(font_manager), window_(window), font_handle_(font_handle) {
    
    render_texture_ = bgfx::createTexture2D(texture_width_, texture_height_, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);
    if (!bgfx::isValid(render_texture_)) PrintError("Failed to create render texture");
    
    frame_buffer_texture_ = bgfx::createTexture2D(texture_width_, texture_height_, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
    if (!bgfx::isValid(frame_buffer_texture_)) PrintError("Failed to create frame buffer texture");
    
    frame_buffer_ = bgfx::createFrameBuffer(1, &frame_buffer_texture_);
    
    window_->SetUpdateLoop([&]() { Update(); });
    window_->GetRenderer()->SetManualMode(true);
    
    out_buffer_ = new uint8_t[1024 * 1024 * 4];
    
    GenerateGlyph();
}

GlyphPrecisionBenchmark::~GlyphPrecisionBenchmark() {
    window_->GetRenderer()->SetManualMode(false);
    bgfx::destroy(render_texture_);
    delete out_buffer_;
}

void GlyphPrecisionBenchmark::GenerateGlyph() {
    
}

void GlyphPrecisionBenchmark::RunBenchmark() {
    window_->StartUpdate();
}

void GlyphPrecisionBenchmark::Update() {
    bgfx::setViewRect(0, 0, 0, bgfx::BackbufferRatio::Equal);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewFrameBuffer(0, frame_buffer_);
    bgfx::touch(0);
    
    bgfx::touch(1);
    bgfx::blit(1, render_texture_, 0, 0, frame_buffer_texture_, 0, 0, texture_width_, texture_height_);
    const auto read_frame = bgfx::readTexture(render_texture_, out_buffer_);
    
//    std::string message = "Render texture will be availalbe in frame number: ";
//    message.append(std::to_string(read_frame));
//    PrintInfo(message.c_str());

    current_frame_ = bgfx::frame();
}
