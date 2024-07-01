//
// Created by MihaelGolob on 1. 07. 2024.
//

#pragma once

#include "../../libs/bgfx.cmake/bimg/3rdparty/etc2/Types.hpp"
#include "../font_processing/FontHandles.h"

class msdfGenerator {
public:
    static void generateMSDF(uint8* out, int width, int height, TrueTypeHandle font_handle);
};
