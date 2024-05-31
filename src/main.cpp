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
constexpr int k_window_width_ = 1400; 
constexpr int k_window_height_ = 900;

// global variables
GLFWwindow* window_;
FT_Library  ft_;

// managers
FontManager* font_manager_;
TextBufferManager* text_buffer_manager_;
InputManager* input_manager_;

// fonts
TrueTypeHandle font_file_;
FontHandle bitmap_font_;
FontHandle bitmap_scaled_font_;
FontHandle sdf_font_;
FontHandle sdf_scaled_font_;

TextBufferHandle static_bitmap_text_buffer_;
TextBufferHandle static_sdf_text_buffer_;
TextBufferHandle dynamic_text_buffer_;

// function handlers
FunctionId key_pressed_id_;
FunctionId key_released_id_;

std::string dynamic_text_;

bool shift_pressed_ = false;

GLFWwindow* CreateAndLinkWindow() {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(k_window_width_, k_window_height_, "BGFX fonts", nullptr, nullptr);

    bgfx::Init bgfxInit;
    bgfxInit.type = bgfx::RendererType::Count;
    bgfxInit.resolution.width = k_window_width_;
    bgfxInit.resolution.height = k_window_height_;
    bgfxInit.resolution.reset = BGFX_RESET_VSYNC;
    bgfxInit.platformData.nwh = glfwGetWin32Window(window);
    bgfx::init(bgfxInit);

    bgfx::setDebug(BGFX_DEBUG_NONE);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, k_window_width_, k_window_height_);

    return window;
}

TrueTypeHandle LoadTTF(const char* file_path) {
    uint32_t size;
    void* data = load(file_path, &size);

    if (nullptr != data) {
        TrueTypeHandle handle = font_manager_->createTtf( (uint8_t*)data, size);
        return handle;
    }

    TrueTypeHandle invalid = BGFX_INVALID_HANDLE;
    return invalid;
}

void InitFonts() {
    if (FT_Init_FreeType(&ft_)) {
        PrintError("Could not init FreeType Library");
    } 
    
    font_manager_ = new FontManager(1024);
    text_buffer_manager_ = new TextBufferManager(font_manager_);
    
    font_file_ = LoadTTF("../assets/fonts/droidsans.ttf");
    
    bitmap_font_ = font_manager_->createFontByPixelSize(font_file_, 0, 16, FontType::Bitmap, 0);
    bitmap_scaled_font_ = font_manager_->createScaledFontToPixelSize(bitmap_font_, 64); // create scaled fonts to show the power of SDF
    
    sdf_font_ = font_manager_->createFontByPixelSize(font_file_, 0, 16, FontType::SDF, 8);
    sdf_scaled_font_ = font_manager_->createScaledFontToPixelSize(sdf_font_, 64);
    
    static_bitmap_text_buffer_ = text_buffer_manager_->createTextBuffer(FontType::Bitmap, BufferType::Static);
    static_sdf_text_buffer_ = text_buffer_manager_->createTextBuffer(FontType::SDF, BufferType::Static);
    dynamic_text_buffer_ = text_buffer_manager_->createTextBuffer(FontType::SDF, BufferType::Transient);
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
    
    PrintInfo(dynamic_text_);
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

void DrawStaticText() {
    text_buffer_manager_->clearTextBuffer(static_bitmap_text_buffer_);
    text_buffer_manager_->clearTextBuffer(static_sdf_text_buffer_);
    
    // draw static bitmap text
    text_buffer_manager_->setPenPosition(static_bitmap_text_buffer_, 10.0f, 10.0f);
    text_buffer_manager_->appendText(static_bitmap_text_buffer_, bitmap_scaled_font_, "Bitmap font scaled from 16px to 64px");
    // draw static sdf text
    text_buffer_manager_->setPenPosition(static_sdf_text_buffer_, 10.0f, 80.0f);
    text_buffer_manager_->appendText(static_sdf_text_buffer_, sdf_scaled_font_, "SDF font scaled from 16px to 64px");
}

void Update() {
    while(!glfwWindowShouldClose(window_)) {
        // rendering
        glfwPollEvents();
        bgfx::touch(0);
        
        // draw dynamically typed text
        text_buffer_manager_->clearTextBuffer(dynamic_text_buffer_);
        text_buffer_manager_->setPenPosition(dynamic_text_buffer_, 10.0f, 150.0f);
        text_buffer_manager_->appendText(dynamic_text_buffer_, sdf_scaled_font_, dynamic_text_.c_str());
        
        SetViewTransform();

        // draw text buffers
        text_buffer_manager_->submitTextBuffer(dynamic_text_buffer_, 0);
        text_buffer_manager_->submitTextBuffer(static_bitmap_text_buffer_, 0);
        text_buffer_manager_->submitTextBuffer(static_sdf_text_buffer_, 0);

        bgfx::frame();
    }
}

void Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
    
    // destroy ttf file handles
    font_manager_->destroyTtf(font_file_);
    
    // destroy font handles
    font_manager_->destroyFont(bitmap_font_);
    font_manager_->destroyFont(bitmap_scaled_font_);
    font_manager_->destroyFont(sdf_font_);
    font_manager_->destroyFont(sdf_scaled_font_);
    
    // destroy text buffer handles
    text_buffer_manager_->destroyTextBuffer(static_bitmap_text_buffer_);
    text_buffer_manager_->destroyTextBuffer(static_sdf_text_buffer_);
    text_buffer_manager_->destroyTextBuffer(dynamic_text_buffer_);
    
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
    
    DrawStaticText();
    // update loop
    Update();
    
    Shutdown();
    
    return 0;
}