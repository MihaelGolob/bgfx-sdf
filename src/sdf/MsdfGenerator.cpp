﻿//
// Created by MihaelGolob on 2. 07. 2024.
//

#include <freetype/ftoutln.h>
#include "MsdfGenerator.h"
#include "../utilities.h"

void MsdfGenerator::BakeGlyphMsdf(CodePoint code_point, FT_Face face, uint8_t *output) {
    auto shape = ParseFtFace(code_point, face);
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
    auto context = reinterpret_cast<FtContext*>(user);
    if (!context->contour || context->contour->HasEdges()) {
        context->contour = &context->shape->AddEmptyContour();
    }
    context->position = Point2(*to, context->scale);
    return 0;
}

int MsdfGenerator::FtLineTo(const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext*>(user);
    auto end_point = Point2(*to, context->scale);
    if (end_point != context->position) {
        context->contour->AddEdge(EdgeHolder(context->position, end_point));
        context->position = end_point;
    }
    
    return 0;
}

int MsdfGenerator::FtConicTo(const FT_Vector *control, const FT_Vector *to, void *user) {
    auto context = reinterpret_cast<FtContext*>(user);
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
    auto context = reinterpret_cast<FtContext*>(user);
    auto control_point1 = Point2(*control1, context->scale);
    auto control_point2 = Point2(*control2, context->scale);
    auto end_point = Point2(*to, context->scale);
    if (end_point != context->position || (control_point1 - end_point).Cross(control_point2 - end_point)) {
        context->contour->AddEdge(EdgeHolder(context->position, control_point1, control_point2, end_point));
        context->position = end_point;
    }
    
    return 0;
}
