//
// Created by MihaelGolob on 3. 07. 2024.
//

#include "EdgeSegment.h"

// edge segment -------------------------------------
EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1) {
    return new LinearSegment(p0, p1);
}

EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2) {
    if ((p1 - p0).Cross(p2 - p1) == 0) return new LinearSegment(p0, p2);
    return new QuadraticSegment(p0, p1, p2);
}

EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) {
    if ((p1 - p0).Cross(p2 - p1) == 0 && (p2 - p1).Cross(p3 - p2) == 0) return new LinearSegment(p0, p3);
    if (p1*1.5 - p0*0.5 == p2*1.5 - p3*0.5) return new QuadraticSegment(p0, p2 - p1, p3);
    return new CubicSegment(p0, p1, p2, p3);
}


// linear segment -----------------------------------
LinearSegment::LinearSegment(const Vector2 &p0, const Vector2 &p1) : points{p0, p1} {}


// quadratic segment --------------------------------
QuadraticSegment::QuadraticSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2) : points{p0, p1, p2} {}


// cubic segment ------------------------------------
CubicSegment::CubicSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) : points{p0, p1, p2, p3} {}
