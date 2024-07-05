//
// Created by MihaelGolob on 3. 07. 2024.
//

#include <algorithm>
#include <cmath>
#include "EdgeSegment.h"

// edge segment -------------------------------------
EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1) {
    return new LinearSegment(p0, p1);
}

EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2) {
    if ((p1 - p0).Cross(p2 - p1) == 0) return new LinearSegment(p0, p2); // cross product is zero for parallel vectors
    return new QuadraticSegment(p0, p1, p2);
}

EdgeSegment *
EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) {
    if ((p1 - p0).Cross(p2 - p1) == 0 && (p2 - p1).Cross(p3 - p2) == 0) return new LinearSegment(p0, p3);
    if (p1 * 1.5 - p0 * 0.5 == p2 * 1.5 - p3 * 0.5) return new QuadraticSegment(p0, p2 - p1, p3);
    return new CubicSegment(p0, p1, p2, p3);
}

double EdgeSegment::SignedDistance(const Vector2 &p, double &t) const {
    const auto distance = Distance(p, t);
    const auto sign = (GetPoint(t) - p).Cross(GetDirection(t)) > 0 ? 1 : -1;
    return distance * sign;
}


// linear segment -----------------------------------
LinearSegment::LinearSegment(const Vector2 &p0, const Vector2 &p1) : points_{p0, p1} {}

double LinearSegment::Distance(const Vector2 &p, double &t) const {
    t = (p - points_[0]) * (points_[1] - points_[0]) / (points_[1] - points_[0]).Norm2();
    t = std::clamp(t, 0.0, 1.0);

    return (GetPoint(t) - p).Norm();
}

double LinearSegment::PseudoDistance(float distance, const Vector2 &p, double &t) const {
    t = (p - points_[0]) * (points_[1] - points_[0]) / (points_[1] - points_[0]).Norm2();

    return (GetPoint(t) - p).Norm();
}

Vector2 LinearSegment::GetPoint(double t) const {
    return points_[0] + (points_[1] - points_[0]) * t;
}

Vector2 LinearSegment::GetDirection(double t) const {
    return (points_[1] - points_[0]).Normalize();
}

// quadratic segment --------------------------------
QuadraticSegment::QuadraticSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2) : points_{p0, p1, p2} {}

double QuadraticSegment::PseudoDistance(float distance, const Vector2 &p, double &t) const {
    // todo: implement
    return 0;
}

double QuadraticSegment::Distance(const Vector2 &p, double &t) const {
    // todo: implement
    return 0;
}

Vector2 QuadraticSegment::GetPoint(double t) const {
    return points_[0]
           + (points_[1] - points_[0]) * 2 * t
           + (points_[2] - points_[1] * 2 + points_[0]) * t * t;
}

Vector2 QuadraticSegment::GetDirection(double t) const {
    auto direction = (points_[2] - points_[1] * 2 + points_[0]) * 2 * t
                     + (points_[1] - points_[0]) * 2;
    return direction.Normalize();
}


// cubic segment ------------------------------------
CubicSegment::CubicSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) : points_{p0, p1, p2, p3} {}

double CubicSegment::PseudoDistance(float distance, const Vector2 &p, double &t) const {
    // todo: implement
    return 0;
}

Vector2 CubicSegment::GetPoint(double t) const {
    return points_[0]
           + (points_[1] - points_[0]) * 3 * t
           + (points_[2] - points_[1] * 2 + points_[0]) * 3 * t * t
           + (points_[3] - points_[2] * 3 + points_[1] * 3 - points_[0]) * t * t * t;
}

double CubicSegment::Distance(const Vector2 &p, double &t) const {
    // todo: implement
    return 0;
}

Vector2 CubicSegment::GetDirection(double t) const {
    auto direction = (points_[3] - points_[2] * 3 + points_[1] * 3 - points_[0]) * 3 * t * t
                     + (points_[2] - points_[1] * 2 + points_[0]) * 6 * t
                     + (points_[1] - points_[0]) * 3;
    return direction.Normalize();
}
