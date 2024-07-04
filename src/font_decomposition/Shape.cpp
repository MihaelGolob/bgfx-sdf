//
// Created by MihaelGolob on 3. 07. 2024.
//

#include "Shape.h"

Contour& Shape::AddEmptyContour() {
    contours.resize(contours.size() + 1);
    return contours.back();
}

void Shape::ApplyEdgeColoring(float max_angle) {
    // todo: make this more sophisticated
    for (auto& c : contours) {
        auto current_color = EdgeColor::White;
        if (c.edges.size() > 1) current_color = EdgeColor::Magenta;
        
        for (auto& e : c.edges) {
            e->color = current_color;
            if (current_color == EdgeColor::Yellow) current_color = EdgeColor::Cyan;
            else current_color = EdgeColor::Yellow;
        }
    }
}
