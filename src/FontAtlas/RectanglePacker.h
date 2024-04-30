//
// Created by MihaelGolob on 30. 04. 2024.
//
#pragma once

#include <bgfx/bgfx.h>
#include <vector>

#include "../utilities.h"

class RectanglePacker {
public:
    RectanglePacker();
    RectanglePacker(uint32_t _width, uint32_t _height);

    /// non constructor initialization
    void init(uint32_t _width, uint32_t _height);

    /// find a suitable position for the given rectangle
    /// @return true if the rectangle can be added, false otherwise
    bool addRectangle(uint16_t _width, uint16_t _height, uint16_t &_outX, uint16_t &_outY);

    /// return the used surface in squared unit
    uint32_t getUsedSurface() {
        return m_usedSpace;
    }

    /// return the total available surface in squared unit
    uint32_t getTotalSurface() {
        return m_width * m_height;
    }

    /// return the usage ratio of the available surface [0:1]
    float getUsageRatio();

    /// reset to initial state
    void clear();

private:
    int32_t fit(uint32_t _skylineNodeIndex, uint16_t _width, uint16_t _height);

    /// Merges all skyline nodes that are at the same level.
    void merge();

    struct Node {
        Node(int16_t _x, int16_t _y, int16_t _width) : x(_x), y(_y), width(_width) {
        }

        int16_t x;     //< The starting x-coordinate (leftmost).
        int16_t y;     //< The y-coordinate of the skyline level line.
        int32_t width; //< The line _width. The ending coordinate (inclusive) will be x+width-1.
    };


    uint32_t m_width;            //< width (in pixels) of the underlying texture
    uint32_t m_height;           //< height (in pixels) of the underlying texture
    uint32_t m_usedSpace;        //< Surface used in squared pixel
    std::vector<Node> m_skyline; //< node of the skyline algorithm
};
