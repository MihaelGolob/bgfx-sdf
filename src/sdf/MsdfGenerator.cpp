//
// Created by MihaelGolob on 2. 07. 2024.
//

#include <freetype/ftoutln.h>
#include <cmath>

#include "MsdfGenerator.h"
#include "../utilities.h"

void MsdfGenerator::Init(FT_Face face, uint32_t font_size, uint32_t padding) {
    face_ = face;
    font_size_ = font_size;
    padding_ = padding;
    
    texture_height_ = font_size_ + 2 * padding;
    texture_width_ = font_size_ + 2 * padding;

    font_scale_ = CalculateFontScale();
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
    auto shape = ParseFtFace(code_point, 1.0);
    shape.ApplyPreprocessing();
    
    FT_BBox_ bbox{};
    FT_Outline_Get_CBox(&face_->glyph->outline, &bbox);
    auto distance_range = std::max(bbox.xMax - bbox.xMin, bbox.yMax - bbox.yMin);
    
    CalculateGlyphMetrics(face_, bbox, glyph_info);
    
    for (int y = 0; y < texture_height_; y++) {
        for (int x = 0; x < texture_width_; x++) {
            auto translation = Vector2(bbox.xMin, -bbox.yMin) + Vector2(padding_, padding_) * font_scale_;
            Vector2 p = Vector2((x + 0.5), (y + 0.5)) * font_scale_ - translation;
            
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
    auto shape = ParseFtFace(code_point, 1.0);
    shape.ApplyPreprocessing();
    shape.ApplyEdgeColoring(3.0);

    FT_BBox_ bbox{};
    FT_Outline_Get_CBox(&face_->glyph->outline, &bbox);
    auto distance_range = std::max(bbox.xMax - bbox.xMin, bbox.yMax - bbox.yMin);

    CalculateGlyphMetrics(face_, bbox, glyph_info);

    // general msdf generation loop
    for (int y = 0; y < texture_height_; y++) {
        for (int x = 0; x < texture_width_; x++) {
            auto translation = Vector2(bbox.xMin, -bbox.yMin) + Vector2(padding_, padding_) * font_scale_;
            Vector2 p = Vector2((x + 0.5), (y + 0.5)) * font_scale_ - translation;

            auto res = GenerateMsdfPixel(shape, p);
            ClampArrayToRange(res, distance_range);

            int index = GetFlippedIndexFromCoordinate(x, y);
            output[index + 0] = MapDistanceToColorValue(res[0], distance_range);    // B
            output[index + 1] = MapDistanceToColorValue(res[1], distance_range);    // G
            output[index + 2] = MapDistanceToColorValue(res[2], distance_range);    // R
            output[index + 3] = 255;                                                         // A
        }
    }
}

std::array<double, 3> MsdfGenerator::GenerateMsdfPixel(const Shape &shape, const Vector2 &p) {
    // struct to hold the closest edge for each color
    struct {
        double min_distance = INFINITY;
        const EdgeHolder *edge = nullptr;
        double near_parameter = 0;
        double orthogonality = 0;
    } red, green, blue;

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
            if ((int) e->color & (int) EdgeColor::Red && is_closer(std::abs(distance), std::abs(red.min_distance), orthogonality, red.orthogonality)) {
                red.min_distance = distance;
                red.edge = &e;
                red.near_parameter = parameter;
                red.orthogonality = orthogonality;
            }
            if ((int) e->color & (int) EdgeColor::Green && is_closer(std::abs(distance), std::abs(green.min_distance), orthogonality, green.orthogonality)) {
                green.min_distance = distance;
                green.edge = &e;
                green.near_parameter = parameter;
                green.orthogonality = orthogonality;
            }
            if ((int) e->color & (int) EdgeColor::Blue && is_closer(std::abs(distance), std::abs(blue.min_distance), orthogonality, blue.orthogonality)) {
                blue.min_distance = distance;
                blue.edge = &e;
                blue.near_parameter = parameter;
                blue.orthogonality = orthogonality;
            }
        }
    }

    std::array<double, 3> res = {
            red.edge ? (*red.edge)->SignedPseudoDistance(p, red.near_parameter) : INFINITY,
            green.edge ? (*green.edge)->SignedPseudoDistance(p, green.near_parameter) : INFINITY,
            blue.edge ? (*blue.edge)->SignedPseudoDistance(p, blue.near_parameter) : INFINITY
    };

    return res;
}

double MsdfGenerator::GenerateSdfPixel(const Shape &shape, const Vector2 &p) {
    return shape.SignedDistance(p);
}

Shape MsdfGenerator::ParseFtFace(CodePoint code_point, double scale) {
    auto glyph_index = FT_Get_Char_Index(face_, code_point);
    if (FT_Load_Glyph(face_, glyph_index, FT_LOAD_NO_SCALE)) {
        PrintError("Failed to load glyph");
        return {};
    }

    Shape output{};

    FtContext context{};
    context.scale = scale;
    context.shape = &output;

    FT_Outline_Funcs_ ft_functions{};
    ft_functions.move_to = &FtMoveTo;
    ft_functions.line_to = &FtLineTo;
    ft_functions.conic_to = &FtConicTo;
    ft_functions.cubic_to = &FtCubicTo;
    ft_functions.shift = 0;
    ft_functions.delta = 0;

    FT_Outline_Decompose(&face_->glyph->outline, &ft_functions, &context);

    return output;
}

int MsdfGenerator::FtMoveTo(const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext *>(user);
    if (!context->contour || context->contour->HasEdges()) {
        context->contour = &context->shape->AddEmptyContour();
    }
    context->position = Point2(*to, context->scale);
    return 0;
}

int MsdfGenerator::FtLineTo(const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext *>(user);
    auto end_point = Point2(*to, context->scale);
    if (end_point != context->position) {
        context->contour->AddEdge(EdgeHolder(context->position, end_point));
        context->position = end_point;
    }

    return 0;
}

int MsdfGenerator::FtConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext *>(user);
    auto control_point = Point2(*control, context->scale);
    auto end_point = Point2(*to, context->scale);
    if (end_point != context->position) {
        auto temp = EdgeHolder(context->position, control_point, end_point);
        context->contour->AddEdge(temp);
        context->position = end_point;
    }

    return 0;
}

int MsdfGenerator::FtCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext *>(user);
    auto control_point1 = Point2(*control1, context->scale);
    auto control_point2 = Point2(*control2, context->scale);
    auto end_point = Point2(*to, context->scale);
    if (end_point != context->position || (control_point1 - end_point).Cross(control_point2 - end_point)) {
        context->contour->AddEdge(EdgeHolder(context->position, control_point1, control_point2, end_point));
        context->position = end_point;
    }

    return 0;
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

void MsdfGenerator::CalculateGlyphMetrics(FT_Face const &face, FT_BBox_ bbox, GlyphInfo &out_glyph_info) const {
    out_glyph_info.width = texture_width_;
    out_glyph_info.height = texture_height_;
    out_glyph_info.advance_x = std::floor(face_->glyph->advance.x * (1.0 / font_scale_));
    out_glyph_info.advance_y = std::floor(face_->glyph->advance.y * (1.0 / font_scale_));
    out_glyph_info.offset_x = bbox.xMin * (1.0 / font_scale_) - padding_;
    out_glyph_info.offset_y = -bbox.yMin * (1.0 / font_scale_) - padding_;
}

int MsdfGenerator::GetFlippedIndexFromCoordinate(int x, int y) const {
    return ((texture_height_ - y - 1) * texture_width_ + x) * 4; // flip over y axis
}
