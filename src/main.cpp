#include <iostream>
#include <vector>
#include <utility>

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

std::vector<std::pair<FontHandle, FontType>> fonts_;
std::vector<std::pair<FontHandle, FontType>> scaled_fonts_;
std::vector<std::pair<TextBufferHandle, FontType>> text_buffers_;

std::vector<FontType> font_types_ = {FontType::Bitmap, FontType::SdfFromBitmap, FontType::SdfFromVector, FontType::Msdf, FontType::MsdfOriginal};

FontHandle original_font_;
TextBufferHandle original_text_buffer_;

// function handlers
FunctionId key_pressed_id_;
FunctionId key_released_id_;

std::string dynamic_text_ = "ABCDVWXabcdvwx1234";

bool shift_pressed_ = false;

void InitFonts() {
    font_manager_ = new FontManager(1024);
    text_buffer_manager_ = new TextBufferManager(font_manager_);

    font_file_ = font_manager_->CreateTtf("../assets/fonts/OpenSans-Bold.ttf");

    int scale_from = 16;
    int scale_to = 140;
    // create different fonts to compare their rendering quality
    // apart from the original font they are all created as (max) 16x16 bitmaps
    // then they are scaled up 4x
    original_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_to, FontType::Bitmap, 0);
    original_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Bitmap, BufferType::Transient);

    for (const auto &type: font_types_) {
        auto font = font_manager_->CreateFontByPixelSize(font_file_, 0, scale_from, type, 2);
        auto scaled_font = font_manager_->CreateScaledFontToPixelSize(font, scale_to);
        auto buffer = text_buffer_manager_->CreateTextBuffer(type, BufferType::Transient);

        fonts_.emplace_back(font, type);
        scaled_fonts_.emplace_back(scaled_font, type);
        text_buffers_.emplace_back(buffer, type);
    }
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
    for (const auto &buffer: text_buffers_) {
        text_buffer_manager_->ClearTextBuffer(buffer.first);
    }
}

void DrawTextBuffers() {
    text_buffer_manager_->SubmitTextBuffer(original_text_buffer_, 0);
    for (const auto &buffer: text_buffers_) {
        text_buffer_manager_->SubmitTextBuffer(buffer.first, 0);
    }
}

void Update() {
    ClearTextBuffers();

    text_buffer_manager_->SetPenPosition(original_text_buffer_, 10.0f, 10.0f);
    text_buffer_manager_->AppendText(original_text_buffer_, original_font_, dynamic_text_.c_str());

    const auto get_buffer_for_type = [&](FontType type) {
        for (const auto &buffer: text_buffers_) {
            if (buffer.second == type) {
                return buffer.first;
            }
        }
        return TextBufferHandle();
    };

    for (int i = 0; i < scaled_fonts_.size(); i++) {
        const auto font = scaled_fonts_[i].first;
        const auto type = scaled_fonts_[i].second;

        text_buffer_manager_->SetPenPosition(get_buffer_for_type(type), 10.0f, 10.0f + 120.0f * (i + 1));
        text_buffer_manager_->AppendText(get_buffer_for_type(type), font, dynamic_text_.c_str());
    }

    DrawTextBuffers();
}

void DestroyAllFonts() {
    font_manager_->DestroyFont(original_font_);

    for (const auto &font: fonts_) {
        font_manager_->DestroyFont(font.first);
    }

    for (const auto &font: scaled_fonts_) {
        font_manager_->DestroyFont(font.first);
    }
}

void DestroyAllTextBuffers() {
    text_buffer_manager_->DestroyTextBuffer(original_text_buffer_);

    for (const auto &buffer: text_buffers_) {
        text_buffer_manager_->DestroyTextBuffer(buffer.first);
    }
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
    window_->GetRenderer()->SetViewTransform();
    window_->SetUpdateLoop(Update);

    InitFonts();
    InitInputManager();

    // update loop
    window_->StartUpdate();

    Shutdown();

    return 0;
}