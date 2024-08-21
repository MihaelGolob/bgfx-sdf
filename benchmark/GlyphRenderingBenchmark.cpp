//
// Created by MihaelGolob on 13. 08. 2024.
//

#include <utilities.h>
#include <helper/Timer.h>
#include "GlyphRenderingBenchmark.h"

GlyphRenderingBenchmark::GlyphRenderingBenchmark(Window *window, FontManager *font_manager, TextBufferManager *text_buffer_manager, TrueTypeHandle font_file) {
    window_ = window;
    font_manager_ = font_manager;
    text_buffer_manager_ = text_buffer_manager;
    font_file_ = font_file;
}

void GlyphRenderingBenchmark::SetupBenchmark(int num_iterations, int num_draws, const std::vector<FontType> &font_types_to_test, const std::function<void(const std::vector<double> &)> &on_results) {
    num_iterations_ = num_iterations;
    font_types_to_test_ = font_types_to_test;
    num_draws_ = num_draws;
    on_results_ = on_results;

    window_->SetUpdateLoop([&]() { Update(); });
}

void GlyphRenderingBenchmark::RunBenchmark() {
    for (auto font_type: font_types_to_test_) {
        std::string message = "Running rendering benchmark for font type: ";
        message.append(FontInfo::FontTypeToString(font_type));
        PrintCyan(message.c_str());

        // setup
        font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, 60, font_type, 2);
        font_manager_->PreloadGlyph(font_, char_set_);
        
        CreateTextBuffers(font_type);
        SetTextToDisplay();

        {
            Timer timer("Rendering benchmark", [&](double time) { OnIterationResults(time); });
            window_->StartUpdate([this]() { return iteration_finished_; });
        }

        // clean up
        iteration_finished_ = false;
        iteration_counter_ = 0;
        DestroyTextBuffers();
        font_manager_->DestroyFont(font_);
    }

    if (on_results_) {
        on_results_(mean_frame_times_);
    }
}

void GlyphRenderingBenchmark::CreateTextBuffers(FontType font_type) {
    for (int i = 0; i < num_draws_; i++) {
        auto text_buffer = text_buffer_manager_->CreateTextBuffer(font_type, BufferType::Transient);
        text_buffers_.push_back(text_buffer);
    }
}

void GlyphRenderingBenchmark::DestroyTextBuffers() {
    for (auto text_buffer: text_buffers_) {
        text_buffer_manager_->DestroyTextBuffer(text_buffer);
    }
    text_buffers_.clear();
}

void GlyphRenderingBenchmark::ClearTextBuffers() {
    for (auto text_buffer: text_buffers_) {
        text_buffer_manager_->ClearTextBuffer(text_buffer);
    }
}

void GlyphRenderingBenchmark::SetTextBuffers() {
    for (int i = 0; i < text_buffers_.size(); i++) {
        const auto &text_buffer = text_buffers_[i];
        text_buffer_manager_->SetPenPosition(text_buffer, 10, 50 + 5*i);
        text_buffer_manager_->AppendText(text_buffer, font_, text_to_display_.c_str());
    }
}

void GlyphRenderingBenchmark::SubmitTextBuffers() {
    for (auto text_buffer: text_buffers_) {
        text_buffer_manager_->SubmitTextBuffer(text_buffer, 0);
    }
}

void GlyphRenderingBenchmark::Update() {
    if (iteration_counter_ >= num_iterations_) {
        iteration_finished_ = true;
        return;
    }
    iteration_counter_++;

    ClearTextBuffers();
    SetTextBuffers();
    SubmitTextBuffers();
}

void GlyphRenderingBenchmark::OnIterationResults(double time) {
    const auto mean_frame_time = time / num_iterations_;
    mean_frame_times_.push_back(mean_frame_time);
}

void GlyphRenderingBenchmark::SetTextToDisplay() {
    text_to_display_.clear();
    for (int i = 0; i < 10; i++) {
        text_to_display_.append("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789\n");
    }
}
