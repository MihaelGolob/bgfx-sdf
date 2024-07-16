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
    for (auto& c : contours) {
        auto current_color = EdgeColor::White;
        if (c.edges.size() > 1) current_color = EdgeColor::Magenta;
        
        auto next_color = [&current_color]() {
            if (current_color == EdgeColor::Yellow) current_color = EdgeColor::Cyan;
            else current_color = EdgeColor::Yellow; 
        };
        
        auto previous_edge = EdgeHolder();
        for (auto& e : c.edges) {
            if (previous_edge.IsValid()) {
                const auto angle = EdgeSegment::GetAngleDeg(previous_edge, e, 1, 0);
                const auto edges_have_opposite_direction = previous_edge->GetDirection(1) * e->GetDirection(0) < 0;
                if (angle > max_angle || edges_have_opposite_direction) next_color();
            }
            
            e->color = current_color;
            previous_edge = e;
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
            
            if (fabs(distance) < fabs(min_distance)) {
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

double Shape::SignedPseudoDistance(const Vector2 &p) const {
    double min_distance = INFINITY;
    double max_orthogonality = 0;
    EdgeHolder closest_edge;

    for (const auto& c : contours) {
        for (const auto& e : c.edges) {
            double t;
            auto distance = e->SignedDistance(p, t);

            if (fabs(distance) < fabs(min_distance)) {
                min_distance = distance;
                max_orthogonality = e->GetOrthogonality(p, t);
                closest_edge = e;
            } else if (distance == min_distance) {
                const auto orthogonality = e->GetOrthogonality(p, t);
                if (orthogonality > max_orthogonality) {
                    // this is needed for correct sign if the distance is the same
                    max_orthogonality = orthogonality;
                    min_distance = distance;
                    closest_edge = e;
                }
            }
        }
    }
    
    return closest_edge->SignedPseudoDistance(p, min_distance);
}
