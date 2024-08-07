﻿#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "managers/FontManager.h"
#include "managers/TextBufferManager.h"
#include "managers/InputManager.h"

// constants
constexpr int k_window_width_ = 1500;
constexpr int k_window_height_ = 900;

// global variables
Window *window_;

// managers
FontManager *font_manager_;
TextBufferManager *text_buffer_manager_;
InputManager *input_manager_;

// fonts
TrueTypeHandle font_file_;
FontHandle original_font_;

FontHandle bitmap_font_;
FontHandle bitmap_scaled_font_;

FontHandle sdf_bitmap_font_;
FontHandle sdf_bitmap_scaled_font_;

FontHandle sdf_vector_font_;
FontHandle sdf_vector_scaled_font_;

FontHandle msdf_font_;
FontHandle msdf_scaled_font_;

TextBufferHandle original_text_buffer_;
TextBufferHandle bitmap_text_buffer_;
TextBufferHandle sdf_bitmap_text_buffer_;
TextBufferHandle sdf_vector_text_buffer_;
TextBufferHandle msdf_text_buffer_;

// function handlers
FunctionId key_pressed_id_;
FunctionId key_released_id_;

std::string dynamic_text_ = "ABCDVWXabcdvwx1234";

bool shift_pressed_ = false;

void InitFonts() {
    font_manager_ = new FontManager(1024);
    text_buffer_manager_ = new TextBufferManager(font_manager_);

    font_file_ = font_manager_->CreateTtf("../assets/fonts/Helvetica-Bold.ttf");

    int scale_from = 16;
    int scale_to = 100;
    // create different fonts to compare their rendering quality
    // apart from the original font they are all created as (max) 16x16 bitmaps
    // then they are scaled up 4x
    original_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_to, FontType::Bitmap, 0);

    bitmap_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_from, FontType::Bitmap, 0);
    bitmap_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(bitmap_font_, scale_to); // create scaled fonts to show the power of SDF

    sdf_bitmap_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_from, FontType::SdfFromBitmap, 2);
    sdf_bitmap_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(sdf_bitmap_font_, scale_to);

    sdf_vector_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_from, FontType::SdfFromVector, 2);
    sdf_vector_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(sdf_vector_font_, scale_to);

    msdf_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_from, FontType::Msdf, 2);
    msdf_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(msdf_font_, scale_to);

    original_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Bitmap, BufferType::Transient);
    bitmap_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Bitmap, BufferType::Transient);
    sdf_bitmap_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::SdfFromBitmap, BufferType::Transient);
    sdf_vector_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::SdfFromVector, BufferType::Transient);
    msdf_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Msdf, BufferType::Transient);
}

void HandleKeyPressed(int key) {
    if (key == GLFW_KEY_BACKSPACE) {
        if (!dynamic_text_.empty()) {
            dynamic_text_.pop_back();
        }
    } else if (key == GLFW_KEY_LEFT_SHIFT) {
        shift_pressed_ = true;
    } else {
        int offset = shift_pressed_ ? 'A' - 'a' : 0; // if shift is pressed, convert to uppercase
        dynamic_text_ += (char) (InputManager::GetKeyChar(key) + offset);
    }
}

void HandleKeyReleased(int key) {
    if (key == GLFW_KEY_LEFT_SHIFT) {
        shift_pressed_ = false;
    }
}

void InitInputManager() {
    std::vector<int> keys_to_track;

    // include alphabet
    for (int i = 65; i <= 90; i++) {
        keys_to_track.push_back(i);
    }

    keys_to_track.push_back(GLFW_KEY_SPACE);
    keys_to_track.push_back(GLFW_KEY_BACKSPACE);
    keys_to_track.push_back(GLFW_KEY_LEFT_SHIFT);
    keys_to_track.push_back(GLFW_KEY_COMMA);

    // include numbers
    for (int i = 48; i <= 57; i++) {
        keys_to_track.push_back(i);
    }

    input_manager_ = new InputManager(window_, keys_to_track);
    key_pressed_id_ = InputManager::SubscribeKeyPressed(HandleKeyPressed);
    key_released_id_ = InputManager::SubscribeKeyReleased(HandleKeyReleased);
}

