//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include <freetype/ftoutln.h>
#include <functional>
#include <string>

class Vector2 {
public:
    float x, y;
    Vector2() = default; 
    Vector2(FT_Vector v, float scale);
    Vector2(float x, float y);
    
    // math operators
    [[nodiscard]] float Cross(const Vector2& other) const;
    [[nodiscard]] double Length2() const;
    [[nodiscard]] double Length() const;
    Vector2 Normalize();
    
    // operator definitions
    bool operator== (const Vector2& other) const;
    bool operator!= (const Vector2& other) const;
    Vector2 operator+ (const Vector2& other) const;
    Vector2 operator- (const Vector2& other) const;
    Vector2 operator* (float&& scalar) const;
    double operator* (const Vector2& other) const;
    
    struct HashFunction {
        size_t operator()(const Vector2& v) const {
            auto x_hash = std::hash<int>()(v.x);
            auto y_hash = std::hash<int>()(v.y);
            return x_hash ^ y_hash;
        }
    };
    
    std::string ToString() const;
};

// A vector can also represent just a point in 2D space
typedef Vector2 Point2;
