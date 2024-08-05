//
// Created by MihaelGolob on 5. 08. 2024.
//

#include "FontParser.h"

#include <freetype/ftimage.h>
#include "Contour.h"
#include "Shape.h"
#include "../utilities.h"

Shape FontParser::ParseFtFace(CodePoint code_point, FT_Face* face, double scale) {
    auto glyph_index = FT_Get_Char_Index(*face, code_point);
    if (FT_Load_Glyph(*face, glyph_index, FT_LOAD_NO_SCALE)) {
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

    FT_Outline_Decompose(&(*face)->glyph->outline, &ft_functions, &context);

    return output;
}

int FontParser::FtMoveTo(const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext *>(user);
    if (!context->contour || context->contour->HasEdges()) {
        context->contour = &context->shape->AddEmptyContour();
    }
    context->position = Point2(*to, context->scale);
    return 0;
}

int FontParser::FtLineTo(const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext *>(user);
    auto end_point = Point2(*to, context->scale);
    if (end_point != context->position) {
        context->contour->AddEdge(EdgeHolder(context->position, end_point));
        context->position = end_point;
    }

    return 0;
}

int FontParser::FtConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
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

int FontParser::FtCubicTo(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, void *user) {
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
