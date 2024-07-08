//
// Created by MihaelGolob on 8. 07. 2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/font_decomposition/EdgeHolder.h"

using namespace Catch;

TEST_CASE("EdgeSegment", "[EdgeSegment]") {
    SECTION("DistanceToLinear") {
        auto bezier = EdgeHolder({2, 2}, {7, 4});

        SECTION("tBetween0and1") {
            auto point = Vector2(3, 5);

            // execution
            double t;
            auto distance = bezier->Distance(point, t);

            // verification
            REQUIRE(t == Approx(0.379310));
            REQUIRE(distance == Approx(2.414039));
        }
        SECTION("tLessThan0") {
            auto point = Vector2(0.6, 2.6);

            // execution
            double t;
            auto distance = bezier->Distance(point, t);

            // verification
            REQUIRE(t == Approx(0));
            REQUIRE(distance == Approx(1.523154));
        }
    }

    SECTION("DistanceToQuadratic") {
        auto bezier = EdgeHolder({2, 2}, {4, 6}, {7, 4});
        SECTION("tBetween0and1") {
            // setup
            auto point = Vector2(3, 5);

            // execution
            double t;
            auto distance = bezier->Distance(point, t);

            // verification
//            REQUIRE(t == Approx());
            printf("warning: not testing for correct t value\n");
            REQUIRE(distance == Approx(0.569801));
        }
        SECTION("tMoreThan1") {
            // setup
            auto point = Vector2(8, 3);

            // execution
            double t;
            auto distance = bezier->Distance(point, t);

            // verification
//            REQUIRE(t == Approx(0));
            printf("warning: not testing for correct t value\n");
            REQUIRE(distance == Approx(1.414113));
        }
    }

    SECTION("DistanceToCubic") {
        auto bezier = EdgeHolder({2, 2}, {3, 5}, {5, 6}, {7, 4});
        SECTION("tBetween0and1") {
            // setup
            auto point = Vector2(4, 4);

            // execution
            double t;
            auto distance = bezier->Distance(point, t);

            // verification
            REQUIRE(distance == Approx(0.690453));
        }
        SECTION("tLessThan0") {
            auto point = Vector2(3, 0);
            
            double t;
            auto distance = bezier->Distance(point, t);
            
            REQUIRE(distance == Approx(2.236068));
        }
    }
}

