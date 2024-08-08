//
// Created by Mihael on 08/08/2024.
//

#include "WindowHandle.h"

WindowHandle::WindowHandle(GLFWwindow *window) {
    window_ = window;
}

void *WindowHandle::GetWindowHandle() {
    return glfwGetWin32Window(window_);
}
