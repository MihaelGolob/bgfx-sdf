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
    void Init(FT_Face face, uint32_t font_size, uint32_t padding);
    
    void BakeGlyphMsdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t* output);
    void BakeGlyphSdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t* output);
    
    std::array<double, 3> GenerateMsdfPixel(const Shape& shape, const Vector2& p);
    double GenerateSdfPixel(const Shape& shape, const Vector2& p);
    
private:
    [[nodiscard]] int MapDistanceToColorValue(float distance) const;
    [[nodiscard]] float ClampDistanceToRange(float distance) const;
    void ClampArrayToRange(std::array<double, 3>& array);
    void CalculateGlyphMetrics(FT_Face const &face, GlyphInfo &out_glyph_info) const;
    
    // glyph decomposition
    static Shape ParseFtFace(CodePoint code_point, FT_Face face);
    static int FtMoveTo(const FT_Vector* to, void* user);
    static int FtLineTo(const FT_Vector* to, void* user);
    static int FtConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int FtCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
    
    float font_size_ = 0;
    float distance_range_ = 1.0;
    unsigned int padding_;
    
    FT_Face face_;
};

// used for decomposition of the glyph
struct FtContext {
    double scale;
    Point2 position;
    Shape *shape;
    Contour *contour;
};
