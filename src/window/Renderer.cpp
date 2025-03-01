//
// Created by Mihael on 08/08/2024.
//

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include "Renderer.h"

Renderer::Renderer(int width, int height, WindowHandle window) : window_width_(width), window_height_(height) {
    bgfx::Init bgfx_init;
    bgfx_init.type = bgfx::RendererType::Count;
    bgfx_init.resolution.width = width;
    bgfx_init.resolution.height = height;
//    bgfx_init.resolution.reset = BGFX_RESET_VSYNC;
    bgfx_init.platformData.nwh = window.GetWindowHandle();
    bgfx::init(bgfx_init);

    bgfx::setDebug(BGFX_DEBUG_NONE);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, width, height);
}

Renderer::~Renderer() {
    bgfx::shutdown();
}

void Renderer::OnBeforeLoop() {
    if (manual_mode_) return;
    bgfx::touch(0);
}

void Renderer::OnAfterLoop() {
    if (manual_mode_) return;
    bgfx::frame();
}

void Renderer::SetViewTransform() const {
    const bx::Vec3 at = {0.0f, 0.0f, 10.0f};
    const bx::Vec3 eye = {0.0f, 0.0f, -1.0f};
    float view[16];
    bx::mtxLookAt(view, eye, at);

    float ortho[16];
    bx::mtxOrtho(ortho,
                 0.0f,
                 float(window_width_),
                 float(window_height_),
                 0.0f,
                 0.0f,
                 100.0f,
                 0.0f,
                 bgfx::getCaps()->homogeneousDepth
    );

    bgfx::setViewTransform(0, view, ortho);
    bgfx::setViewRect(0, 0, 0, uint16_t(window_width_), uint16_t(window_height_));
}

void Renderer::SetManualMode(bool manual_mode) {
    manual_mode_ = manual_mode;
}