void ClearTextBuffers() {
    text_buffer_manager_->ClearTextBuffer(original_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(bitmap_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(sdf_bitmap_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(sdf_vector_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(msdf_text_buffer_);
}

void DrawTextBuffers() {
    text_buffer_manager_->SubmitTextBuffer(original_text_buffer_, 0);
    text_buffer_manager_->SubmitTextBuffer(bitmap_text_buffer_, 0);
    text_buffer_manager_->SubmitTextBuffer(msdf_text_buffer_, 0);
    text_buffer_manager_->SubmitTextBuffer(sdf_vector_text_buffer_, 0);
    text_buffer_manager_->SubmitTextBuffer(sdf_bitmap_text_buffer_, 0);
}

void Update() {
    ClearTextBuffers();

    text_buffer_manager_->SetPenPosition(original_text_buffer_, 10.0f, 10.0f);
    text_buffer_manager_->AppendText(original_text_buffer_, original_font_, dynamic_text_.c_str());

    text_buffer_manager_->SetPenPosition(bitmap_text_buffer_, 10.0f, 110.0f);
    text_buffer_manager_->AppendText(bitmap_text_buffer_, bitmap_scaled_font_, dynamic_text_.c_str());

    text_buffer_manager_->SetPenPosition(sdf_bitmap_text_buffer_, 10.0f, 220.0f);
    text_buffer_manager_->AppendText(sdf_bitmap_text_buffer_, sdf_bitmap_scaled_font_, dynamic_text_.c_str());

    text_buffer_manager_->SetPenPosition(sdf_vector_text_buffer_, 10.0f, 320.0f);
    text_buffer_manager_->AppendText(sdf_vector_text_buffer_, sdf_vector_scaled_font_, dynamic_text_.c_str());

    text_buffer_manager_->SetPenPosition(msdf_text_buffer_, 10.0f, 420.0f);
    text_buffer_manager_->AppendText(msdf_text_buffer_, msdf_scaled_font_, dynamic_text_.c_str());

    DrawTextBuffers();
}

void DestroyAllFonts() {
    font_manager_->DestroyFont(original_font_);

    font_manager_->DestroyFont(bitmap_font_);
    font_manager_->DestroyFont(bitmap_scaled_font_);

    font_manager_->DestroyFont(sdf_bitmap_font_);
    font_manager_->DestroyFont(sdf_bitmap_scaled_font_);

    font_manager_->DestroyFont(sdf_vector_font_);
    font_manager_->DestroyFont(sdf_vector_scaled_font_);

    font_manager_->DestroyFont(msdf_font_);
    font_manager_->DestroyFont(msdf_scaled_font_);
}

void DestroyAllTextBuffers() {
    text_buffer_manager_->DestroyTextBuffer(original_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(bitmap_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(sdf_bitmap_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(sdf_vector_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(msdf_text_buffer_);
}

void DestroyAllManagers() {
    delete font_manager_;
    delete text_buffer_manager_;
    delete input_manager_;
    delete window_;
}

void Shutdown() {
    font_manager_->DestroyTtf(font_file_);
    DestroyAllFonts();
    DestroyAllTextBuffers();

    InputManager::UnsubscribeKeyPressed(key_pressed_id_);
    InputManager::UnsubscribeKeyReleased(key_released_id_);

    DestroyAllManagers();
}

int main() {
    // initialization
    window_ = new Window(k_window_width_, k_window_height_, "BGFX fonts");
    window_->SetUpdateLoop(Update);

    InitFonts();
    InitInputManager();

    // update loop
    window_->StartUpdate();

    Shutdown();

    return 0;
}