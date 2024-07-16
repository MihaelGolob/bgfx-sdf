//
// Created by MihaelGolob on 2. 07. 2024.
//

#include <freetype/ftoutln.h>
#include <cmath>

#include "MsdfGenerator.h"
#include "../utilities.h"

void MsdfGenerator::Init(FT_Face face, uint32_t font_size) {
    face_ = face;
    font_size_ = font_size;
    distance_range_ = font_size_ / 2.0; // todo: is this good enough? 

    FT_Set_Pixel_Sizes(face, 0, font_size_);
}

void MsdfGenerator::BakeGlyphSdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output) {
    auto shape = ParseFtFace(code_point, face_);
    shape.ApplyEdgeColoring(3.0);

    CalculateGlyphMetrics(face_, glyph_info);

    int width = glyph_info.width;
    int height = glyph_info.height;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto translation = Vector2(face_->glyph->metrics.horiBearingX >> 6, -((face_->glyph->metrics.height >> 6) - (face_->glyph->metrics.horiBearingY >> 6)));
            Vector2 p = Vector2((x + 0.5), (y + 0.5)) + translation;
            auto distance = GenerateSdfPixel(shape, p);
            auto mapped_distance = MapDistanceToColorValue(distance);
            
            auto index = ((height - y - 1) * width + x) * 4; // flip over y axis
            
            output[index] = mapped_distance;
            output[index + 1] = mapped_distance;
            output[index + 2] = mapped_distance;
            output[index + 3] = 255;
        }
    }
}

void MsdfGenerator::BakeGlyphMsdf(CodePoint code_point, GlyphInfo &glyph_info, uint8_t *output) {
    auto shape = ParseFtFace(code_point, face_);
    shape.ApplyEdgeColoring(3.0);

    CalculateGlyphMetrics(face_, glyph_info);

    int width = glyph_info.width;
    int height = glyph_info.height;
    // general msdf generation loop
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            auto translation = Vector2(face_->glyph->metrics.horiBearingX >> 6, -((face_->glyph->metrics.height >> 6) - (face_->glyph->metrics.horiBearingY >> 6)));
            Vector2 p = Vector2((x + 0.5), (y + 0.5)) + translation;
            
            auto res = GenerateMsdfPixel(shape, p);
            auto index = ((height - y - 1) * width + x) * 4; // flip over y axis

            output[index] = MapDistanceToColorValue(res[0]);        // B
            output[index + 1] = MapDistanceToColorValue(res[1]);    // G
            output[index + 2] = MapDistanceToColorValue(res[2]);    // R
            output[index + 3] = 255;                                         // A
        }
    }
}

std::array<double, 3> MsdfGenerator::GenerateMsdfPixel(const Shape &shape, const Vector2 &p) {
    // struct to hold the closest edge for each color
    struct {
        float min_distance = INFINITY;
        const EdgeHolder *edge = nullptr;
        double near_parameter = 0;
    } red, green, blue;

    for (const Contour &c: shape.contours) {
        for (const EdgeHolder &e: c.edges) {
            double parameter = 0;
            auto distance = e->SignedDistance(p, parameter);
            auto abs_distance = std::abs(distance);

            // only save edges that have a common color with the channel
            if ((int) e->color & (int) EdgeColor::Red && abs_distance < std::abs(red.min_distance)) {
                red.min_distance = distance;
                red.edge = &e;
                red.near_parameter = parameter;
            }
            if ((int) e->color & (int) EdgeColor::Green && abs_distance < std::abs(green.min_distance)) {
                green.min_distance = distance;
                green.edge = &e;
                green.near_parameter = parameter;
            }
            if ((int) e->color & (int) EdgeColor::Blue && abs_distance < std::abs(blue.min_distance)) {
                blue.min_distance = distance;
                blue.edge = &e;
                blue.near_parameter = parameter;
            }
        }
    }

    std::array<double, 3> res = {
            red.edge ? (*red.edge)->SignedPseudoDistance(p, red.near_parameter) : INFINITY,
            green.edge ? (*green.edge)->SignedPseudoDistance(p, green.near_parameter) : INFINITY,
            blue.edge ? (*blue.edge)->SignedPseudoDistance(p, blue.near_parameter) : INFINITY
    };
    ClampArrayToRange(res);
    return res;
}

double MsdfGenerator::GenerateSdfPixel(const Shape &shape, const Vector2 &p) {
    return shape.SignedDistance(p);
}


Shape MsdfGenerator::ParseFtFace(CodePoint code_point, FT_Face face) {
    auto glyph_index = FT_Get_Char_Index(face, code_point);
    if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT)) {
        PrintError("Failed to load glyph");
        return {};
    }

    Shape output{};

    FtContext context{};
    context.scale = 1.0 / 64.0; // default for freetype
    context.shape = &output;

    FT_Outline_Funcs_ ft_functions{};
    ft_functions.move_to = &FtMoveTo;
    ft_functions.line_to = &FtLineTo;
    ft_functions.conic_to = &FtConicTo;
    ft_functions.cubic_to = &FtCubicTo;
    ft_functions.shift = 0;
    ft_functions.delta = 0;

    FT_Outline_Decompose(&face->glyph->outline, &ft_functions, &context);

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

int MsdfGenerator::MapDistanceToColorValue(float distance) const {
    return (int) std::lround((distance / distance_range_ + 0.5f) * 255);
}

float MsdfGenerator::ClampDistanceToRange(float distance) const {
    float clamped = bx::clamp(distance, -distance_range_, distance_range_);
    return clamped;
}

void MsdfGenerator::ClampArrayToRange(std::array<double, 3> &array) {
    for (int i = 0; i < 3; i++) {
        array[i] = ClampDistanceToRange(array[i]);
    }
}

void MsdfGenerator::CalculateGlyphMetrics(FT_Face const &face, GlyphInfo &out_glyph_info) {
    out_glyph_info.width = face->glyph->metrics.width >> 6;
    out_glyph_info.height = face->glyph->metrics.height >> 6;

    out_glyph_info.advance_x = face->glyph->advance.x >> 6;
    out_glyph_info.advance_y = face->glyph->advance.y >> 6;

    out_glyph_info.offset_x = face->glyph->metrics.horiBearingX >> 6;
    out_glyph_info.offset_y = -face->glyph->metrics.horiBearingY >> 6;

    out_glyph_info.bitmap_scale = 1;
}
