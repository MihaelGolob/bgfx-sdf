//
// Created by MihaelGolob on 22. 08. 2024.
//

#pragma once

#include <font_processing/FontHandles.h>

class FontManager;

class GlyphPrecisionBenchmark {
public:
    GlyphPrecisionBenchmark(FontManager* font_manager, TrueTypeHandle font_handle);
    ~GlyphPrecisionBenchmark();
    
private:
    FontManager* font_manager_;
    TrueTypeHandle font_handle_{};
    
    bgfx::TextureHandle render_texture_{};
};
