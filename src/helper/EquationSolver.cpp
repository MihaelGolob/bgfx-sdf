//
// Created by MihaelGolob on 8. 07. 2024.
//

#include <vector>
#include "EquationSolver.h"
#include <poly34.h>
#include <algorithm>

std::vector<double> SolveCubicEquation(double a, double b, double c, double d) {
    double r[3];
    int n = SolveP3(r, b/a, c/a, d/a);
    
    auto roots = std::vector<double>(n);
    for (int i = 0; i < n; i++) {
        roots[i] = r[i];
    }
    std::sort(roots.begin(), roots.end());

    return roots;
}

std::vector<double> SolveQuinticEquation(double a, double b, double c, double d, double e, double f) {
    std::vector<double> roots;

    return roots;
}

