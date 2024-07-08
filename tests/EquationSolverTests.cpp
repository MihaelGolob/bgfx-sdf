//
// Created by MihaelGolob on 8. 07. 2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/helper/EquationSolver.h"

using namespace Catch;

TEST_CASE("EquationSolver", "[EquationSolver]") {
    SECTION("CubicEquation") {
        SECTION("RootsFrom1To3") {
            double a = 1, b = -6, c = 11, d = -6;
            auto roots = SolveCubicEquation(a, b, c, d);
            REQUIRE(roots.size() == 3);
            REQUIRE(roots[0] == Approx(1));
            REQUIRE(roots[1] == Approx(2));
            REQUIRE(roots[2] == Approx(3));
        }
        SECTION("Roots0To2") {
            double a = 1, b = -3, c = 2, d = 0;
            auto roots = SolveCubicEquation(a, b, c, d);
            REQUIRE(roots.size() == 3);
            REQUIRE(roots[0] == Approx(0));
            REQUIRE(roots[1] == Approx(1));
            REQUIRE(roots[2] == Approx(2));
        }
        SECTION("Roots1AndDouble2") {
            double a = 1, b = -5, c = 8, d = -4;
            auto roots = SolveCubicEquation(a, b, c, d);
            REQUIRE(roots.size() == 3);
            REQUIRE(roots[0] == Approx(1));
            REQUIRE(roots[1] == Approx(2));
            REQUIRE(roots[2] == Approx(2));
        }
        SECTION("Roots1AndDoubleIrrational") {
            double a = 1, b = -4, c = 2, d = 3;
            auto roots = SolveCubicEquation(a, b, c, d);
            REQUIRE(roots.size() == 1);
            REQUIRE(roots[0] == Approx(3));
        }
    }
}