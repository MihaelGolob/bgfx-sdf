//
// Created by Mihael on 05/07/2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using namespace Catch;

#include "../src/font_decomposition/Vector2.h"

TEST_CASE("Vector2Length2Test", "[Vector2]") {
    REQUIRE(Vector2{3, 4}.Length2() == Approx(25));
    REQUIRE(Vector2{0, 0}.Length2() == Approx(0));
    REQUIRE(Vector2{1, 1}.Length2() == Approx(2));
    REQUIRE(Vector2{1, 0}.Length2() == Approx(1));
    REQUIRE(Vector2{150, 21}.Length2() == Vector2{21, 150}.Length2());
}

TEST_CASE("Vector2LengthTest", "[Vector2]") {
    REQUIRE(Vector2{3, 4}.Length() == Approx(5));
    REQUIRE(Vector2{0, 0}.Length() == Approx(0));
    REQUIRE(Vector2{1, 1}.Length() == Approx(1.41421356));
    REQUIRE(Vector2{1, 0}.Length() == Approx(1));
    REQUIRE(Vector2{19, 52}.Length() == Approx(55.36244));
}

TEST_CASE("Vector2CrossTest", "[Vector2]") {
    REQUIRE(Vector2{1, 0}.Cross(Vector2{5, 0}) == Approx(0));
    REQUIRE(Vector2{1, 5}.Cross(Vector2{1, 5}) == Approx(0)); // a x a = 0
    REQUIRE(Vector2{4.2, 1.5}.Cross(Vector2{52.1, 12.3}) == -Approx(Vector2{52.1, 12.3}.Cross(Vector2{4.2, 1.5}))); // a x b = -b x a
    REQUIRE((Vector2{51, 2.25} * 4.1).Cross(Vector2{1, 2} * 2.4) == Approx(4.1 * 2.4 * Vector2{51, 2.25}.Cross(Vector2{1, 2}))); // (k1 * a) x (k2 * b) = k1 * k2 * (a x b)
}

TEST_CASE("Vector2NormalizeTest", "[Vector2]") {
    REQUIRE(Vector2{3, 4}.Normalize() == Vector2{0.6, 0.8});
    REQUIRE(Vector2{0, 0}.Normalize() == Vector2{0, 0});
    REQUIRE(Vector2{1, 1}.Normalize() == Vector2{0.70710678, 0.70710678});
    REQUIRE(Vector2{1, 0}.Normalize() == Vector2{1, 0});
    
    const auto normalized = Vector2{19, 52}.Normalize();
    REQUIRE(normalized.x == Approx(0.34319295));
    REQUIRE(normalized.y == Approx(0.93926492));
}