//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include "Vector2.h"

class EdgeSegment {
public:
    // constructors
    static EdgeSegment* CreateEdgeSegment(const Vector2& p0, const Vector2& p1);
    static EdgeSegment* CreateEdgeSegment(const Vector2& p0, const Vector2& p1, const Vector2& p2);
    static EdgeSegment* CreateEdgeSegment(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);
    
    virtual ~EdgeSegment() = default;
};

class LinearSegment : public EdgeSegment {
    Vector2 points[2];
public:
    LinearSegment(const Vector2 &p0, const Vector2 &p1);
};

class QuadraticSegment : public EdgeSegment {
    Vector2 points[3];
public:
    QuadraticSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2);
};

class CubicSegment : public EdgeSegment {
    Vector2 points[4];
public:
    CubicSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);
};
