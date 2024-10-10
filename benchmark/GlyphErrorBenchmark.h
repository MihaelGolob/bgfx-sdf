//
// Created by MihaelGolob on 22. 08. 2024.
//

#pragma once

#include <font_processing/FontHandles.h>

class FontManager;
class Window;

class GlyphErrorBenchmark {
public:
    GlyphErrorBenchmark(FontManager* font_manager, Window* window, TrueTypeHandle font_file_handle);
    ~GlyphErrorBenchmark();
    void RunBenchmark();
    
private:
    void Update();
    void GenerateGlyph();
    void WriteBufferToImageIfReady(uint32_t current_frame);
    
    FontManager* font_manager_;
    Window* window_;
    TrueTypeHandle font_file_handle_{};
    
    bgfx::TextureHandle render_texture_{};
    bgfx::TextureHandle frame_buffer_texture_{};
    bgfx::FrameBufferHandle frame_buffer_{};
    
    int texture_width_{1024};
    int texture_height_{1024};
    
    uint8_t* out_buffer_{};
    uint8_t* glyph_buffer_{};
    uint32_t read_frame_{0};
};
