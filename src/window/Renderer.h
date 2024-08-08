//
// Created by Mihael on 08/08/2024.
//

#pragma once

#include "WindowHandle.h"

class Renderer {
public:
    Renderer(int width, int height, WindowHandle window);
    ~Renderer();

    void SetViewTransform() const;

    void onBeforeLoop();
    void onAfterLoop();

private:
    int window_width_, window_height_;
};
