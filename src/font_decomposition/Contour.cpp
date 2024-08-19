//
// Created by MihaelGolob on 3. 07. 2024.
//

#include <string>
#include "Contour.h"

void Contour::AddEdge(const EdgeHolder &edge) {
    edges.push_back(edge);
}

bool Contour::HasEdges() const {
    return !edges.empty();
}

std::string Contour::ToString() const {
    std::string result;
    for (const auto &edge: edges) {
        result.append(edge->ToString());
        result.append("\n");
    }
    return result;
}
