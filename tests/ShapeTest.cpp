//
// Created by MihaelGolob on 8. 07. 2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/font_decomposition/Shape.h"

using namespace Catch;

TEST_CASE("Shape", "[Shape]") {
    SECTION("Shape made of linear segments") {
        // create a "box inside a box" shape
        auto shape = Shape();

        // outer contour
        auto outer = Contour();
        outer.AddEdge(EdgeHolder({2, 2}, {2, 7}));
        outer.AddEdge(EdgeHolder({2, 7}, {7, 7}));
        outer.AddEdge(EdgeHolder({7, 7}, {7, 2}));
        outer.AddEdge(EdgeHolder({7, 2}, {2, 2}));
        shape.contours.emplace_back(outer);

        // inner contour
        auto inner = Contour();
        inner.AddEdge(EdgeHolder({3, 3}, {6, 3}));
        inner.AddEdge(EdgeHolder({6, 3}, {6, 6}));
        inner.AddEdge(EdgeHolder({6, 6}, {3, 6}));
        inner.AddEdge(EdgeHolder({3, 6}, {3, 3}));
        shape.contours.emplace_back(inner);

        SECTION("Signed distance inside shape") {
            auto point = Vector2(4, 2.3);
            auto signed_distance = shape.SignedDistance(point);
            REQUIRE(signed_distance == Approx(0.3));
        }

        SECTION("Signed distance inside on corner") {
            auto point = Vector2(2.5, 6.5);
            auto signed_distance = shape.SignedDistance(point);
            REQUIRE(signed_distance == Approx(0.5));
        }

        SECTION("Signed distance outside shape") {
            auto point = Vector2(4, 1.3);
            auto signed_distance = shape.SignedDistance(point);
            REQUIRE(signed_distance == Approx(-0.7));
        }

        SECTION("Signed distance outside but on corner") {
            auto point = Vector2(9, 8);
            auto signed_distance = shape.SignedDistance(point);
            REQUIRE(signed_distance == Approx(-2.236067));
        }

        SECTION("Signed distance outside but inner contour") {
            auto point = Vector2(5.5, 4);
            auto signed_distance = shape.SignedDistance(point);
            REQUIRE(signed_distance == Approx(-0.5));
        }
        
        SECTION("Signed distance on edge") {
            auto point = Vector2(3, 3);
            auto signed_distance = shape.SignedDistance(point);
            REQUIRE(signed_distance == Approx(0));
        }
    }
}