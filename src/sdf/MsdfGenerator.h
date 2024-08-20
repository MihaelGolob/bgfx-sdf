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
    void Init(FT_Face face, uint32_t font_size, uint32_t padding, bool apply_preprocessing = false);
    
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
    
    void MsdfCollisionCorrection(uint8_t* map, int threshold);
    [[nodiscard]] int FindHighestDifferenceInNeighbours(int x, int y, uint8_t* map);
    [[nodiscard]] bool HigherThanThreshold(const std::vector<int>& x, int how_many, int threshold);
    [[nodiscard]] bool AreCellsOfDifferentSign(int a, int b);
    [[nodiscard]] int GetMedian(int a, int b, int c);
    
    [[nodiscard]] int GetFlippedIndexFromCoordinate(int x, int y) const;
    [[nodiscard]] int GetIndexFromCoordinate(int x, int y) const;
    [[nodiscard]] Vector2 GetGlyphCoordinate(Vector2 bitmap_coordinate, FT_BBox_ bbox) const;
    
    long font_size_ = 0;
    unsigned int padding_;
    double font_scale_;
    
    int texture_width_;
    int texture_height_;
    
    int collision_correction_threshold_;
    bool apply_preprocessing_;
    
    FT_Face face_;
};
