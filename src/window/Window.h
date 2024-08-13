//
// Created by Mihael on 08/08/2024.
//
#include <string>
#include <functional>
#include "Renderer.h"

#pragma once

enum class WindowKeyAction {
    None, Press, Release, Repeat
};

class Window {
public:
    Window(int width, int height, const std::string &title);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void SetUpdateLoop(const std::function<void()> &update_loop = nullptr);
    void StartUpdate(const std::function<bool()>& should_pause);

    void SetKeyCallback(const std::function<void(int, WindowKeyAction)> &key_callback);

    Renderer *GetRenderer();

private:
    WindowKeyAction ConvertToWindowKeyAction(int action);

private:
    GLFWwindow *window_;
    Renderer *renderer_;

    std::function<void()> update_loop_;
    std::function<void(int, WindowKeyAction)> key_callback_;
};
