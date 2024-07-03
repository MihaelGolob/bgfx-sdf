//
// Created by MihaelGolob on 2. 07. 2024.
//

#pragma once

#include <freetype/freetype.h>
#include "../font_processing/TrueTypeFont.h"
#include "../font_decomposition/Shape.h"
#include "../font_decomposition/Contour.h"
#include "../font_decomposition/Vector2.h"

class MsdfGenerator {
private:
    static Shape ParseFtFace(CodePoint code_point, FT_Face face);
    
    static int FtMoveTo(const FT_Vector* to, void* user);
    static int FtLineTo(const FT_Vector* to, void* user);
    static int FtConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int FtCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
    
public:
    void BakeGlyphMsdf(CodePoint code_point, GlyphInfo &out_glyph_info, FT_Face face, uint8_t* output) const;
    
private:
    float scale_ = 0;
    float padding_ = 0;
};

// used for decomposition of the glyph
struct FtContext {
    double scale;
    Point2 position;
    Shape *shape;
    Contour *contour;
};
