﻿//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include "EdgeSegment.h"

class EdgeHolder {
public:
    EdgeHolder() : edge() {};
    explicit EdgeHolder(EdgeSegment* edge) : edge(edge) {};
    EdgeHolder(Point2 p0, Point2 p1) : edge(EdgeSegment::CreateEdgeSegment(p0, p1)) {};
    EdgeHolder(Point2 p0, Point2 p1, Point2 p2) : edge(EdgeSegment::CreateEdgeSegment(p0, p1, p2)) {};
    EdgeHolder(Point2 p0, Point2 p1, Point2 p2, Point2 p3) : edge(EdgeSegment::CreateEdgeSegment(p0, p1, p2, p3)) {};
    
    ~EdgeHolder();
    void ToString() const;
    
private:
    EdgeSegment* edge;
};
