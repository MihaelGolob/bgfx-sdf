//
// Created by Mihael on 05/07/2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

using namespace Catch;

#include "../src/font_decomposition/Vector2.h"

TEST_CASE("Vector2Length2Test", "[Vector2]") {
    REQUIRE(Vector2{3,4}.Length2() == Approx(25));
    REQUIRE(Vector2{0,0}.Length2() == Approx(0));
    REQUIRE(Vector2{1,1}.Length2() == Approx(2));
    REQUIRE(Vector2{1,0}.Length2() == Approx(1));
    REQUIRE(Vector2{150,21}.Length2() == Vector2{21, 150}.Length2());
}