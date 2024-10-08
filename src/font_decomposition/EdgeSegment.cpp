﻿//
// Created by MihaelGolob on 3. 07. 2024.
//

#include <algorithm>
#include <cmath>
#include "EdgeSegment.h"
#include "../helper/EquationSolver.h"
#include "../utilities.h"

#define PI 3.14159265359

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
    return distance * GetSign(p, t);
}

double EdgeSegment::GetOrthogonality(const Vector2 &p, double t) const {
    const auto direction = GetDirection(t);
    const auto x = (p - GetPoint(t)).Normalize();
    return std::abs(direction.Cross(x));
}

int EdgeSegment::GetSign(const Vector2 &p, double &t) const {
    if (t < 0.0 || t > 1.0) {
        PrintError("Edge segment sign should be calculated for t in [0, 1].");
    }

    return (GetPoint(t) - p).Cross(GetDirection(t)) > 0 ? -1 : 1;
}

double EdgeSegment::GetAngleDeg(const EdgeSegment *edge1, const EdgeSegment *edge2, double t1, double t2) {
    return std::asin(edge1->GetDirection(t1).Cross(edge2->GetDirection(t2))) * 180 / PI;
}

double EdgeSegment::DistanceToPseudoDistance(const Vector2 &p, double t, double distance) const {
    if (t < 0) {
        auto dir = GetDirection(0);
        auto aq = p - GetPoint(0);
        if (aq * dir < 0) {
            double pseudo_dist = aq.Cross(dir);
            if (abs(pseudo_dist) <= abs(distance)) {
                return pseudo_dist;
            }
        }
    } else if (t > 1) {
        auto dir = GetDirection(1);
        auto bq = p - GetPoint(1);
        if (bq * dir > 0) {
            double pseudo_dist = bq.Cross(dir);
            if (abs(pseudo_dist) <= abs(distance)) {
                return pseudo_dist;
            }
        }
    }

    return distance;
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

    double x = std::clamp(t, 0.0, 1.0);
    return (GetPoint(t) - p).Length() * GetSign(p, x);
}

std::string LinearSegment::ToString() const {
    std::string res;
    res.append("[");
    for (int i = 0; i < 2; i++) {
        res.append(points_[i].ToString());
        if (i != 1) res.append(",");
    }
    res.append("]");

    return res;
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

    double x = std::clamp(t, 0.0, 1.0);
    return min_distance * GetSign(p, x);
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

std::string QuadraticSegment::ToString() const {
    std::string res;
    res.append("[");
    for (int i = 0; i < 3; i++) {
        res.append(points_[i].ToString());
        if (i != 2) res.append(",");
    }
    res.append("]");

    return res;
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

    double x = std::clamp(t, 0.0, 1.0);
    return min_distance * GetSign(p, x);
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

std::string CubicSegment::ToString() const {
    std::string res;
    res.append("[");
    for (int i = 0; i < 4; i++) {
        res.append(points_[i].ToString());
        if (i != 3) res.append(",");
    }
    res.append("]");

    return res;
}
