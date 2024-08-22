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

    void OnBeforeLoop();
    void OnAfterLoop();
    
    void SetManualMode(bool manual_mode);

private:
    int window_width_, window_height_;
    bool manual_mode_ = false;
};
