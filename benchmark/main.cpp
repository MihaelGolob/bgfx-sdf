#include <window/Window.h>
#include <managers/FontManager.h>
#include <managers/TextBufferManager.h>
#include <font_processing/FontInfo.h>
#include <utilities.h>
#include "GlyphGenerationBenchmark.h"
#include "GlyphRenderingBenchmark.h"

const int k_window_width_ = 1500;
const int k_window_height_ = 900;

Window *window_;
FontManager *font_manager_;
TextBufferManager *text_buffer_manager_;
TrueTypeHandle font_file_;

auto font_types_to_test_ = std::vector<FontType>{FontType::Bitmap, FontType::SdfFromBitmap, FontType::SdfFromVector, FontType::Msdf, FontType::MsdfOriginal};

void Setup() {
    font_manager_ = new FontManager(1024);
    text_buffer_manager_ = new TextBufferManager(font_manager_);
    font_file_ = font_manager_->CreateTtf("../assets/fonts/Helvetica-Bold.ttf");
}

void Shutdown() {
    font_manager_->DestroyTtf(font_file_);

    delete font_manager_;
    delete text_buffer_manager_;
    delete window_;
}

void PrintGlyphBenchmarkResults(const std::string &name, std::vector<double> results, std::vector<FontType> font_types) {
    PrintGreen(name.c_str());
    for (int i = 0; i < font_types.size(); i++) {
        std::string str = "Font type ";
        str.append(FontInfo::FontTypeToString(font_types[i]));
        str.append(" took on average: ");
        str.append(std::to_string(results[i]));
        str.append(" ms.");

        PrintGreen(str.c_str());
    }
}

void BenchmarkGlyphGeneration() {
    auto glyph_generation_benchmark = GlyphGenerationBenchmark(font_manager_, font_file_);
    glyph_generation_benchmark.SetupBenchmark(font_types_to_test_, 500, false);
    auto mean_times = glyph_generation_benchmark.RunBenchmark();

    PrintGlyphBenchmarkResults("Glyph Generation Results (ms / glyph generated)", mean_times, font_types_to_test_);
}

void BenchmarkGlyphRendering() {
    auto glyph_rendering_benchmark = GlyphRenderingBenchmark(window_, font_manager_, text_buffer_manager_, font_file_);
    glyph_rendering_benchmark.SetupBenchmark(500, 60, font_types_to_test_, [&](const std::vector<double> &results) {
        PrintGlyphBenchmarkResults("Glyph Rendering Results (ms / frame)", results, font_types_to_test_);
    });
    glyph_rendering_benchmark.RunBenchmark();
}

int main() {
    window_ = new Window(k_window_width_, k_window_height_, "Benchmark");
    Setup();

    BenchmarkGlyphGeneration();
    BenchmarkGlyphRendering();

    Shutdown();
    return 0;
}