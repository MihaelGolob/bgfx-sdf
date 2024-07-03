//
// Created by MihaelGolob on 3. 07. 2024.
//

#include "Shape.h"

Contour& Shape::AddEmptyContour() {
    contours.resize(contours.size() + 1);
    return contours.back();
}
