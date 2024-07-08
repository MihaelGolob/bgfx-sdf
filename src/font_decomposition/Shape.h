//
// Created by MihaelGolob on 3. 07. 2024.
//
#pragma once

#include <vector>
#include "Contour.h"

class Shape {
public:
    Contour& AddEmptyContour();
    
    void ApplyEdgeColoring(float max_angle);
    [[nodiscard]] double Distance(const Vector2 &p) const;
    [[nodiscard]] double SignedDistance(const Vector2 &p) const;
    
public:
    std::vector<Contour> contours;
};
