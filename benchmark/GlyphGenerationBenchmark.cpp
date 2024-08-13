//
// Created by MihaelGolob on 12. 08. 2024.
//

#include "GlyphGenerationBenchmark.h"
#include "Timer.h"

#include <managers/FontManager.cpp>
#include <managers/TextBufferManager.cpp>

GlyphGenerationBenchmark::GlyphGenerationBenchmark(FontManager *font_manager, TrueTypeHandle font_file) {
    font_manager_ = font_manager;
    font_file_ = font_file;

    num_iterations_ = 0;
    print_progress_ = false;
}

void GlyphGenerationBenchmark::SetupBenchmark(const std::vector<FontType> &font_types_to_test, int num_iterations, bool print_progress) {
    font_types_ = font_types_to_test;
    num_iterations_ = num_iterations;
    print_progress_ = print_progress;
}

std::vector<double> GlyphGenerationBenchmark::RunBenchmark() {
    std::vector<double> mean_times;

    for (auto font_type: font_types_) {
        std::string message = "Running generation benchmark for font type: ";
        message.append(FontInfo::FontTypeToString(font_type));
        PrintCyan(message.c_str());
        
        double total_time = 0;
        std::string char_set = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        const auto font = font_manager_->CreateFontByPixelSize(font_file_, 0, 32, font_type, 0);

        for (int i = 0; i < num_iterations_; i++) {
            auto output = new uint8_t[32 * 32 * 4];
            auto bitmap_type = AtlasRegion::Type::TypeGray;
            auto code_point = char_set[i % char_set.size()];

            {
                Timer timer("GenerateGlyph", [&](double time) { total_time += time; }, print_progress_);
                font_manager_->GenerateGlyph(font, code_point, output, bitmap_type);
            }
            
            delete[] output;
        }

        mean_times.push_back(total_time / num_iterations_);
        font_manager_->DestroyFont(font);
    }

    return mean_times;
}
