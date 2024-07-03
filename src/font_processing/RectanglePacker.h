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
    RectanglePacker(uint32_t width, uint32_t height);

    /// non constructor initialization
    void Init(uint32_t width, uint32_t height);

    /// find a suitable position for the given rectangle
    /// @return true if the rectangle can be added, false otherwise
    bool AddRectangle(uint16_t width, uint16_t height, uint16_t &out_x, uint16_t &out_y);

    /// return the used surface in squared unit
    [[nodiscard]] uint32_t GetUsedSurface() const {
        return used_space_;
    }

    /// return the total available surface in squared unit
    [[nodiscard]] uint32_t GetTotalSurface() const {
        return width_ * height_;
    }

    /// return the usage ratio of the available surface [0:1]
    [[nodiscard]] float GetUsageRatio() const;

    /// reset to initial state
    void Clear();

private:
    int32_t Fit(uint32_t skyline_node_index, uint16_t width, uint16_t height);

    /// Merges all skyline nodes that are at the same level.
    void Merge();

    struct Node {
        Node(int16_t x, int16_t y, int16_t width) : x(x), y(y), width(width) {
        }

        int16_t x;     //< The starting x-coordinate (leftmost).
        int16_t y;     //< The y-coordinate of the skyline level line.
        int32_t width; //< The line _width. The ending coordinate (inclusive) will be x+width-1.
    };


    uint32_t width_;            //< width (in pixels) of the underlying texture
    uint32_t height_;           //< height (in pixels) of the underlying texture
    uint32_t used_space_;        //< Surface used in squared pixel
    std::vector<Node> skyline_; //< node of the skyline algorithm
};
