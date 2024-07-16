#define GLFW_EXPOSE_NATIVE_WIN32

#include <iostream>

#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "managers/FontManager.h"
#include "managers/TextBufferManager.h"
#include "managers/InputManager.h"
#include "utilities.h"

// constants
constexpr int k_window_width_ = 1800; 
constexpr int k_window_height_ = 900;

// global variables
GLFWwindow* window_;

// managers
FontManager* font_manager_;
TextBufferManager* text_buffer_manager_;
InputManager* input_manager_;

// fonts
TrueTypeHandle font_file_;
FontHandle original_font_;
FontHandle bitmap_font_;
FontHandle bitmap_scaled_font_;
FontHandle sdf_font_;
FontHandle sdf_scaled_font_;
FontHandle msdf_font_;
FontHandle msdf_scaled_font_;

TextBufferHandle original_text_buffer_;
TextBufferHandle bitmap_text_buffer_;
TextBufferHandle sdf_text_buffer_;
TextBufferHandle msdf_text_buffer_;

// function handlers
FunctionId key_pressed_id_;
FunctionId key_released_id_;

std::string dynamic_text_ = "Lorem ipsum dolor sit amet, consectetur adipiscing elit";

bool shift_pressed_ = false;

GLFWwindow* CreateAndLinkWindow() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(k_window_width_, k_window_height_, "BGFX fonts", nullptr, nullptr);

    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::Count;
    bgfx_init.resolution.width = k_window_width_;
    bgfx_init.resolution.height = k_window_height_;
    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx_init.platformData.nwh = glfwGetWin32Window(window);
    bgfx::init(bgfx_init);

    bgfx::setDebug(BGFX_DEBUG_NONE);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, k_window_width_, k_window_height_);

    return window;
}

void InitFonts() {
    font_manager_ = new FontManager(1024);
    text_buffer_manager_ = new TextBufferManager(font_manager_);
    
    font_file_ = font_manager_->CreateTtf("../assets/fonts/OpenSans-Bold.ttf");
    
    original_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, 64, FontType::Bitmap, 0);
    
    bitmap_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, 16, FontType::Bitmap, 0);
    bitmap_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(bitmap_font_, 64); // create scaled fonts to show the power of SDF
    
    sdf_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, 16, FontType::Sdf, 8);
    sdf_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(sdf_font_, 64);
    
    msdf_font_ = font_manager_->CreateFontByPixelSize(font_file_, 0, 16, FontType::Msdf, 8);
    msdf_scaled_font_ = font_manager_->CreateScaledFontToPixelSize(msdf_font_, 64);

    original_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Bitmap, BufferType::Transient);
    bitmap_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Bitmap, BufferType::Transient);
    sdf_text_buffer_ = text_buffer_manager_->CreateTextBuffer(FontType::Sdf, BufferType::Transient);
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
    
    // include numbers
    for (int i = 48; i <= 57; i++) {
        keys_to_track.push_back(i);
    }
    
    input_manager_ = new InputManager(window_, keys_to_track);
    key_pressed_id_ = InputManager::SubscribeKeyPressed(HandleKeyPressed);
    key_released_id_ = InputManager::SubscribeKeyReleased(HandleKeyReleased);
}

void SetViewTransform() {
    const bx::Vec3 at  = { 0.0f, 0.0f,  10.0f };
    const bx::Vec3 eye = { 0.0f, 0.0f, -1.0f };
    float view[16];
    bx::mtxLookAt(view, eye, at);

    float ortho[16];
    bx::mtxOrtho(ortho,
                0.0f,
                float(k_window_width_),
                float(k_window_height_),
                0.0f,
                0.0f,
                100.0f,
                0.0f,
                bgfx::getCaps()->homogeneousDepth
    );

    bgfx::setViewTransform(0, view, ortho);
    bgfx::setViewRect(0, 0, 0, uint16_t(k_window_width_), uint16_t(k_window_height_) );
}

void ClearTextBuffers() {
    text_buffer_manager_->ClearTextBuffer(original_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(bitmap_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(sdf_text_buffer_);
    text_buffer_manager_->ClearTextBuffer(msdf_text_buffer_);
}

void Update() {
    while(!glfwWindowShouldClose(window_)) {
        // rendering
        glfwPollEvents();
        bgfx::touch(0);
        
        ClearTextBuffers();

        text_buffer_manager_->SetPenPosition(original_text_buffer_, 10.0f, 10.0f);
        text_buffer_manager_->AppendText(original_text_buffer_, original_font_, dynamic_text_.c_str());
        
        text_buffer_manager_->SetPenPosition(bitmap_text_buffer_, 10.0f, 80.0f);
        text_buffer_manager_->AppendText(bitmap_text_buffer_, bitmap_scaled_font_, dynamic_text_.c_str());
        
        text_buffer_manager_->SetPenPosition(sdf_text_buffer_, 10.0f, 150.0f);
        text_buffer_manager_->AppendText(sdf_text_buffer_, sdf_scaled_font_, dynamic_text_.c_str());
        
        text_buffer_manager_->SetPenPosition(msdf_text_buffer_, 10.0f, 220.0f);
        text_buffer_manager_->AppendText(msdf_text_buffer_, msdf_scaled_font_, dynamic_text_.c_str());
        
        SetViewTransform();

        // draw text buffers
        text_buffer_manager_->SubmitTextBuffer(original_text_buffer_, 0);
        text_buffer_manager_->SubmitTextBuffer(bitmap_text_buffer_, 0);
        text_buffer_manager_->SubmitTextBuffer(msdf_text_buffer_, 0);
        text_buffer_manager_->SubmitTextBuffer(sdf_text_buffer_, 0);

        bgfx::frame();
    }
}

void Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
    
    // destroy ttf file handles
    font_manager_->DestroyTtf(font_file_);
    
    // destroy font handles
    font_manager_->DestroyFont(original_font_);
    font_manager_->DestroyFont(bitmap_font_);
    font_manager_->DestroyFont(bitmap_scaled_font_);
    font_manager_->DestroyFont(sdf_font_);
    font_manager_->DestroyFont(sdf_scaled_font_);
    font_manager_->DestroyFont(msdf_font_);
    font_manager_->DestroyFont(msdf_scaled_font_);
    
    // destroy text buffer handles
    text_buffer_manager_->DestroyTextBuffer(original_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(bitmap_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(sdf_text_buffer_);
    text_buffer_manager_->DestroyTextBuffer(msdf_text_buffer_);
    
    // unsubscribe from input manager
    InputManager::UnsubscribeKeyPressed(key_pressed_id_);
    InputManager::UnsubscribeKeyReleased(key_released_id_);
    
    // destroy managers
    delete font_manager_;
    delete text_buffer_manager_;
    delete input_manager_;
    
    bgfx::shutdown();
}

int main() {
    // initialization
    window_ = CreateAndLinkWindow();
    InitFonts();
    InitInputManager();
    
    // update loop
    Update();
    
    Shutdown();
    
    return 0;
}