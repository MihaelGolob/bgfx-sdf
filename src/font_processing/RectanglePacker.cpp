//
// Created by MihaelGolob on 30. 04. 2024.
//

#include "RectanglePacker.h"

RectanglePacker::RectanglePacker() : width_(0), height_(0), used_space_(0) { }

RectanglePacker::RectanglePacker(uint32_t width, uint32_t height) : width_(width), height_(height), used_space_(0) {
    // We want a one pixel border around the whole atlas to avoid any artefact when sampling texture
    skyline_.emplace_back(1, 1, uint16_t(width - 2));
}

void RectanglePacker::Init(uint32_t width, uint32_t height) {
    BX_ASSERT(width > 2, "_width must be > 2")
    BX_ASSERT(height > 2, "_height must be > 2")

    width_ = width;
    height_ = height;
    used_space_ = 0;

    skyline_.clear();
    // We want a one pixel border around the whole atlas to avoid any artifact when
    // sampling texture
    skyline_.emplace_back(1, 1, uint16_t(width - 2));
}

bool RectanglePacker::AddRectangle(uint16_t width, uint16_t height, uint16_t &out_x, uint16_t &out_y) {
    int best_height, best_index;
    int32_t best_width;
    Node *node;
    Node *prev;
    out_x = 0;
    out_y = 0;

    best_height = INT_MAX;
    best_index = -1;
    best_width = INT_MAX;
    for (uint16_t ii = 0, num = uint16_t(skyline_.size()); ii < num; ++ii) {
        int32_t yy = Fit(ii, width, height);
        if (yy >= 0) {
            node = &skyline_[ii];
            if (((yy + height) < best_height)
                || (((yy + height) == best_height) && (node->width < best_width))) {
                best_height = uint16_t(yy) + height;
                best_index = ii;
                best_width = node->width;
                out_x = node->x;
                out_y = uint16_t(yy);
            }
        }
    }

    if (best_index == -1) {
        return false;
    }

    Node new_node(out_x, out_y + height, width);
    skyline_.insert(skyline_.begin() + best_index, new_node);

    for (auto ii = uint16_t(best_index + 1), num = uint16_t(skyline_.size()); ii < num; ++ii) {
        node = &skyline_[ii];
        prev = &skyline_[ii - 1];
        if (node->x < (prev->x + prev->width)) {
            auto shrink = uint16_t(prev->x + prev->width - node->x);
            node->x += shrink;
            node->width -= shrink;
            if (node->width <= 0) {
                skyline_.erase(skyline_.begin() + ii);
                --ii;
                --num;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    Merge();
    used_space_ += width * height;
    return true;
}

float RectanglePacker::GetUsageRatio() const {
    uint32_t total = width_ * height_;
    if (total > 0) {
        return (float) used_space_ / (float) total;
    }

    return 0.0f;
}

void RectanglePacker::Clear() {
    skyline_.clear();
    used_space_ = 0;

    // We want a one pixel border around the whole atlas to avoid any artefact when
    // sampling texture
    skyline_.emplace_back(1, 1, uint16_t(width_ - 2));
}

int32_t RectanglePacker::Fit(uint32_t skyline_node_index, uint16_t w, uint16_t h) {
    int32_t width = w;
    int32_t height = h;

    const Node &base_node = skyline_[skyline_node_index];

    int32_t xx = base_node.x, yy;
    int32_t width_left = width;
    int32_t ii = skyline_node_index;

    if ((xx + width) > (int32_t) (width_ - 1)) {
        return -1;
    }

    yy = base_node.y;
    while (width_left > 0) {
        const Node &node = skyline_[ii];
        if (node.y > yy) {
            yy = node.y;
        }

        if ((yy + height) > (int32_t) (height_ - 1)) {
            return -1;
        }

        width_left -= node.width;
        ++ii;
    }

    return yy;
}

void RectanglePacker::Merge() {
    Node *node;
    Node *next;
    uint32_t ii;

    for (ii = 0; ii < skyline_.size() - 1; ++ii) {
        node = (Node *) &skyline_[ii];
        next = (Node *) &skyline_[ii + 1];
        if (node->y == next->y) {
            node->width += next->width;
            skyline_.erase(skyline_.begin() + ii + 1);
            --ii;
        }
    }
}