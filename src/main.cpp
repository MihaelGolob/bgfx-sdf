#define GLFW_EXPOSE_NATIVE_WIN32

#include "bgfx/bgfx.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "ft2build.h"
#include FT_FREETYPE_H

#include <iostream>

// constants
constexpr int k_window_width_ = 1600;
constexpr int k_window_height_ = 900;

// global variables
GLFWwindow* window_;
FT_Library  ft_;

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

void InitFreetype() {
    if (FT_Init_FreeType(&ft_)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    } 
}

FT_Face LoadFont(const char* font_path, int font_size) {
    FT_Face face;
    if (FT_New_Face(ft_, font_path, 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    if (FT_Set_Char_Size(face, 0, font_size * 64, k_window_width_, k_window_height_)) {
        std::cerr << "ERROR::FREETYPE: Failed to set char size" << std::endl;
    }
    return face;
}

void Update() {
    while(!glfwWindowShouldClose(window_)) {
        glfwPollEvents();

        bgfx::touch(0);
        bgfx::frame();
    }
}

void Shutdown() {
    bgfx::shutdown();
    glfwDestroyWindow(window_);
    glfwTerminate();
}

int main() {
    // initialization
    window_ = CreateAndLinkWindow();
    InitFreetype();
    const auto font_face = LoadFont(R"(..\assets\fonts\freedom.ttf)", 20);
    
    // update loop
    Update();
    
    Shutdown();
    return 0;
}