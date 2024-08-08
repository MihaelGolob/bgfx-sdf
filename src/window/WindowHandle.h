//
// Created by Mihael on 08/08/2024.
//

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

class WindowHandle {
public:
    explicit WindowHandle(GLFWwindow* window);
    void* GetWindowHandle();
private:
    GLFWwindow* window_;
};
