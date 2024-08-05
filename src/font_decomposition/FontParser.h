//
// Created by MihaelGolob on 5. 08. 2024.
//

#pragma once

#include "Vector2.h"
#include "../font_processing/TextBuffer.h"

class Shape;
class Contour;

class FontParser {
public:
    static Shape ParseFtFace(CodePoint code_point, FT_Face* face, double scale = 1 / 64.0);
    
private:
    static int FtMoveTo(const FT_Vector* to, void* user);
    static int FtLineTo(const FT_Vector* to, void* user);
    static int FtConicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int FtCubicTo(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
};

struct FtContext {
    double scale;
    Point2 position;
    Shape *shape;
    Contour *contour;
};

