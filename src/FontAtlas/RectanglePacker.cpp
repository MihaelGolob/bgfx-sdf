//
// Created by MihaelGolob on 30. 04. 2024.
//

#include "RectanglePacker.h"

RectanglePacker::RectanglePacker() : m_width(0), m_height(0), m_usedSpace(0) { }

RectanglePacker::RectanglePacker(uint32_t _width, uint32_t _height) : m_width(_width), m_height(_height), m_usedSpace(0) {
    // We want a one pixel border around the whole atlas to avoid any artefact when sampling texture
    m_skyline.emplace_back(1, 1, uint16_t(_width - 2));
}

void RectanglePacker::init(uint32_t _width, uint32_t _height) {
    BX_ASSERT(_width > 2, "_width must be > 2");
    BX_ASSERT(_height > 2, "_height must be > 2");

    m_width = _width;
    m_height = _height;
    m_usedSpace = 0;

    m_skyline.clear();
    // We want a one pixel border around the whole atlas to avoid any artifact when
    // sampling texture
    m_skyline.emplace_back(1, 1, uint16_t(_width - 2));
}

bool RectanglePacker::addRectangle(uint16_t _width, uint16_t _height, uint16_t &_outX, uint16_t &_outY) {
    int best_height, best_index;
    int32_t best_width;
    Node *node;
    Node *prev;
    _outX = 0;
    _outY = 0;

    best_height = INT_MAX;
    best_index = -1;
    best_width = INT_MAX;
    for (uint16_t ii = 0, num = uint16_t(m_skyline.size()); ii < num; ++ii) {
        int32_t yy = fit(ii, _width, _height);
        if (yy >= 0) {
            node = &m_skyline[ii];
            if (((yy + _height) < best_height)
                || (((yy + _height) == best_height) && (node->width < best_width))) {
                best_height = uint16_t(yy) + _height;
                best_index = ii;
                best_width = node->width;
                _outX = node->x;
                _outY = uint16_t(yy);
            }
        }
    }

    if (best_index == -1) {
        return false;
    }

    Node newNode(_outX, _outY + _height, _width);
    m_skyline.insert(m_skyline.begin() + best_index, newNode);

    for (auto ii = uint16_t(best_index + 1), num = uint16_t(m_skyline.size()); ii < num; ++ii) {
        node = &m_skyline[ii];
        prev = &m_skyline[ii - 1];
        if (node->x < (prev->x + prev->width)) {
            auto shrink = uint16_t(prev->x + prev->width - node->x);
            node->x += shrink;
            node->width -= shrink;
            if (node->width <= 0) {
                m_skyline.erase(m_skyline.begin() + ii);
                --ii;
                --num;
            } else {
                break;
            }
        } else {
            break;
        }
    }

    merge();
    m_usedSpace += _width * _height;
    return true;
}

float RectanglePacker::getUsageRatio() {
    uint32_t total = m_width * m_height;
    if (total > 0) {
        return (float) m_usedSpace / (float) total;
    }

    return 0.0f;
}

void RectanglePacker::clear() {
    m_skyline.clear();
    m_usedSpace = 0;

    // We want a one pixel border around the whole atlas to avoid any artefact when
    // sampling texture
    m_skyline.emplace_back(1, 1, uint16_t(m_width - 2));
}

int32_t RectanglePacker::fit(uint32_t _skylineNodeIndex, uint16_t _width, uint16_t _height) {
    int32_t width = _width;
    int32_t height = _height;

    const Node &baseNode = m_skyline[_skylineNodeIndex];

    int32_t xx = baseNode.x, yy;
    int32_t widthLeft = width;
    int32_t ii = _skylineNodeIndex;

    if ((xx + width) > (int32_t) (m_width - 1)) {
        return -1;
    }

    yy = baseNode.y;
    while (widthLeft > 0) {
        const Node &node = m_skyline[ii];
        if (node.y > yy) {
            yy = node.y;
        }

        if ((yy + height) > (int32_t) (m_height - 1)) {
            return -1;
        }

        widthLeft -= node.width;
        ++ii;
    }

    return yy;
}

void RectanglePacker::merge() {
    Node *node;
    Node *next;
    uint32_t ii;

    for (ii = 0; ii < m_skyline.size() - 1; ++ii) {
        node = (Node *) &m_skyline[ii];
        next = (Node *) &m_skyline[ii + 1];
        if (node->y == next->y) {
            node->width += next->width;
            m_skyline.erase(m_skyline.begin() + ii + 1);
            --ii;
        }
    }
}