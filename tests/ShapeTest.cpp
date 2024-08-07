﻿//
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

        SECTION("Signed Distance") {
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

        auto custom = Contour();
        custom.AddEdge(EdgeHolder({2, 7}, {5, 9}));
        shape.contours.emplace_back(custom);

        SECTION("Signed Pseudo Distance") {
            SECTION("It calculates pseudo distance to nearest edge") {
                auto point = Vector2(10, 2.1);
                auto distance = shape.SignedPseudoDistance(point);
                REQUIRE(distance == Approx(-3));
            }

            SECTION("Check pseudo distance") {
                auto point = Vector2(6, 10);
                auto distance = shape.SignedPseudoDistance(point);
                REQUIRE(distance == Approx(-0.277350));
            }

            SECTION("inside on corner") {
                auto point = Vector2(2.5, 6.5);
                auto signed_distance = shape.SignedPseudoDistance(point);
                REQUIRE(signed_distance == Approx(0.5));
            }
        }
    }

    SECTION("Glyph t") {
        auto shape = Shape();
        auto contour = Contour();
        contour.AddEdge(EdgeHolder({4, 1}, {5, 1}));
        contour.AddEdge(EdgeHolder({5, 1}, {6, 2}));
        contour.AddEdge(EdgeHolder({6, 2}, {6, 0}));
        contour.AddEdge(EdgeHolder({6, 0}, {2, 0}));
        contour.AddEdge(EdgeHolder({2, 0}, {1, 1}));
        contour.AddEdge(EdgeHolder({1, 1}, {1, 7}));
        contour.AddEdge(EdgeHolder({1, 7}, {0, 7}));
        contour.AddEdge(EdgeHolder({0, 7}, {0, 8}));
        contour.AddEdge(EdgeHolder({0, 8}, {1, 9}));
        contour.AddEdge(EdgeHolder({1, 9}, {2, 11}));
        contour.AddEdge(EdgeHolder({2, 11}, {3, 9}));
        contour.AddEdge(EdgeHolder({3, 9}, {6, 9}));
        contour.AddEdge(EdgeHolder({6, 9}, {6, 7}));
        contour.AddEdge(EdgeHolder({6, 7}, {3, 7}));
        contour.AddEdge(EdgeHolder({3, 7}, {3, 2}));
        contour.AddEdge(EdgeHolder({3, 2}, {4, 2}));
        contour.AddEdge(EdgeHolder({4, 2}, {4, 1}));
        shape.contours.emplace_back(contour);

        SECTION("(7,3) is outside") {
            auto point = Vector2(7, 3);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(8,2) is outside") {
            auto point = Vector2(8, 2);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(5, 3) is outside") {
            auto point = Vector2(5, 3);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(8,7) is outside") {
            auto point = Vector2(8, 7);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(-2,4) is outside") {
            auto point = Vector2(-2, 4);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(7, 10) is outside") {
            auto point = Vector2(7, 10);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);

        }

        SECTION("(3,1) is inside") {
            auto point = Vector2(3, 1);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }
    }

    SECTION("Glyph high quality t") {
        Shape shape;
        Contour contour;
        contour.AddEdge(EdgeHolder({9, 3}, {11, 3}));
        contour.AddEdge(EdgeHolder({11, 3}, {12, 3}, {12, 4}));
        contour.AddEdge(EdgeHolder({12, 4}, {12, 0}));
        contour.AddEdge(EdgeHolder({12, 0}, {10, 0}));
        contour.AddEdge(EdgeHolder({10, 0}, {8, 0}));
        contour.AddEdge(EdgeHolder({8, 0}, {5, 0}));
        contour.AddEdge(EdgeHolder({5, 0}, {4, 0}, {3, 1}));
        contour.AddEdge(EdgeHolder({3, 1}, {3, 5}));
        contour.AddEdge(EdgeHolder({3, 5}, {3, 13}));
        contour.AddEdge(EdgeHolder({3, 13}, {0, 13}));
        contour.AddEdge(EdgeHolder({0, 13}, {0, 15}));
        contour.AddEdge(EdgeHolder({0, 15}, {3, 17}));
        contour.AddEdge(EdgeHolder({3, 17}, {4, 21}));
        contour.AddEdge(EdgeHolder({4, 21}, {7, 21}));
        contour.AddEdge(EdgeHolder({7, 21}, {7, 17}));
        contour.AddEdge(EdgeHolder({7, 17}, {12, 17}));
        contour.AddEdge(EdgeHolder({12, 17}, {12, 13}));
        contour.AddEdge(EdgeHolder({12, 13}, {7, 13}));
        contour.AddEdge(EdgeHolder({7, 13}, {7, 5}));
        contour.AddEdge(EdgeHolder({7, 5}, {7, 4}, {8, 3}));
        contour.AddEdge(EdgeHolder({8, 3}, {9, 3}));
        shape.contours.emplace_back(contour);

        SECTION("(14, 6) is outside") {
            auto point = Vector2(14, 6);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(11,6) is outside") {
            auto point = Vector2(11, 6);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }
    }

    SECTION("Gyph d") {
        auto shape = Shape();
        auto outline = Contour();
        outline.AddEdge(EdgeHolder({4, 0}, {2, 0}, {1, 1}));
        outline.AddEdge(EdgeHolder({1, 1}, {0, 2}, {0, 4}));
        outline.AddEdge(EdgeHolder({0, 4}, {0, 6}, {1, 7}));
        outline.AddEdge(EdgeHolder({1, 7}, {2, 9}, {4, 9}));
        outline.AddEdge(EdgeHolder({4, 9}, {5, 8}));
        outline.AddEdge(EdgeHolder({5, 8}, {6, 8}));
        outline.AddEdge(EdgeHolder({6, 8}, {6, 7}));
        outline.AddEdge(EdgeHolder({6, 7}, {6, 8}));
        outline.AddEdge(EdgeHolder({6, 8}, {6, 9}));
        outline.AddEdge(EdgeHolder({6, 9}, {6, 13}));
        outline.AddEdge(EdgeHolder({6, 13}, {8, 13}));
        outline.AddEdge(EdgeHolder({8, 13}, {8, 0}));
        outline.AddEdge(EdgeHolder({8, 0}, {7, 0}));
        outline.AddEdge(EdgeHolder({7, 0}, {6, 1}));
        outline.AddEdge(EdgeHolder({6, 1}, {6, 0}, {5, 0}));
        outline.AddEdge(EdgeHolder({5, 0}, {4, 0}));
        shape.contours.emplace_back(outline);

        auto hole = Contour();
        hole.AddEdge(EdgeHolder({4, 1}, {5, 1}, {6, 2}));
        hole.AddEdge(EdgeHolder({6, 2}, {6, 4}));
        hole.AddEdge(EdgeHolder({6, 4}, {6, 6}));
        hole.AddEdge(EdgeHolder({6, 6}, {5, 7}, {4, 7}));
        hole.AddEdge(EdgeHolder({4, 7}, {3, 6}));
        hole.AddEdge(EdgeHolder({3, 6}, {3, 4}));
        hole.AddEdge(EdgeHolder({3, 4}, {3, 2}));
        hole.AddEdge(EdgeHolder({3, 2}, {4, 1}));
        shape.contours.emplace_back(hole);

        shape.ApplyPreprocessing();

        SECTION("(9,5) is outside") {
            auto point = Vector2(9, 5);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(6,0) is outside") {
            auto point = Vector2(6, 0);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(8.5, 6.5) is outside") {
            auto point = Vector2(8.5, 6.5);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(6,7) is inside") {
            auto point = Vector2(6, 7);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(7,7) is inside") {
            auto point = Vector2(7, 7);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(5.5, 7) is inside") {
            auto point = Vector2(5.5, 7);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }
    }

    SECTION("Glyph r") {
        auto shape = Shape();
        auto contour = Contour();
        contour.AddEdge(EdgeHolder({6, 8}, {6, 6}));
        contour.AddEdge(EdgeHolder({6, 6}, {5, 6}));
        contour.AddEdge(EdgeHolder({5, 6}, {4, 6}));
        contour.AddEdge(EdgeHolder({4, 6}, {3, 5}));
        contour.AddEdge(EdgeHolder({3, 5}, {3, 4}));
        contour.AddEdge(EdgeHolder({3, 4}, {3, 0}));
        contour.AddEdge(EdgeHolder({3, 0}, {1, 0}));
        contour.AddEdge(EdgeHolder({1, 0}, {1, 9}));
        contour.AddEdge(EdgeHolder({1, 9}, {3, 9}));
        contour.AddEdge(EdgeHolder({3, 9}, {3, 7}));
        contour.AddEdge(EdgeHolder({3, 7}, {4, 8}));
        contour.AddEdge(EdgeHolder({4, 8}, {5, 8}));
        contour.AddEdge(EdgeHolder({5, 8}, {5, 9}, {6, 8}));
        shape.contours.emplace_back(contour);
        shape.ApplyPreprocessing();

        SECTION("(2,4) is inside") {
            auto point = Vector2(2, 4);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(2,8) is inside") {
            auto point = Vector2(2, 8);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(5,7) is inside") {
            auto point = Vector2(5, 7);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(3.5,6) is inside") {
            auto point = Vector2(3.5, 6);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(3.5, 6.5) is inside") {
            auto point = Vector2(3.5, 6.5);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }

        SECTION("(4,5) is outside") {
            auto point = Vector2(4, 5);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(3.5,8) is outside") {
            auto point = Vector2(3.5, 8);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance < 0);
        }
    }

    SECTION("Glyph , pseudo distance") {
        auto shape = Shape();
        auto contour = Contour();
        contour.AddEdge(EdgeHolder({444, 238}, {459, 215}));
        contour.AddEdge(EdgeHolder({459, 215}, {441, 143}, {412, 60}));
        contour.AddEdge(EdgeHolder({412, 60}, {384, -23}, {350, -106}));
        contour.AddEdge(EdgeHolder({350, -106}, {317, -189}, {283, -264}));
        contour.AddEdge(EdgeHolder({283, -264}, {63, -264}));
        contour.AddEdge(EdgeHolder({63, -264}, {83, -183}, {102, -94}));
        contour.AddEdge(EdgeHolder({102, -94}, {121, -5}, {137, 81}));
        contour.AddEdge(EdgeHolder({137, 81}, {154, 167}, {164, 238}));
        contour.AddEdge(EdgeHolder({164, 238}, {444, 238}));
        shape.contours.emplace_back(contour);

        SECTION("(50, 300) is outside") {
            auto point = Vector2(50, 300);
            auto distance = shape.SignedPseudoDistance(point);

            REQUIRE(distance < 0);
        }

        SECTION("(-50, 400) is outside") {
            auto point = Vector2(-120, 350);
            auto distance = shape.SignedPseudoDistance(point);

            REQUIRE(distance < 0);
        }
    }

    SECTION("Glyph V") {
        auto shape = Shape();
        auto contour = Contour();
        contour.AddEdge(EdgeHolder({1331, 1462}, {834, 0}));
        contour.AddEdge(EdgeHolder({834, 0}, {496, 0}));
        contour.AddEdge(EdgeHolder({496, 0}, {0, 1462}));
        contour.AddEdge(EdgeHolder({0, 1462}, {313, 1462}));
        contour.AddEdge(EdgeHolder({313, 1462}, {588, 592}));
        contour.AddEdge(EdgeHolder({588, 592}, {595, 570}, {611, 509}));
        contour.AddEdge(EdgeHolder({611, 509}, {627, 449}, {643, 381}));
        contour.AddEdge(EdgeHolder({643, 381}, {660, 314}, {666, 270}));
        contour.AddEdge(EdgeHolder({666, 270}, {672, 314}, {687, 381}));
        contour.AddEdge(EdgeHolder({687, 381}, {703, 448}, {718, 508}));
        contour.AddEdge(EdgeHolder({718, 508}, {734, 569}, {741, 592}));
        contour.AddEdge(EdgeHolder({741, 592}, {1018, 1462}));
        contour.AddEdge(EdgeHolder({1018, 1462}, {1331, 1462}));
        shape.contours.emplace_back(contour);
        
        SECTION("(600, 200) is inside") {
            auto point = Vector2(600, 200);
            auto distance = shape.SignedDistance(point);

            REQUIRE(distance > 0);
        }
    }
}