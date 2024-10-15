//
// Created by MihaelGolob on 22. 08. 2024.
//

#pragma once

#include <font_processing/FontHandles.h>
#include "font_processing/FontInfo.h"
#include <managers/FontManager.h>

class FontManager;

class Window;

class GlyphErrorBenchmark {
public:
    GlyphErrorBenchmark(FontManager *font_manager, Window *window, TrueTypeHandle font_file_handle);
    ~GlyphErrorBenchmark();
    void RunBenchmark();

private:
    void GenerateCurrentGlyph();
    void GenerateGlyph(FontType font_type, CodePoint code_point, int font_size, float scale);
    void CreateQuad();
    void InitializeShaders(FontType font_type);
    void InitializeTextures();
    void AdjustQuadForGlyph(int glyph_width, int glyph_height, int padding, float scale);

    void Update();
    void Render(int context);
    void NextState();

    bool WriteBufferToImageIfReady(uint32_t current_frame);

    FontManager *font_manager_;
    Window *window_;
    TrueTypeHandle font_file_handle_{};

    bgfx::TextureHandle render_texture_{};
    bgfx::TextureHandle target_texture_{};
    bgfx::TextureHandle glyph_texture_{};
    bgfx::FrameBufferHandle frame_buffer_{};

    bgfx::VertexBufferHandle vb_{};
    bgfx::IndexBufferHandle ib_{};
    bgfx::UniformHandle tex_color_uniform_{};

    int texture_width_{1024};
    int texture_height_{1024};

    uint8_t *out_buffer_{};
    uint8_t *glyph_buffer_{};
    uint32_t read_frame_{0};
    bool is_frame_read_{false};
    bool ready_to_read_texture_{true};

    // shader programs
    bgfx::ProgramHandle basic_program_{};

    struct PosTexCoordVertex {
        float x, y, z;
        float u, v;
    };

    PosTexCoordVertex quad_vertices_[4] = {
            {-1.0f, 1.0f,  0, 0.0f, 0.0f}, // Top-left
            {1.0f,  1.0f,  0, 1.0f, 0.0f}, // Top-right
            {-1.0f, -1.0f, 0, 0.0f, 1.0f}, // Bottom-left
            {1.0f,  -1.0f, 0, 1.0f, 1.0f}, // Bottom-right
    };

    const uint16_t quad_indices_[6] = {
            0, 1, 2,
            1, 3, 2,
    };

    // benchmark settings
    std::vector<FontType> font_types_{};
    std::vector<char> code_points_{};
    std::vector<int> font_sizes_{};
    std::vector<float> font_scales_{};

    // hierachy: font -> character -> sizes -> scales

    bool done_{false};
    int current_font_type_{0};
    int current_code_point_{0};
    int current_font_size_{0};
    int current_font_scale_{0};
};
