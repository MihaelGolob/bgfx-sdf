#include <window/Window.h>
#include <managers/FontManager.h>
#include <managers/TextBufferManager.h>
#include <font_processing/FontInfo.h>
#include "GlyphGenerationBenchmark.h"

const int k_window_width_ = 1500;
const int k_window_height_ = 900;

Window* window_;
FontManager* font_manager_;
TextBufferManager* text_buffer_manager_;
TrueTypeHandle font_file_;

void Setup() {
    font_manager_ = new FontManager(1024);
    text_buffer_manager_ = new TextBufferManager(font_manager_);
    font_file_ = font_manager_->CreateTtf("../assets/fonts/Helvetica-Bold.ttf");
}

void Shutdown() {
    delete font_manager_;
    delete text_buffer_manager_;
    delete window_;
}

void BenchmarkAtlasGeneration() {
    auto glyph_generation_benchmark = GlyphGenerationBenchmark(font_manager_, text_buffer_manager_, &font_file_);
    auto font_types = std::vector<FontType>{FontType::Bitmap, FontType::SdfFromBitmap, FontType::SdfFromVector, FontType::Msdf};
    glyph_generation_benchmark.SetupBenchmark(font_types, 10, true);
}

void Update() {
    
}

int main() {
    Setup();

    BenchmarkAtlasGeneration();
    
    window_ = new Window(k_window_width_, k_window_height_, "Benchmark");
    window_->SetUpdateLoop(Update);
    
    window_->StartUpdate();
    
    Shutdown();
    return 0;
}