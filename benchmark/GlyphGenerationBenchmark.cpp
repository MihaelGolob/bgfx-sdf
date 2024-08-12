//
// Created by MihaelGolob on 12. 08. 2024.
//

#include "GlyphGenerationBenchmark.h"

#include <managers/FontManager.cpp>
#include <managers/TextBufferManager.cpp>

GlyphGenerationBenchmark::GlyphGenerationBenchmark(FontManager *font_manager, TextBufferManager *text_buffer_manager, TrueTypeHandle* font_file) {
    font_manager_ = font_manager;
    text_buffer_manager_ = text_buffer_manager;
    font_file_ = font_file;
}

void GlyphGenerationBenchmark::SetupBenchmark(const std::vector<FontType> &font_types_to_test, int num_iterations, bool print_progress) {
    font_types_ = font_types_to_test;
    num_iterations_ = num_iterations;
    print_progress_ = print_progress;
}

std::vector<double> GlyphGenerationBenchmark::RunBenchmark() {
    std::vector<double> mean_times;
    
    
    
    return mean_times;
}
