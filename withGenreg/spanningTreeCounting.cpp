#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <iomanip>

#include "spanningTreeCounting.h"

using Matrix = std::vector<std::vector<long double>>;
using Edges = std::vector<std::vector<int>>;

/**
 * finds pair with highes values overall
 *
 * @param number index of the element to consider
 * @return second element from the pair with max values
 */
long double SpanningTreeCounter::maxValue(int number) {

    long double currentMax[2] {0., 0.};
    for (int i = 0; i < number; i++) {
        
        long double p = pairs[i][0];
        long double d = pairs[i][1];
        
        if (p > currentMax[0]) {
            currentMax[0] = p;
            currentMax[1] = d;
        }
        else if (p == currentMax[0] && d > currentMax[1]) {
            currentMax[0] = p;
            currentMax[1] = d;
        }
   }

    return currentMax[1];
}

/**
 * cumputes determinant of matrix of size n x n using Gauss elimination
 *
 * @param n dimensions of matrix
 * @return determinant of matrix
 */
long long SpanningTreeCounter::gauss(int n) {
    int r1 = 0;
    int s1 = 0;

    while (r1 < n && s1 < n) {
            long double iMax;

            for (int i = r1; i < n; i++) {
                pairs[i-r1][0] = (long double)std::abs(matrix[i][s1]);
                pairs[i-r1][1] = (long double)i;
            }
            iMax = maxValue(n - r1);
            
            if (matrix[(int)iMax][s1] == 0) {
                s1++;
            }

            else {
                std::swap(matrix[r1], matrix[(int)iMax]);
                for (int i = r1+1; i < n; i++) {
                    long double f = matrix[i][s1] / matrix[r1][s1];
                    matrix[i][s1] = 0.;
                    for (int j = s1+1; j < n; j++) {
                        matrix[i][j] -= matrix[r1][j] * f;
                    }
                }

            }
            r1++;
            s1++;
        }
            
        long double result = 1.;
        for (int i = 0; i < n; i++) {
            result *= matrix[i][i];
        }
    //std::cout << std::setprecision (15) << result << std::endl;
    //std::cout << result << "----" << llabs(result) << std::endl;
   
    return llround(result);


}

/**
 * cumputes cofactor of matrix with specified row and column for removal
 *
 * @param r, s index of removed row and column
 * @param n dimensions of matrix
 * @return cofactor of matrix
 */
long long SpanningTreeCounter::kofaktor(int n, int r, int s) {
    return SpanningTreeCounter::gauss(n-1);
}

/**
 * counts number of spanning trees in a graph
 *
 * @param edges edge list of a graph whose spanning trees are counted
 * @return number of spanning trees of the given graph
 */
long long SpanningTreeCounter::countForGraph(const Edges& edges) {
    for (int i= 0; i < n-1; i++) {
        std::fill(matrix[i].begin(), matrix[i].end(), 0.0);
    }
   

    for (const std::vector<int>& edge : edges) {
        int u = edge[0]-1;
        int v = edge[1]-1;
        if (v >= 0 && u >= 0) {
            matrix[u][v] = -1.;
            matrix[v][u] = -1.;
        }
        if (!reg) { 
            if (u >= 0) matrix[u][u]++;
            if (v >= 0) matrix[v][v]++;
        }

    }

    if (reg) {
        for (int r = 0; r < n-1; r++) {
            matrix[r][r] = (long long)reg;
        }
    }


    return SpanningTreeCounter::kofaktor(n, 0, 0);
}