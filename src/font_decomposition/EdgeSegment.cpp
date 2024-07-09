//
// Created by MihaelGolob on 3. 07. 2024.
//

#include <algorithm>
#include <cmath>
#include "EdgeSegment.h"
#include "../helper/EquationSolver.h"

// edge segment -------------------------------------
EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1) {
    return new LinearSegment(p0, p1);
}

EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2) {
    if ((p1 - p0).Cross(p2 - p1) == 0) return new LinearSegment(p0, p2); // cross product is zero for parallel vectors
    return new QuadraticSegment(p0, p1, p2);
}

EdgeSegment *EdgeSegment::CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) {
    if ((p1 - p0).Cross(p2 - p1) == 0 && (p2 - p1).Cross(p3 - p2) == 0) return new LinearSegment(p0, p3);
    if (p1 * 1.5 - p0 * 0.5 == p2 * 1.5 - p3 * 0.5) return new QuadraticSegment(p0, p2 - p1, p3);
    return new CubicSegment(p0, p1, p2, p3);
}

double EdgeSegment::SignedDistance(const Vector2 &p, double &t) const {
    const auto distance = Distance(p, t);
    const auto sign = (GetPoint(t) - p).Cross(GetDirection(t)) > 0 ? -1 : 1;
    return distance * sign;
}

double EdgeSegment::GetOrthogonality(const Vector2 &p, double t) const {
    const auto direction = GetDirection(t);
    const auto x = (p - GetPoint(t)).Normalize();
    return direction.Cross(x);
}


// linear segment -----------------------------------
LinearSegment::LinearSegment(const Vector2 &p0, const Vector2 &p1) : points_{p0, p1} {}

double LinearSegment::Distance(const Vector2 &p, double &t) const {
    t = (p - points_[0]) * (points_[1] - points_[0]) / (points_[1] - points_[0]).Length2();
    t = std::clamp(t, 0.0, 1.0);

    return (GetPoint(t) - p).Length();
}

Vector2 LinearSegment::GetPoint(double t) const {
    return points_[0] + (points_[1] - points_[0]) * t;
}

Vector2 LinearSegment::GetDirection(double t) const {
    return (points_[1] - points_[0]).Normalize();
}

double LinearSegment::SignedPseudoDistance(const Vector2 &p, double &t) const {
    t = (p - points_[0]) * (points_[1] - points_[0]) / (points_[1] - points_[0]).Length2();

    return (GetPoint(t) - p).Length();
}

// quadratic segment --------------------------------
QuadraticSegment::QuadraticSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2) : points_{p0, p1, p2} {}

double QuadraticSegment::Distance(const Vector2 &p, double &t) const {
    auto candidates = CandidateTValues(p);
    // also check the endpoints
    candidates.emplace_back(0);
    candidates.emplace_back(1);

    // find the closest root
    double min_distance = INFINITY;
    for (const auto can: candidates) {
        if (can < 0 || can > 1) continue; // skip invalid roots (outside the segment range)

        const auto dist = (GetPoint(can) - p).Length();
        if (dist < min_distance) {
            min_distance = dist;
            t = can;
        }
    }

    return min_distance;
}

double QuadraticSegment::SignedPseudoDistance(const Vector2 &p, double &t) const {
    auto candidates = CandidateTValues(p);

    double min_distance = INFINITY;
    for (const auto can: candidates) {
        const auto dist = (GetPoint(can) - p).Length();
        if (dist < min_distance) {
            min_distance = dist;
            t = can;
        }
    }

    return min_distance;
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

std::vector<double> QuadraticSegment::CandidateTValues(const Vector2 &p) const {
    // helper variables
    const auto p0 = p - points_[0];
    const auto p1 = points_[1] - points_[0];
    const auto p2 = points_[2] - points_[1] * 2 + points_[0];

    // equation coefficients
    const auto a = p2 * p2;
    const auto b = 3 * (p1 * p2);
    const auto c = 2 * (p1 * p1) - p2 * p0;
    const auto d = -(p1 * p0);

    return SolveCubicEquation(a, b, c, d);
}

// cubic segment ------------------------------------
CubicSegment::CubicSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3) : points_{p0, p1, p2, p3} {}

double CubicSegment::Distance(const Vector2 &p, double &t) const {
    auto candidates = CandidateTValues(p);
    // also check the endpoints
    candidates.emplace_back(0);
    candidates.emplace_back(1);

    // find the closest root
    double min_distance = INFINITY;
    for (const auto can: candidates) {
        if (can < 0 || can > 1) continue; // skip invalid roots (outside the segment range)

        const auto dist = (GetPoint(can) - p).Length();
        if (dist < min_distance) {
            min_distance = dist;
            t = can;
        }
    }

    return min_distance;
}

double CubicSegment::SignedPseudoDistance(const Vector2 &p, double &t) const {
    auto candidates = CandidateTValues(p);

    // find the closest root
    double min_distance = INFINITY;
    for (const auto can: candidates) {
        const auto dist = (GetPoint(can) - p).Length();
        if (dist < min_distance) {
            min_distance = dist;
            t = can;
        }
    }

    return min_distance;
}

Vector2 CubicSegment::GetPoint(double t) const {
    return points_[0]
           + (points_[1] - points_[0]) * 3 * t
           + (points_[2] - points_[1] * 2 + points_[0]) * 3 * t * t
           + (points_[3] - points_[2] * 3 + points_[1] * 3 - points_[0]) * t * t * t;
}

Vector2 CubicSegment::GetDirection(double t) const {
    auto direction = (points_[3] - points_[2] * 3 + points_[1] * 3 - points_[0]) * 3 * t * t
                     + (points_[2] - points_[1] * 2 + points_[0]) * 6 * t
                     + (points_[1] - points_[0]) * 3;
    return direction.Normalize();
}

std::vector<double> CubicSegment::CandidateTValues(const Vector2 &p) const {
    // helper variables
    const auto p0 = p - points_[0];
    const auto p1 = points_[1] - points_[0];
    const auto p2 = points_[2] - points_[1] * 2 + points_[0];
    const auto p3 = points_[3] - points_[2] * 3 + points_[1] * 3 - points_[0];

    // equation coefficients
    const auto a = p3 * p3;
    const auto b = 5 * (p2 * p3);
    const auto c = 4 * (p1 * p3) + 6 * (p2 * p2);
    const auto d = 9 * (p1 * p2) - p3 * p0;
    const auto e = 3 * (p1 * p1) - 2 * (p2 * p0);
    const auto f = -(p1 * p0);

    return SolveQuinticEquation(a, b, c, d, e, f);
}
