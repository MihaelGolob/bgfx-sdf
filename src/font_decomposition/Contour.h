//
// Created by MihaelGolob on 3. 07. 2024.
//

#pragma once

#include <vector>
#include "Vector2.h"
#include "EdgeHolder.h"

class Contour {
public:
    std::vector<EdgeHolder> edges;
    void AddEdge(const EdgeHolder& edge);
    
    [[nodiscard]] bool HasEdges() const;
};
