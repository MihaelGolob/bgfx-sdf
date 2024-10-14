//
// Created by MihaelGolob on 22. 08. 2024.
//

#pragma once

#include <font_processing/FontHandles.h>

class FontManager;

class Window;

class GlyphErrorBenchmark {
public:
    GlyphErrorBenchmark(FontManager *font_manager, Window *window, TrueTypeHandle font_file_handle);
    ~GlyphErrorBenchmark();
    void RunBenchmark();

private:
    void Update();

    void GenerateGlyph();
    void CreateQuad();
    void InitializeShaders();
    void InitializeTextures();
    void AdjustQuadForGlyph(int glyph_width, int glyph_height);

    void WriteBufferToImageIfReady(uint32_t current_frame);

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

};
