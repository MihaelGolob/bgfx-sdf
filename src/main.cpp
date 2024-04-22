#define GLFW_EXPOSE_NATIVE_WIN32

#include <iostream>

#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "managers/FontManager.h"
#include "managers/TextBufferManager.h"
#include "utilities.h"

// constants
constexpr int k_window_width_ = 1600;
constexpr int k_window_height_ = 900;

// global variables
GLFWwindow* window_;
FT_Library  ft_;
FontManager* font_manager_;
TextBufferManager* text_buffer_manager_;

// fonts
TrueTypeHandle font_file_;
FontHandle font_;

TextBufferHandle text_buffer_;

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
    
    font_manager_ = new FontManager(512);
    text_buffer_manager_ = new TextBufferManager(font_manager_);
    
    font_file_ = LoadTTF("../assets/fonts/freedom.ttf");
    font_ = font_manager_->createFontByPixelSize(font_file_, 0, 40);
    text_buffer_ = text_buffer_manager_->createTextBuffer(FONT_TYPE_ALPHA, BufferType::Transient);
}

void Update() {
    while(!glfwWindowShouldClose(window_)) {
        glfwPollEvents();
        bgfx::touch(0);
        
        text_buffer_manager_->clearTextBuffer(text_buffer_);
        text_buffer_manager_->setPenPosition(text_buffer_, 10.0f, 50.0f);
        text_buffer_manager_->appendText(text_buffer_, font_, "Hello, World!\n");

        const bx::Vec3 at  = { 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye = { 0.0f, 0.0f, -1.0f };
        float view[16];
        bx::mtxLookAt(view, eye, at);
        
        const bgfx::Caps* caps = bgfx::getCaps();
        {
            float ortho[16];
            bx::mtxOrtho(
                    ortho
                    , 0.0f
                    , float(k_window_width_)
                    , float(k_window_height_)
                    , 0.0f
                    , 0.0f
                    , 100.0f
                    , 0.0f
                    , caps->homogeneousDepth
            );
            bgfx::setViewTransform(0, view, ortho);
            bgfx::setViewRect(0, 0, 0, uint16_t(k_window_width_), uint16_t(k_window_height_) );
        }
        
        text_buffer_manager_->submitTextBuffer(text_buffer_, 0);

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
    text_buffer_manager_->destroyTextBuffer(text_buffer_);
    
    // destroy managers
    delete font_manager_;
    delete text_buffer_manager_;
    
    bgfx::shutdown();
}

int main() {
    // initialization
    window_ = CreateAndLinkWindow();
    InitFonts();
    
    // update loop
    Update();
    
    Shutdown();
    
    return 0;
}