//
// Created by MihaelGolob on 2. 07. 2024.
//

#include <freetype/ftoutln.h>
#include <cmath>

#include "MsdfGenerator.h"
#include "../utilities.h"
#include "../font_decomposition/FontParser.h"

void MsdfGenerator::Init(FT_Face face, uint32_t font_size, uint32_t padding) {
    face_ = face;
    font_size_ = font_size;
    padding_ = padding;

    texture_height_ = font_size_ + 2 * padding;
    texture_width_ = font_size_ + 2 * padding;

    font_scale_ = CalculateFontScale();
    collision_correction_threshold_ = 33;
}

/* 
 * Calculate how much to scale the glyphs in order to fit them all
 * into a texture of size font_size. Currently we just check all the
 * letters and adjust the scale so that the largest fits.
 * note: this might not be the most efficient way to do this.
*/
double MsdfGenerator::CalculateFontScale() {
    long max_width = 0;
    long max_height = 0;

    for (int i = 'A'; i <= 'z'; i++) {
        auto glyph_index = FT_Get_Char_Index(face_, i);
        FT_Load_Glyph(face_, glyph_index, FT_LOAD_NO_SCALE);

        FT_BBox_ bbox{};
        FT_Outline_Get_CBox(&face_->glyph->outline, &bbox);

        max_width = std::max(max_width, bbox.xMax - bbox.xMin);
        max_height = std::max(max_height, bbox.yMax - bbox.yMin);
    }

    return (double) std::max(max_width, max_height) / font_size_;
}

void MsdfGenerator::BakeGlyphSdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output) {
    auto shape = FontParser::ParseFtFace(code_point, &face_, 1.0);
    shape.ApplyPreprocessing();

    FT_BBox_ bbox{};
    FT_Outline_Get_CBox(&face_->glyph->outline, &bbox);
    auto distance_range = std::max(bbox.xMax - bbox.xMin, bbox.yMax - bbox.yMin);

    CalculateGlyphMetrics(bbox, glyph_info);

    for (int y = 0; y < texture_height_; y++) {
        for (int x = 0; x < texture_width_; x++) {
            auto p = GetGlyphCoordinate(Vector2(x, y), bbox);
            auto distance = GenerateSdfPixel(shape, p);
            auto clamped = ClampDistanceToRange(distance, distance_range);
            auto mapped_distance = MapDistanceToColorValue(clamped, distance_range);

            auto index = GetFlippedIndexFromCoordinate(x, y);

            output[index + 0] = mapped_distance;
            output[index + 1] = mapped_distance;
            output[index + 2] = mapped_distance;
            output[index + 3] = 255;
        }
    }
}

void MsdfGenerator::BakeGlyphMsdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output) {
    auto shape = FontParser::ParseFtFace(code_point, &face_, 1.0);
    shape.ApplyPreprocessing();
    shape.ApplyEdgeColoring(15.0);
    
    FT_BBox_ bbox{};
    FT_Outline_Get_CBox(&face_->glyph->outline, &bbox);
    auto distance_range = std::max(bbox.xMax - bbox.xMin, bbox.yMax - bbox.yMin);

    CalculateGlyphMetrics(bbox, glyph_info);

    // general msdf generation loop
    for (int y = 0; y < texture_height_; y++) {
        for (int x = 0; x < texture_width_; x++) {
            auto p = GetGlyphCoordinate(Vector2(x, y), bbox);
            auto res = GenerateMsdfPixel(shape, p);
            ClampArrayToRange(res, distance_range);

            int index = GetFlippedIndexFromCoordinate(x, y);
            output[index + 0] = MapDistanceToColorValue(res[0], distance_range);    // B
            output[index + 1] = MapDistanceToColorValue(res[1], distance_range);    // G
            output[index + 2] = MapDistanceToColorValue(res[2], distance_range);    // R
            output[index + 3] = 255;                                                         // A
        }
    }

    MsdfCollisionCorrection(output, collision_correction_threshold_);
}

std::array<double, 3> MsdfGenerator::GenerateMsdfPixel(const Shape &shape, const Vector2 &p) {
    // struct to hold the closest edge for each color
    struct {
        double min_distance = INFINITY;
        const EdgeHolder *edge = nullptr;
        double near_parameter = 0;
        double orthogonality = 0;
    } r, g, b;

    auto round = [](double d, int decimals) {
        return std::ceil(d * std::pow(10, decimals)) / std::pow(10, decimals);
    };

    auto is_closer = [round](const double d1, const double d2, const double orth1, const double orth2) {
        if (round(d1, 4) < round(d2, 4)) return true;
        else if (round(d1, 4) == round(d2, 4)) return orth1 > orth2;
        return false;
    };

    for (const Contour &c: shape.contours) {
        for (const EdgeHolder &e: c.edges) {
            double parameter = 0;
            auto distance = e->SignedDistance(p, parameter);
            auto orthogonality = e->GetOrthogonality(p, parameter);

            // only save edges that have a common color with the channel
            if ((int) e->color & (int) EdgeColor::Red && is_closer(std::abs(distance), std::abs(r.min_distance), orthogonality, r.orthogonality)) {
                r.min_distance = distance;
                r.edge = &e;
                r.near_parameter = parameter;
                r.orthogonality = orthogonality;
            }
            if ((int) e->color & (int) EdgeColor::Green && is_closer(std::abs(distance), std::abs(g.min_distance), orthogonality, g.orthogonality)) {
                g.min_distance = distance;
                g.edge = &e;
                g.near_parameter = parameter;
                g.orthogonality = orthogonality;
            }
            if ((int) e->color & (int) EdgeColor::Blue && is_closer(std::abs(distance), std::abs(b.min_distance), orthogonality, b.orthogonality)) {
                b.min_distance = distance;
                b.edge = &e;
                b.near_parameter = parameter;
                b.orthogonality = orthogonality;
            }
        }
    }

    std::array<double, 3> res = {
            r.edge ? (*r.edge)->SignedDistance(p, r.near_parameter) : INFINITY,
            g.edge ? (*g.edge)->SignedDistance(p, g.near_parameter) : INFINITY,
            b.edge ? (*b.edge)->SignedDistance(p, b.near_parameter) : INFINITY
    };

    return res;
}

