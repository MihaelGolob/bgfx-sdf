//
// Created by MihaelGolob on 22. 08. 2024.
//
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "GlyphErrorBenchmark.h"
#include "bgfx/embedded_shader.h"

#include <utilities.h>
#include <window/Window.h>
#include <stb_image_write.h>

// shaders
#include "../src/shaders/vertex/vs_font_basic.bin.h"
#include "../src/shaders/vertex/vs_font_basic_bench.bin.h"
#include "../src/shaders/fragment/fs_font_basic.bin.h"
#include "../src/shaders/fragment/fs_font_sdf.bin.h"
#include "../src/shaders/fragment/fs_font_sdf_bench.bin.h"
#include "../src/shaders/fragment/fs_font_msdf.bin.h"
#include "../src/shaders/fragment/fs_font_msdf_bench.bin.h"
#include "../src/shaders/fragment/fs_color.bin.h"
#include "../src/shaders/fragment/fs_font_basic_bench.bin.h"

static const bgfx::EmbeddedShader embedded_shaders_[] = {
        BGFX_EMBEDDED_SHADER(vs_font_basic),
        BGFX_EMBEDDED_SHADER(vs_font_basic_bench),
        BGFX_EMBEDDED_SHADER(fs_font_basic),
        BGFX_EMBEDDED_SHADER(fs_font_basic_bench),
        BGFX_EMBEDDED_SHADER(fs_font_sdf),
        BGFX_EMBEDDED_SHADER(fs_font_sdf_bench),
        BGFX_EMBEDDED_SHADER(fs_font_msdf),
        BGFX_EMBEDDED_SHADER(fs_font_msdf_bench),
        BGFX_EMBEDDED_SHADER(fs_color),
        BGFX_EMBEDDED_SHADER_END()
};

GlyphErrorBenchmark::GlyphErrorBenchmark(FontManager *font_manager, Window *window, TrueTypeHandle font_file_handle) : font_manager_(font_manager), window_(window),
                                                                                                                       font_file_handle_(font_file_handle) {
    InitializeTextures();

    bgfx::setViewFrameBuffer(1, frame_buffer_);
    bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff, 1.0f, 0);

    window_->SetUpdateLoop([this]() { Update(); });
    window_->GetRenderer()->SetManualMode(true);

    out_buffer_ = new uint8_t[1024 * 1024 * 4];
    glyph_buffer_ = new uint8_t[512 * 512 * 4];

    GenerateGlyph(FontType::Msdf, 128, 'A');
    CreateQuad();
}

void GlyphErrorBenchmark::InitializeTextures() {
    // we will render to this texture
    target_texture_ = bgfx::createTexture2D(texture_width_, texture_height_, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT);
    if (!bgfx::isValid(target_texture_)) PrintError("Failed to create frame buffer texture");

    frame_buffer_ = bgfx::createFrameBuffer(1, &target_texture_);

    // we will copy target_texture to this texture and read from it
    render_texture_ = bgfx::createTexture2D(texture_width_, texture_height_, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);
    if (!bgfx::isValid(render_texture_)) PrintError("Failed to create render texture");
}

void GlyphErrorBenchmark::InitializeShaders(FontType font_type) {
    const auto type = bgfx::getRendererType();
    std::string frag_shader;

    switch (font_type) {
        case FontType::Bitmap:
            frag_shader = "fs_font_basic_bench";
            break;
        case FontType::SdfFromBitmap:
        case FontType::SdfFromVector:
            frag_shader = "fs_font_sdf_bench";
            break;
        case FontType::Msdf:
        case FontType::MsdfOriginal:
            frag_shader = "fs_font_msdf_bench";
            break;
    }

    basic_program_ = bgfx::createProgram(
            bgfx::createEmbeddedShader(embedded_shaders_, type, "vs_font_basic_bench"),
            bgfx::createEmbeddedShader(embedded_shaders_, type, frag_shader.c_str()),
            true
    );

    tex_color_uniform_ = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
}

GlyphErrorBenchmark::~GlyphErrorBenchmark() {
    window_->GetRenderer()->SetManualMode(false);

    bgfx::destroy(render_texture_);
    bgfx::destroy(target_texture_);
    bgfx::destroy(frame_buffer_);

    delete[] out_buffer_;
    delete[] glyph_buffer_;
}

