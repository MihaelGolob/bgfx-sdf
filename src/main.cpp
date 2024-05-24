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
FontHandle font_;
TextBufferHandle static_text_buffer_;
TextBufferHandle dynamic_text_buffer_;

// function handlers
FunctionId key_pressed_id_;

const std::string static_text_ = "Use your keyboard to write something down!";
std::string dynamic_text_;

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
    font_ = font_manager_->createFontByPixelSize(font_file_, 0, 200, FontType::SDF, 15, 15);
    static_text_buffer_ = text_buffer_manager_->createTextBuffer(FontType::SDF, BufferType::Transient);
    dynamic_text_buffer_ = text_buffer_manager_->createTextBuffer(FontType::SDF, BufferType::Transient);
}

void HandleKeyPressed(int key) {
    if (key == GLFW_KEY_BACKSPACE) {
        dynamic_text_.pop_back();
    } else {
        dynamic_text_ += InputManager::GetKeyChar(key);
    }
    
    PrintInfo(dynamic_text_);
}

void InitInputManager() {
    std::vector<int> keys_to_track;
    
    // include alphabet
    for (int i = 65; i <= 90; i++) {
        keys_to_track.push_back(i);
    }
    
    keys_to_track.push_back(GLFW_KEY_SPACE);
    keys_to_track.push_back(GLFW_KEY_BACKSPACE);
    
    // include numbers
    for (int i = 48; i <= 57; i++) {
        keys_to_track.push_back(i);
    }
    
    input_manager_ = new InputManager(window_, keys_to_track);
    key_pressed_id_ = InputManager::SubscribeKeyPressed(HandleKeyPressed);
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

void Update() {
    double last_time = glfwGetTime();
    std::string text;
    int index = 0;
    double timer = 0;
    
    while(!glfwWindowShouldClose(window_)) {
        // calculate delta time
        double current_time = glfwGetTime();
        double delta_time = current_time - last_time;
        last_time = current_time;
        
        // update
        timer += delta_time;
        if (timer > 0.1) {
            text += static_text_[index];
            index++;
            timer = 0;
        }
        
        // rendering
        glfwPollEvents();
        bgfx::touch(0);
        
        // draw static text
        text_buffer_manager_->clearTextBuffer(static_text_buffer_);
        text_buffer_manager_->setPenPosition(static_text_buffer_, 10.0f, 20.0f);
        text_buffer_manager_->appendText(static_text_buffer_, font_, text.c_str());
        
        // draw dynamic text
        text_buffer_manager_->clearTextBuffer(dynamic_text_buffer_);
        text_buffer_manager_->setPenPosition(dynamic_text_buffer_, 10.0f, 200.0f);
        text_buffer_manager_->appendText(dynamic_text_buffer_, font_, dynamic_text_.c_str());
        
        SetViewTransform();

        text_buffer_manager_->submitTextBuffer(static_text_buffer_, 0);
        text_buffer_manager_->submitTextBuffer(dynamic_text_buffer_, 0);

        bgfx::frame();
    }
}

void Shutdown() {
    glfwDestroyWindow(window_);
    glfwTerminate();
    
    // destroy ttf file handles
    font_manager_->destroyTtf(font_file_);
    
    // destroy font handles
    font_manager_->destroyFont(font_);
    
    // destroy text buffer handles
    text_buffer_manager_->destroyTextBuffer(static_text_buffer_);
    text_buffer_manager_->destroyTextBuffer(dynamic_text_buffer_);
    
    // unsubscribe from input manager
    InputManager::UnsubscribeKeyPressed(key_pressed_id_);
    
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