double MsdfGenerator::GenerateSdfPixel(const Shape &shape, const Vector2 &p) {
    return shape.SignedDistance(p);
}

int MsdfGenerator::MapDistanceToColorValue(double distance, double distance_range) const {
    return (int) std::lround((distance / distance_range + 0.5f) * 255);
}

double MsdfGenerator::ClampDistanceToRange(double distance, double distance_range) const {
    double clamped = bx::clamp(distance, -distance_range / 2.0, distance_range / 2.0);
    return clamped;
}

void MsdfGenerator::ClampArrayToRange(std::array<double, 3> &array, double distance_range) {
    for (int i = 0; i < 3; i++) {
        array[i] = ClampDistanceToRange(array[i], distance_range);
    }
}

void MsdfGenerator::CalculateGlyphMetrics(FT_BBox_ bbox, GlyphInfo &out_glyph_info) const {
    out_glyph_info.width = texture_width_;
    out_glyph_info.height = texture_height_;
    out_glyph_info.advance_x = std::floor(face_->glyph->advance.x * (1.0 / font_scale_));
    out_glyph_info.advance_y = std::floor(face_->glyph->advance.y * (1.0 / font_scale_));
    out_glyph_info.offset_x = -bbox.xMin * (1.0 / font_scale_) - padding_;
    out_glyph_info.offset_y = -bbox.yMin * (1.0 / font_scale_) - padding_;
}

int MsdfGenerator::GetFlippedIndexFromCoordinate(int x, int y) const {
    return ((texture_height_ - y - 1) * texture_width_ + x) * 4; // flip over y axis
}

int MsdfGenerator::GetIndexFromCoordinate(int x, int y) const {
    return (y * texture_width_ + x) * 4;
}

Vector2 MsdfGenerator::GetGlyphCoordinate(Vector2 bitmap_coordinate, FT_BBox_ bbox) const {
    auto translation = Vector2(bbox.xMin, -bbox.yMin) + Vector2(padding_, padding_) * font_scale_;
    return Vector2((bitmap_coordinate.x + 0.5), (bitmap_coordinate.y + 0.5)) * font_scale_ - translation;
}

void MsdfGenerator::MsdfCollisionCorrection(uint8_t *map, int threshold) {
    for (int y = 0; y < texture_height_; y++) {
        for (int x = 0; x < texture_width_; x++) {
            int blue_jump = FindHighestDifferenceInNeighbours(x, y, map);
            int green_jump = FindHighestDifferenceInNeighbours(x, y, map + 1);
            int red_jump = FindHighestDifferenceInNeighbours(x, y, map + 2);

            if (HigherThanThreshold({blue_jump, green_jump, red_jump}, 2, threshold)) {
                int index = GetIndexFromCoordinate(x, y);
                int median = GetMedian(map[index], map[index + 1], map[index + 2]);
                map[index] = median;
                map[index + 1] = median;
                map[index + 2] = median;
            }
        }
    }
}

int MsdfGenerator::FindHighestDifferenceInNeighbours(int x, int y, uint8_t *map) {
    int index = GetIndexFromCoordinate(x, y);
    int right_index = GetIndexFromCoordinate(x + 1, y);
    int down_index = GetIndexFromCoordinate(x, y + 1);

    int right_diff = AreCellsOfDifferentSign(map[index], map[right_index]) ? std::abs(map[index] - map[right_index]) : 0;
    int down_diff = AreCellsOfDifferentSign(map[index], map[down_index]) ? std::abs(map[index] - map[down_index]) : 0;

    return std::max(right_diff, down_diff);
}

bool MsdfGenerator::HigherThanThreshold(const std::vector<int> &x, int how_many, int threshold) {
    int count = 0;
    for (const auto &i: x) {
        count += i > threshold;
    }
    return count >= how_many;
}

int MsdfGenerator::GetMedian(int a, int b, int c) {
    return a + b + c - std::min(a, std::min(b, c)) - std::max(a, std::max(b, c));
}

bool MsdfGenerator::AreCellsOfDifferentSign(int a, int b) {
    return (a - 127) * (b - 127) < 0;
}
