//
// Created by MihaelGolob on 12. 08. 2024.
//

#pragma once

#include <font_processing/FontInfo.h>
#include <vector>
#include <font_processing/FontHandles.h>

class FontManager;
class TextBufferManager;

class GlyphGenerationBenchmark {
public:
    GlyphGenerationBenchmark(FontManager* font_manager, TrueTypeHandle font_file);
    void SetupBenchmark(const std::vector<FontType>& font_types_to_test, int num_iterations, bool print_progress = false);
    std::vector<double> RunBenchmark();
    
private:
    FontManager* font_manager_;
    TrueTypeHandle font_file_{};
    
    std::vector<FontType> font_types_;
    int num_iterations_;
    bool print_progress_;
};
