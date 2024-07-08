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
            double a = 2, b = -5, c = -2, d = 25;
            auto roots = SolveCubicEquation(a, b, c, d);
            REQUIRE(roots.size() == 1);
            REQUIRE(roots[0] == Approx(-1.820578));
        }
    }
    
    SECTION("QuinticEquation", "[EquationSolver]") {
        SECTION("UniqueRoots") {
            double a = 1, b = 27.4, c = -910.75, d = 6905.3, e = -16116.45, f = 1543.5;
            auto roots = SolveQuinticEquation(a, b, c, d, e, f);
            REQUIRE(roots.size() == 5);
            REQUIRE(roots[0] == Approx(-49));
            REQUIRE(roots[1] == Approx(0.1));
            REQUIRE(roots[2] == Approx(5));
            REQUIRE(roots[3] == Approx(6));
            REQUIRE(roots[4] == Approx(10.5));
        }
        SECTION("Only1RealRoot") {
            double a = 1, b = 1, c = 0, d = 1, e = 0, f = 1;
            auto roots = SolveQuinticEquation(a, b, c, d, e, f);
            REQUIRE(roots.size() == 1);
            REQUIRE(roots[0] == Approx(-1.57015));
        }
        SECTION("Roots3RealAnd2Imaginary") {
            double a = 1, b = 4, c = -1, d = -1, e = 0, f = -5;
            auto roots = SolveQuinticEquation(a, b, c, d, e, f);
            REQUIRE(roots.size() == 3);
            REQUIRE(roots[0] == Approx(-4.16582));
            REQUIRE(roots[1] == Approx(-1.139267));
            REQUIRE(roots[2] == Approx(1.10323));
        }
    }
}