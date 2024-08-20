//
// Created by MihaelGolob on 16. 08. 2024.
//
#pragma once

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <string>
#include "../font_processing/GlyphInfo.h"
#include "../font_processing/TextBuffer.h"

class MsdfOriginalGenerator {
public:
    MsdfOriginalGenerator();
    void Init(const std::string& font_path, int font_size);
    
    void BakeGlyphMsdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output);
    
private:
    msdfgen::FreetypeHandle *ft_;
    msdfgen::FontHandle *font_{};
    int font_size_{};
};
