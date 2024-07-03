//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include <freetype/ftoutln.h>

class Vector2 {
public:
    float x, y;
    Vector2() = default; 
    Vector2(FT_Vector v, float scale);
    Vector2(float x, float y);
    
    // math operators
    [[nodiscard]] float Cross(const Vector2& other) const;
    
    // operator definitions
    bool operator== (const Vector2& other) const;
    bool operator!= (const Vector2& other) const;
    Vector2 operator+ (const Vector2& other) const;
    Vector2 operator- (const Vector2& other) const;
    Vector2 operator* (float&& scalar) const;
};

// A vector can also represent just a point in 2D space
typedef Vector2 Point2;
