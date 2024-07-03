//
// Created by MihaelGolob on 3. 07. 2024.
//

#include "Vector2.h"

Vector2::Vector2(const FT_Vector v, float scale) {
    x = v.x * scale;
    y = v.y * scale;
}

Vector2::Vector2(float x, float y) : x{x}, y{y} {}

// operator definitions
bool Vector2::operator== (const Vector2& other) const {
    return x == other.x && y == other.y;
}

bool Vector2::operator!= (const Vector2& other) const {
    return x != other.x || y != other.y;
}

float Vector2::Cross(const Vector2 &other) const {
    return x * other.y - y * other.x;
}

Vector2 Vector2::operator+(const Vector2 &other) const {
    return {x + other.x, y + other.y};
}

Vector2 Vector2::operator-(const Vector2 &other) const {
    return {x - other.x, y - other.y};
}

Vector2 Vector2::operator*(float&& scalar) const {
    return {x * scalar, y * scalar};
}

