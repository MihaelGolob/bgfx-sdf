//
// Created by MihaelGolob on 25. 07. 2024.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../src/font_decomposition/EdgeSegment.h"

using namespace Catch;

TEST_CASE("EdgeColor", "[EdgeColor]") {
    SECTION("Color comparisons") {
        SECTION("Blue & Red equals black") {
            REQUIRE(((int)EdgeColor::Blue & (int)EdgeColor::Red) == (int)EdgeColor::Black);
        }
        
        SECTION("Blue & Green equals black") {
            REQUIRE(((int)EdgeColor::Blue & (int)EdgeColor::Green) == (int)EdgeColor::Black);
        }
        
        SECTION("Red & Green equals black") {
            REQUIRE(((int)EdgeColor::Red & (int)EdgeColor::Green) == (int)EdgeColor::Black);
        }
        
        SECTION("Blue & Green & Red equals black") {
            REQUIRE(((int)EdgeColor::Blue & (int)EdgeColor::Green & (int)EdgeColor::Red) == (int)EdgeColor::Black);
        }
        
        SECTION("Yellow tests") {
            SECTION("Yellow & Red equals red") {
                REQUIRE(((int)EdgeColor::Yellow & (int)EdgeColor::Red) == (int)EdgeColor::Red);
            }
            
            SECTION("Yellow & Green equals green") {
                REQUIRE(((int)EdgeColor::Yellow & (int)EdgeColor::Green) == (int)EdgeColor::Green);
            }
            
            SECTION("Yellow & Blue equals black") {
                REQUIRE(((int)EdgeColor::Yellow & (int)EdgeColor::Blue) == (int)EdgeColor::Black);
            }
        }
        
        SECTION("Magenta tests") {
            SECTION("Magenta & Red equals red") {
                REQUIRE(((int)EdgeColor::Magenta & (int)EdgeColor::Red) == (int)EdgeColor::Red);
            }
            
            SECTION("Magenta & Blue equals blue") {
                REQUIRE(((int)EdgeColor::Magenta & (int)EdgeColor::Blue) == (int)EdgeColor::Blue);
            }
            
            SECTION("Magenta & Green equals black") {
                REQUIRE(((int)EdgeColor::Magenta & (int)EdgeColor::Green) == (int)EdgeColor::Black);
            }
        }
        
        SECTION("Cyan tests") {
            SECTION("Cyan & Green equals green") {
                REQUIRE(((int)EdgeColor::Cyan & (int)EdgeColor::Green) == (int)EdgeColor::Green);
            }
            
            SECTION("Cyan & Blue equals blue") {
                REQUIRE(((int)EdgeColor::Cyan & (int)EdgeColor::Blue) == (int)EdgeColor::Blue);
            }
            
            SECTION("Cyan & Red equals black") {
                REQUIRE(((int)EdgeColor::Cyan & (int)EdgeColor::Red) == (int)EdgeColor::Black);
            }
        }
    }
}