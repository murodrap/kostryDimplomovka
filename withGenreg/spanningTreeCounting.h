#pragma once
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
using Matrix = std::vector<std::vector<long double>>;


class SpanningTreeCounter
{
    int n;
    int reg;
    //double **matrix = nullptr;
    Matrix matrix;
    //double **pairs = nullptr;
    std::vector<std::vector<long double>> pairs;

    long double maxValue(int number);

    long long gauss(int n);
    long long kofaktor(int n, int r, int s);
    
public:
    
    using Edges = std::vector<std::vector<int>>;
    long long countForGraph(const Edges& edges);
    SpanningTreeCounter(int n2, int reg2)
    : n(n2)
    , reg(reg2)
    {
        pairs.reserve(n - 1);
        matrix.reserve(n - 1);
        for (int i = 0; i < n - 1; i++) {
            pairs.emplace_back(std::vector<long double>(2));
            matrix.emplace_back(std::vector<long double>(n - 1));
        }
    }
};