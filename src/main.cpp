#define GLFW_EXPOSE_NATIVE_WIN32

#include "bgfx/bgfx.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include <iostream>

// constants
constexpr int k_window_width_ = 1600;
constexpr int k_window_height_ = 900;

// global variables
GLFWwindow* window_;

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

    return window;
}

void Init() {
    bgfx::setDebug(BGFX_DEBUG_NONE);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, k_window_width_, k_window_height_);
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
    window_ = CreateAndLinkWindow();
    Init();
    Update();
    Shutdown();
    return 0;
}