//
// Created by MihaelGolob on 2. 07. 2024.
//

#pragma once

#include <freetype/freetype.h>
#include <array>

#include "../font_processing/TrueTypeFont.h"
#include "../font_decomposition/Shape.h"
#include "../font_decomposition/Contour.h"
#include "../font_decomposition/Vector2.h"

class MsdfGenerator {
private:
    
public:
    void Init(FT_Face face, uint32_t pixel_height, int16_t padding);
    
    void BakeGlyphMsdf(CodePoint code_point, GlyphInfo &out_glyph_info, uint8_t* output);
    std::array<double, 3> GeneratePixel(const Shape& shape, const Vector2& p);
    
private:
    void CalculateGlyphMetrics(const FT_Face& face, GlyphInfo& glyph_info);
    [[nodiscard]] int MapDistanceToColorValue(float distance) const;
    
    // glyph decomposition
    static Shape ParseFtFace(CodePoint code_point, FT_Face face);
    static int FtMoveTo(const FT_Vector* to, void* user);
    static int FtLineTo(const FT_Vector* to, void* user);
    static int FtConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int FtCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
    
    float scale_ = 1;
    float padding_ = 0;
    float distance_range_ = 1.0;
    
    FT_Face face_;
};

// used for decomposition of the glyph
struct FtContext {
    double scale;
    Point2 position;
    Shape *shape;
    Contour *contour;
};
