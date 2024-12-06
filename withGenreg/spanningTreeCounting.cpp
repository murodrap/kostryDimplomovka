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
#include <armadillo>


using Matrix = std::vector<std::vector<long double>>;
using AdjMatrix = std::vector<std::vector<int>>;

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
                //std::cout << std::abs(matrix[i][s1]) << "-----" << (long double)std::abs(matrix[i][s1]) << std::endl;

                pairs[i-r1][0] = std::abs(matrix[i][s1]);
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
            result = result * matrix[i][i];
            //std::cout << std::setprecision (15) << result  << "       " << matrix[i][i]<< std::endl;
        }
    
    //std::cout<< std::setprecision (25)  << result << std::setprecision (15) << "----" << llabs(result) << std::endl;
   
    return llabs(llround(result));


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
long long SpanningTreeCounter::countForGraph(const AdjMatrix& adjMatrix) {
     //arma::dmat matrixArma(n-1, n-1, arma::fill::zeros);

    for (int i= 0; i < n-1; i++) {
        std::fill(matrix[i].begin(), matrix[i].end(), 0.0);
    }

    for (int i = 1; i < n; i++) {
        for (int j = 1; j < n; j++) {
            matrix[i-1][j-1] = adjMatrix[i][j] * -1.;
            //matrixArma(i-1, j-1) = adjMatrix[i][j] * -1;
            if (reg && i == j) {
               matrix[i-1][j-1] = (long double)reg;
               //matrixArma(i-1, j-1) = reg;
            }
        }
    }
    //long long determinant = arma::det(matrixArma);
    //std::cout << "===========" << determinant << "=================\n";


    //return determinant;
    return SpanningTreeCounter::kofaktor(n, 0, 0);
}