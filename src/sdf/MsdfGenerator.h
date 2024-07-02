//
// Created by MihaelGolob on 2. 07. 2024.
//

#pragma once

#include <freetype/freetype.h>
#include "../font_processing/TrueTypeFont.h"

class MsdfGenerator {
public:
    void BakeGlyphMSDF(CodePoint code_point, FT_Face face, uint8_t* output);
};
