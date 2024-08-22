//
// Created by MihaelGolob on 22. 08. 2024.
//

#include <utilities.h>
#include "GlyphPrecisionBenchmark.h"

GlyphPrecisionBenchmark::GlyphPrecisionBenchmark(FontManager *font_manager, TrueTypeHandle font_handle) :
        font_manager_(font_manager), font_handle_(font_handle) {
    render_texture_ = bgfx::createTexture2D(1024, 1024, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_READ_BACK);
    if (!bgfx::isValid(render_texture_)) PrintError("Failed to create render texture");
    
}

GlyphPrecisionBenchmark::~GlyphPrecisionBenchmark() {
    bgfx::destroy(render_texture_);
}


