//
// Created by Mihael on 08/08/2024.
//

#include <bgfx/bgfx.h>
#include "Renderer.h"

Renderer::Renderer(int width, int height, WindowHandle window) {
    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::Count;
    bgfx_init.resolution.width = width;
    bgfx_init.resolution.height = height;
    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx_init.platformData.nwh = window.GetWindowHandle();
    bgfx::init(bgfx_init);

    bgfx::setDebug(BGFX_DEBUG_NONE);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);
}

Renderer::~Renderer() {
    bgfx::shutdown();
}