void GlyphErrorBenchmark::GenerateGlyph(FontType font_type, int font_size, CodePoint code_point) {
    const int padding = 5;
    const auto font = font_manager_->CreateFontByPixelSize(font_file_handle_, 0, font_size, font_type, padding);

    AtlasRegion::Type bitmap_type{};
    GlyphInfo info{};
    font_manager_->GenerateGlyph(font, code_point, glyph_buffer_, bitmap_type, info);
    font_manager_->DestroyFont(font);

    switch (font_type) {
        case FontType::Bitmap:
        case FontType::SdfFromBitmap:
            glyph_texture_ = bgfx::createTexture2D(info.width, info.height, false, 1, bgfx::TextureFormat::R8, 0, bgfx::copy(glyph_buffer_, info.width * info.height));
            break;
        default:
            glyph_texture_ = bgfx::createTexture2D(info.width, info.height, false, 1, bgfx::TextureFormat::RGBA8, 0, bgfx::copy(glyph_buffer_, info.width * info.height * 4));
            break;
    }
    AdjustQuadForGlyph(info.width, info.height, padding, 3.0);
    InitializeShaders(font_type);

    const auto channels = font_type == FontType::Bitmap || font_type == FontType::SdfFromBitmap ? 1.0f : 4.0f;
    stbi_write_png("glyph.png", info.width, info.height, channels, glyph_buffer_, info.width * channels);
}

void GlyphErrorBenchmark::CreateQuad() {
    bgfx::VertexLayout layout;
    layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float, true)
            .end();

    vb_ = bgfx::createVertexBuffer(
            bgfx::makeRef(quad_vertices_, sizeof(quad_vertices_)),
            layout
    );
    ib_ = bgfx::createIndexBuffer(bgfx::makeRef(quad_indices_, sizeof(quad_indices_)));
}

void GlyphErrorBenchmark::RunBenchmark() {
    window_->StartUpdate();
}

void GlyphErrorBenchmark::Update() {
    // render to texture
    bgfx::setViewRect(1, 0, 0, texture_width_, texture_height_);
    bgfx::touch(1);

    bgfx::setVertexBuffer(0, vb_);
    bgfx::setIndexBuffer(ib_);

    bgfx::setTexture(0, tex_color_uniform_, glyph_texture_);

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
    bgfx::submit(1, basic_program_);

    // render to window
    bgfx::setViewRect(0, 0, 0, window_->GetWindowWidth(), window_->GetWindowHeight());
    bgfx::touch(0);

    bgfx::setVertexBuffer(0, vb_);
    bgfx::setIndexBuffer(ib_);

    bgfx::setTexture(0, tex_color_uniform_, glyph_texture_);

    bgfx::setState(0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA));
    bgfx::submit(0, basic_program_);

    // copy render texture and save
    bgfx::blit(2, render_texture_, 0, 0, target_texture_, 0, 0, texture_width_, texture_height_);
    const auto current_frame = bgfx::frame();
    WriteBufferToImageIfReady(current_frame);
}

void GlyphErrorBenchmark::WriteBufferToImageIfReady(uint32_t current_frame) {
    if (is_frame_read_) return;
    if (read_frame_ == 0) {
        read_frame_ = bgfx::readTexture(render_texture_, out_buffer_);
        return;
    }
    if (read_frame_ == 0 || current_frame < read_frame_) return;

//    stbi_flip_vertically_on_write(1);
    stbi_write_png("render_texture.png", texture_width_, texture_height_, 4, out_buffer_, texture_width_ * 4);
    std::cout << "Render texture saved" << std::endl;

    read_frame_ = 0;
    is_frame_read_ = true;
}

void GlyphErrorBenchmark::AdjustQuadForGlyph(int glyph_width, int glyph_height, int glyph_padding, float scale) {
    float width = (static_cast<float>(glyph_width) / texture_width_) * scale;
    float height = (static_cast<float>(glyph_height) / texture_height_) * scale;
    float padding = (static_cast<float>(glyph_padding) / texture_width_) * scale;

    float width_map = width * 2 - 1.0f; // map from [0, 1] to [-1, 1]
    float height_map = height * (-2) + 1.0f; // map from [0, 1] to [1, -1]

    quad_vertices_[0].x = -1.0f - padding;
    quad_vertices_[0].y = 1.0f + padding;

    quad_vertices_[1].x = width_map - padding;
    quad_vertices_[1].y = 1.0f + padding;

    quad_vertices_[2].x = -1.0f - padding;
    quad_vertices_[2].y = height_map + padding;

    quad_vertices_[3].x = width_map - padding;
    quad_vertices_[3].y = height_map + padding;
}
