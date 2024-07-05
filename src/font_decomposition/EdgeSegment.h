//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include "Vector2.h"

enum class EdgeColor {
    Black, Red, Green, Yellow, Blue, Magenta, Cyan, White
};

class EdgeSegment {
public:
    // constructors
    static EdgeSegment *CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1);
    static EdgeSegment *CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2);
    static EdgeSegment *CreateEdgeSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);

    virtual ~EdgeSegment() = default;

    [[nodiscard]] virtual double Distance(const Vector2 &p, double &t) const = 0;
    
    [[nodiscard]] virtual Vector2 GetPoint(double t) const = 0;
    [[nodiscard]] virtual Vector2 GetDirection(double t) const = 0;
    
    [[nodiscard]] virtual double SignedDistance(const Vector2 &p, double &t) const;
    [[nodiscard]] virtual double PseudoDistance(float distance, const Vector2 &p, double &t) const {return 0;};

    EdgeColor color = EdgeColor::White;
};

class LinearSegment : public EdgeSegment {
    Vector2 points_[2];
public:
    LinearSegment(const Vector2 &p0, const Vector2 &p1);

    [[nodiscard]] double Distance(const Vector2 &p, double &t) const override;

    [[nodiscard]] Vector2 GetPoint(double t) const override;
    [[nodiscard]] Vector2 GetDirection(double t) const override;
};

class QuadraticSegment : public EdgeSegment {
    Vector2 points_[3];
public:
    QuadraticSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2);

    [[nodiscard]] double Distance(const Vector2 &p, double &t) const override;

    [[nodiscard]] Vector2 GetPoint(double t) const override;
    [[nodiscard]] Vector2 GetDirection(double t) const override;
};

class CubicSegment : public EdgeSegment {
    Vector2 points_[4];
public:
    CubicSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);

    [[nodiscard]] double Distance(const Vector2 &p, double &t) const override;

    [[nodiscard]] Vector2 GetPoint(double t) const override;
    [[nodiscard]] Vector2 GetDirection(double t) const override;
};
