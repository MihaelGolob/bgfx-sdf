#include <window/Window.h>
#include <managers/FontManager.h>
#include <managers/TextBufferManager.h>
#include <font_processing/FontInfo.h>
#include <utilities.h>
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
    font_manager_->DestroyTtf(font_file_);
    
    delete font_manager_;
    delete text_buffer_manager_;
    delete window_;
}

void PrintGlyphBenchmarkResults(std::vector<double> results, std::vector<FontType> font_types) {
    PrintGreen("Glyph benchmark results:\n");
    for (int i = 0; i < font_types.size(); i++) {
        std::string str = "Font type: ";
        str.append(FontInfo::FontTypeToString(font_types[i]));
        str.append(" took on average ");
        str.append(std::to_string(results[i]));
        str.append(" ms.");

        PrintGreen(str.c_str());
    } 
}

void BenchmarkGlyphGeneration() {
    auto font_types = std::vector<FontType>{FontType::Bitmap, FontType::SdfFromBitmap, FontType::SdfFromVector, FontType::Msdf};
    
    auto glyph_generation_benchmark = GlyphGenerationBenchmark(font_manager_, text_buffer_manager_, font_file_);
    glyph_generation_benchmark.SetupBenchmark(font_types, 10, false);
    auto mean_times = glyph_generation_benchmark.RunBenchmark();
    
    PrintGlyphBenchmarkResults(mean_times, font_types);
}

void Update() {
    
}

int main() {
    window_ = new Window(k_window_width_, k_window_height_, "Benchmark");
    window_->SetUpdateLoop(Update);
    Setup();

    BenchmarkGlyphGeneration();
    
//    window_->StartUpdate();
    
    Shutdown();
    return 0;
}