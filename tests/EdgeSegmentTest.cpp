//
// Created by MihaelGolob on 8. 07. 2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/font_decomposition/EdgeHolder.h"

using namespace Catch;

TEST_CASE("EdgeSegment", "[EdgeSegment]") {
    SECTION("Linear") {
        SECTION("PointOn") {
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

        SECTION("DistanceToPoint") {
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
        
        SECTION("SignedDistanceToPoint") {
            auto bezier = EdgeHolder({1,4}, {6,6});
            auto positive_point = Vector2(4, 3);
            auto negative_point = Vector2(3, 5);
            
            double t;
            auto inside_distance = bezier->SignedDistance(positive_point, t);
            auto outside_distance = bezier->SignedDistance(negative_point, t);
            
            REQUIRE(inside_distance == Approx(bezier->Distance(positive_point, t)));
            REQUIRE(outside_distance == Approx(-bezier->Distance(negative_point, t)));
        }
    }
    
    SECTION("Quadratic") {
        SECTION("PointOn") {
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

        SECTION("DistanceToPoint") {
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
        
        SECTION("SignedDistanceToPoint") {
            auto bezier = EdgeHolder({1,4}, {6,6}, {3,1});
            auto positive_point = Vector2(3, 3);
            auto negative_point = Vector2(5, 5);
            
            double t;
            auto inside_distance = bezier->SignedDistance(positive_point, t);
            auto outside_distance = bezier->SignedDistance(negative_point, t);
            
            REQUIRE(inside_distance == Approx(bezier->Distance(positive_point, t)));
            REQUIRE(outside_distance == Approx(-bezier->Distance(negative_point, t)));
        }
    }
    
    SECTION("Cubic") {
        SECTION("PointOn") {

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

        SECTION("DistanceToPoint") {
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
        
        SECTION("SignedDistanceToPoint") {
            auto bezier = EdgeHolder({1,4}, {6,6}, {3,1}, {5, 5});
            auto positive_point = Vector2(4, 3);
            auto negative_point = Vector2(3, 5);
            
            double t;
            auto inside_distance = bezier->SignedDistance(positive_point, t);
            auto outside_distance = bezier->SignedDistance(negative_point, t);
            
            REQUIRE(inside_distance == Approx(bezier->Distance(positive_point, t)));
            REQUIRE(outside_distance == Approx(-bezier->Distance(negative_point, t)));
        }
    }
}

