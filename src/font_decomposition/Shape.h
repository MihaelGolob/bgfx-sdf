//
// Created by MihaelGolob on 3. 07. 2024.
//
#pragma once

#include <vector>
#include "Contour.h"

class Shape {
public:
    std::vector<Contour> contours;
    Contour& AddEmptyContour();
};
