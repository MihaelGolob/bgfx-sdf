//
// Created by MihaelGolob on 3. 07. 2024.
//

#include "Contour.h"

void Contour::AddEdge(const EdgeHolder &edge) {
    edges.push_back(edge);
}

bool Contour::HasEdges() const {
    return !edges.empty();
}
