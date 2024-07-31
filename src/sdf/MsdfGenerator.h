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
    [[nodiscard]] int MapDistanceToColorValue(double distance, double distance_range) const;
    [[nodiscard]] double ClampDistanceToRange(double distance, double distance_range) const;
    void ClampArrayToRange(std::array<double, 3>& array, double distance_range);
    void CalculateGlyphMetrics(FT_BBox_ bbox, GlyphInfo &out_glyph_info) const;
    [[nodiscard]] double CalculateFontScale();
    int GetFlippedIndexFromCoordinate(int x, int y) const;
    Vector2 GetGlyphCoordinate(Vector2 bitmap_coordinate, FT_BBox_ bbox) const;
    
    // glyph decomposition
    Shape ParseFtFace(CodePoint code_point, double scale = 1 / 64.0);
    static int FtMoveTo(const FT_Vector* to, void* user);
    static int FtLineTo(const FT_Vector* to, void* user);
    static int FtConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int FtCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
    
    long font_size_ = 0;
    unsigned int padding_;
    double font_scale_;
    
    int texture_width_;
    int texture_height_;
    
    FT_Face face_;
};

// used for decomposition of the glyph
struct FtContext {
    double scale;
    Point2 position;
    Shape *shape;
    Contour *contour;
};
