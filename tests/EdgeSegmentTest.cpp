//
// Created by MihaelGolob on 8. 07. 2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/font_decomposition/EdgeHolder.h"

using namespace Catch;

TEST_CASE("EdgeSegment", "[EdgeSegment]") {
    SECTION("Linear") {
        SECTION("PointOnLinear") {
            auto bezier = EdgeHolder({2, 2}, {7, 4});
            SECTION("tBetween0and1") {
                auto point = bezier->GetPoint(0.6);
                // verification
                REQUIRE(point.x == Approx(5));
                REQUIRE(point.y == Approx(3.2));
            }

            SECTION("tEquals0") {
                auto point = bezier->GetPoint(0);
                // verification
                REQUIRE(point.x == Approx(2));
                REQUIRE(point.y == Approx(2));
            }

            SECTION("tEquals1") {
                auto point = bezier->GetPoint(1);
                // verification
                REQUIRE(point.x == Approx(7));
                REQUIRE(point.y == Approx(4));
            }
        }

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
            }SECTION("tLessThan0") {
                auto point = Vector2(0.6, 2.6);

                // execution
                double t;
                auto distance = bezier->Distance(point, t);

                // verification
                REQUIRE(t == Approx(0));
                REQUIRE(distance == Approx(1.523154));
            }
        }
    }
    
    SECTION("Quadratic") {
        SECTION("PointOnQuadratic") {
            auto bezier = EdgeHolder({2, 1}, {5, 5}, {15, 0});
            SECTION("tBetween0and1") {
                auto point = bezier->GetPoint(0.13);
                // verification
                REQUIRE(point.x == Approx(2.8983));
                REQUIRE(point.y == Approx(1.8879));
            }

            SECTION("tEquals0") {
                auto point = bezier->GetPoint(0);
                // verification
                REQUIRE(point.x == Approx(2));
                REQUIRE(point.y == Approx(1));
            }

            SECTION("tEquals1") {
                auto point = bezier->GetPoint(1);
                // verification
                REQUIRE(point.x == Approx(15));
                REQUIRE(point.y == Approx(0));
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
                REQUIRE(t == Approx(0.37306));
                REQUIRE(distance == Approx(1.059306));
            }SECTION("tMoreThan1") {
                // setup
                auto point = Vector2(8, 3);

                // execution
                double t;
                auto distance = bezier->Distance(point, t);

                // verification
                REQUIRE(t == Approx(1));
                REQUIRE(distance == Approx(1.414213));
            }
        }
    }
    
    SECTION("Cubic") {
        SECTION("PointOnCubic") {

            auto bezier = EdgeHolder({2, 2}, {3, 5}, {5, 6}, {7, 4});
            SECTION("tBetween0and1") {
                auto point = bezier->GetPoint(0.79);
                // verification
                REQUIRE(point.x == Approx(5.74926));
                REQUIRE(point.y == Approx(4.87236));
            }

            SECTION("tEquals0") {
                auto point = bezier->GetPoint(0);
                // verification
                REQUIRE(point.x == Approx(2));
                REQUIRE(point.y == Approx(2));
            }

            SECTION("tEquals1") {
                auto point = bezier->GetPoint(1);
                // verification
                REQUIRE(point.x == Approx(7));
                REQUIRE(point.y == Approx(4));
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
                REQUIRE(t == Approx(0.391215656));
                REQUIRE(distance == Approx(0.690649));
            }SECTION("tLessThan0") {
                auto point = Vector2(3, 0);

                double t;
                auto distance = bezier->Distance(point, t);

                REQUIRE(t == Approx(0));
                REQUIRE(distance == Approx(2.236067977));
            }
        }
    }
}

