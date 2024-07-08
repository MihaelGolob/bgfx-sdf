//
// Created by MihaelGolob on 3. 07. 2024.
//

#include <cmath>
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

double Shape::Distance(const Vector2 &p) const {
    double min_distance = INFINITY;
    
    for (const auto& c : contours) {
        for (const auto& e : c.edges) {
            double t;
            auto distance = e->Distance(p, t);
            if (distance < min_distance) {
                min_distance = distance;
            }
        }
    }
    
    return min_distance;
}

double Shape::SignedDistance(const Vector2 &p) const {
    double min_distance = INFINITY;
    double max_orthogonality = 0;
    
    for (const auto& c : contours) {
        for (const auto& e : c.edges) {
            double t;
            auto distance = e->SignedDistance(p, t);
            
            if (distance < min_distance) {
                min_distance = distance;
                max_orthogonality = e->GetOrthogonality(p, t);
            } else if (distance == min_distance) {
                const auto orthogonality = e->GetOrthogonality(p, t);
                if (orthogonality > max_orthogonality) {
                    // this is needed for correct sign if the distance is the same
                    max_orthogonality = orthogonality;
                    min_distance = distance;
                }
            }
        }
    }
    
    return min_distance;
}
