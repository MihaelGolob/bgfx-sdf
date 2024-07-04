//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include "Vector2.h"

enum class EdgeColor { Black, Red, Green, Yellow, Blue, Magenta, Cyan, White };

class EdgeSegment {
public:
    // constructors
    static EdgeSegment* CreateEdgeSegment(const Vector2& p0, const Vector2& p1);
    static EdgeSegment* CreateEdgeSegment(const Vector2& p0, const Vector2& p1, const Vector2& p2);
    static EdgeSegment* CreateEdgeSegment(const Vector2& p0, const Vector2& p1, const Vector2& p2, const Vector2& p3);
    
    virtual ~EdgeSegment() = default;
    
    [[nodiscard]] virtual float SignedDistance(const Vector2& p, double parameter) const = 0;
    [[nodiscard]] virtual float PseudoDistance(float distance, const Vector2&p, double parameter) const = 0;
    
    EdgeColor color = EdgeColor::White;
};

class LinearSegment : public EdgeSegment {
    Vector2 points_[2];
public:
    LinearSegment(const Vector2 &p0, const Vector2 &p1);
    
    // todo: implement
    [[nodiscard]] float SignedDistance(const Vector2 &p, double parameter) const override {return 0;}
    [[nodiscard]] float PseudoDistance(float distance, const Vector2&p, double parameter) const override {return 0;}
};

class QuadraticSegment : public EdgeSegment {
    Vector2 points_[3];
public:
    QuadraticSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2);
    
    // todo: implement
    [[nodiscard]] float SignedDistance(const Vector2 &p, double parameter) const override {return 0;}
    [[nodiscard]] float PseudoDistance(float distance, const Vector2&p, double parameter) const override {return 0;}
};

class CubicSegment : public EdgeSegment {
    Vector2 points_[4];
public:
    CubicSegment(const Vector2 &p0, const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);
    
    // todo: implement
    [[nodiscard]] float SignedDistance(const Vector2 &p, double parameter) const override {return 0;} 
    [[nodiscard]] float PseudoDistance(float distance, const Vector2&p, double parameter) const override {return 0;}
};
