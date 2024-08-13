//
// Created by MihaelGolob on 13. 08. 2024.
//

#pragma once

#include <managers/FontManager.h>
#include <managers/TextBufferManager.h>
#include <window/Window.h>

class GlyphRenderingBenchmark {
public:
    GlyphRenderingBenchmark(Window* window, FontManager* font_manager, TextBufferManager* text_buffer_manager, TrueTypeHandle font_file);
    void SetupBenchmark(int num_iterations, int num_draws, const std::vector<FontType> &font_types_to_test, const std::function<void(const std::vector<double>&)>& on_results = nullptr);
    void RunBenchmark();
    
private:
    void Update();
    void OnIterationResults(double time);
    
    void SetTextToDisplay();
    void CreateTextBuffers(FontType font_type);
    void DestroyTextBuffers();
    
    void ClearTextBuffers();
    void SetTextBuffers();
    void SubmitTextBuffers();
    
    Window* window_;
    FontManager* font_manager_;
    TextBufferManager* text_buffer_manager_;
    TrueTypeHandle font_file_{};
    
    int num_iterations_{};
    int num_draws_{};
    int iteration_counter_{};
    std::vector<FontType> font_types_to_test_;
    std::function<void(const std::vector<double>&)> on_results_;
    
    FontHandle font_{};
    std::vector<TextBufferHandle> text_buffers_{};
    
    bool iteration_finished_ = false;
    const wchar_t* char_set_ = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string text_to_display_;
    
    std::vector<double> mean_frame_times_;
};
