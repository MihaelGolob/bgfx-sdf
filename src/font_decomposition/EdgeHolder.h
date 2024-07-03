//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include "EdgeSegment.h"

class EdgeHolder {
public:
    EdgeHolder() : edge_() {};
    explicit EdgeHolder(EdgeSegment* edge) : edge_(edge) {};
    EdgeHolder(Point2 p0, Point2 p1) : edge_(EdgeSegment::CreateEdgeSegment(p0, p1)) {};
    EdgeHolder(Point2 p0, Point2 p1, Point2 p2) : edge_(EdgeSegment::CreateEdgeSegment(p0, p1, p2)) {};
    EdgeHolder(Point2 p0, Point2 p1, Point2 p2, Point2 p3) : edge_(EdgeSegment::CreateEdgeSegment(p0, p1, p2, p3)) {};
    
    ~EdgeHolder();
    
private:
    EdgeSegment* edge_;
};
