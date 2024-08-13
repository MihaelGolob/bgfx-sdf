//
// Created by Mihael on 08/08/2024.
//
#include <GLFW/glfw3.h>

#include "Window.h"
#include "WindowHandle.h"
#include "../utilities.h"

Window::Window(int width, int height, const std::string &title) {
    glfwInit();
    window_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window_, static_cast<void *>(this));

    auto window_handle = WindowHandle(window_);
    renderer_ = new Renderer(width, height, window_handle);
    renderer_->SetViewTransform();
}

Window::~Window() {
    glfwSetWindowUserPointer(window_, nullptr);
    glfwDestroyWindow(window_);
    glfwTerminate();
    delete renderer_;
}

void Window::SetUpdateLoop(const std::function<void()> &update_loop) {
    update_loop_ = update_loop;
}

void Window::StartUpdate(const std::function<bool()>& should_pause) {
    while (!glfwWindowShouldClose(window_)) {
        if (should_pause && should_pause()) {
            break;
        }
        
        glfwPollEvents();
        renderer_->onBeforeLoop();
        update_loop_();
        renderer_->onAfterLoop();
    }
}

void Window::SetKeyCallback(const std::function<void(int, WindowKeyAction)> &key_callback) {
    key_callback_ = key_callback;

    glfwSetKeyCallback(window_, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        // needed because glfw takes a plain old function pointer, which doesn't work with captures
        auto self = static_cast<Window *>(glfwGetWindowUserPointer(window));
        self->key_callback_(key, self->ConvertToWindowKeyAction(action));
    });
}

WindowKeyAction Window::ConvertToWindowKeyAction(int action) {
    switch (action) {
        case GLFW_PRESS:
            return WindowKeyAction::Press;
        case GLFW_RELEASE:
            return WindowKeyAction::Release;
        case GLFW_REPEAT:
            return WindowKeyAction::Repeat;
        default:
            PrintError("Unknown key action!");
            return WindowKeyAction::None;
    }

}

Renderer *Window::GetRenderer() {
    return renderer_;
}
