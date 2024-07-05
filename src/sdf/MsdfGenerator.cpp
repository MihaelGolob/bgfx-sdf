//
// Created by MihaelGolob on 2. 07. 2024.
//

#include <freetype/ftoutln.h>
#include <cmath>

#include "MsdfGenerator.h"
#include "../utilities.h"

void MsdfGenerator::Init(FT_Face face, uint32_t pixel_height, int16_t padding) {
    face_ = face;
    scale_ = pixel_height / (float) face->units_per_EM;
    padding_ = padding;
    distance_range_ = 30.0f;
}

void MsdfGenerator::BakeGlyphMsdf(CodePoint code_point, GlyphInfo &out_glyph_info, uint8_t *output) {
    auto shape = ParseFtFace(code_point, face_);
    shape.ApplyEdgeColoring(3.0);

    CalculateGlyphMetrics(face_, out_glyph_info);
    const int width = out_glyph_info.width;
    const int height = out_glyph_info.height;

    // general msdf generation loop
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < height; x++) {
            Vector2 p = Vector2((x + 0.5) / width, (y + 0.5) / height); // todo: apply some sort of transformation
            auto res = GeneratePixel(shape, p);

            output[y * width + x] = MapDistanceToColorValue(res[0]);
            output[width * height + y * width + x] = MapDistanceToColorValue(res[1]);
            output[2 * width * height + y * width + x] = MapDistanceToColorValue(res[2]);
        }
    }
}

std::array<double, 3> MsdfGenerator::GeneratePixel(const Shape &shape, const Vector2 &p) {
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
            if ((int)e->color & (int)EdgeColor::Red && abs_distance < std::abs(red.min_distance)) {
                red.min_distance = distance;
                red.edge = &e;
                red.near_parameter = parameter;
            }
            if ((int)e->color & (int)EdgeColor::Green) {
                green.min_distance = distance;
                green.edge = &e;
                green.near_parameter = parameter;
            }
            if ((int)e->color & (int)EdgeColor::Blue) {
                blue.min_distance = distance;
                blue.edge = &e;
                blue.near_parameter = parameter;
            }
        }
    }

    return {
        red.edge ? (*red.edge)->PseudoDistance(red.min_distance, p, red.near_parameter) : INFINITY,
        green.edge ? (*green.edge)->PseudoDistance(green.min_distance, p, green.near_parameter) : INFINITY,
        blue.edge ? (*blue.edge)->PseudoDistance(blue.min_distance, p, blue.near_parameter) : INFINITY
    };
}

Shape MsdfGenerator::ParseFtFace(CodePoint code_point, FT_Face face) {
    if (FT_Load_Glyph(face, code_point, FT_LOAD_NO_SCALE)) {
        PrintError("Failed to load glyph");
        return {};
    }

    Shape output{};

    FtContext context{};
    double scale = 1.0; // todo: support different scale types
    context.scale = scale;
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

void MsdfGenerator::CalculateGlyphMetrics(FT_Face const &face, GlyphInfo &out_glyph_info) {
    // todo: fix these metrics!
    out_glyph_info.offset_x = 0;
    out_glyph_info.offset_y = -10;
    out_glyph_info.width = face_->glyph->metrics.width * scale_;
    out_glyph_info.height = face_->glyph->metrics.height * scale_;
    out_glyph_info.advance_x = bx::round(face_->glyph->advance.x * scale_);
    out_glyph_info.advance_y = bx::round(face_->glyph->advance.y * scale_);
}

int MsdfGenerator::MapDistanceToColorValue(float distance) const {
    return (int) std::lround((distance / distance_range_ + 0.5f) * 255);
}
