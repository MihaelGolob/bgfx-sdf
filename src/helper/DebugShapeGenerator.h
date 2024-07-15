//
// Created by MihaelGolob on 15. 07. 2024.
//

#pragma once

#include "../font_decomposition/Shape.h"
#include "../font_processing/GlyphInfo.h"

class DebugShapeGenerator {
public:
    static Shape GetGlyphI();
    static Shape GetGlyphA();
    
    static GlyphInfo GetDebugGlyphInfo();
};
