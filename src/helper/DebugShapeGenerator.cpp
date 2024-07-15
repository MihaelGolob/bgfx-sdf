//
// Created by MihaelGolob on 15. 07. 2024.
//

#include "DebugShapeGenerator.h"
Shape DebugShapeGenerator::GetGlyphI() {
    // returns a glyph "I" shape in the 4th quadrant
    // bbox of shape is [0, 50] in x and [0, 100] in y
    auto shape = Shape();

    // outer contour
    auto outer = Contour();
    outer.AddEdge(EdgeHolder({20, 10}, {30, 10}));
    outer.AddEdge(EdgeHolder({30, 10}, {30, 90}));
    outer.AddEdge(EdgeHolder({30, 90}, {20, 90}));
    outer.AddEdge(EdgeHolder({20, 90}, {20, 10}));
    shape.contours.emplace_back(outer);

    return shape;
}

Shape DebugShapeGenerator::GetGlyphA() {
    // returns a glyph "A" shape in the 4th quadrant
    // bbox of shape is [0, 50] in x and [0, 100] in y
    auto shape = Shape();

    // outer contour
    auto outer = Contour();
    outer.AddEdge(EdgeHolder({15, 10}, {35, 10}));
    outer.AddEdge(EdgeHolder({35, 10}, {40, 90}));
    outer.AddEdge(EdgeHolder({40, 90}, {35, 90}));
    outer.AddEdge(EdgeHolder({35, 90}, {30, 50}));
    outer.AddEdge(EdgeHolder({30, 50}, {20, 50}));
    outer.AddEdge(EdgeHolder({20, 50}, {15, 90}));
    outer.AddEdge(EdgeHolder({15, 90}, {10, 90}));
    outer.AddEdge(EdgeHolder({10, 90}, {15, 10}));
    shape.contours.emplace_back(outer);

    auto inner = Contour();
    inner.AddEdge(EdgeHolder({30, 45}, {20, 45}));
    inner.AddEdge(EdgeHolder({20, 45}, {25, 35}));
    inner.AddEdge(EdgeHolder({25, 35}, {30, 45}));
    shape.contours.emplace_back(inner);

    return shape;
}