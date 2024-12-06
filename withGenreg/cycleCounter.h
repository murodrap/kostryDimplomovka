#pragma once
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>



class CycleCounter
{
    using AdjMatrix = std::vector<std::vector<int>>;
    int n;
    int cycleLength;
    int count = 0;
    std::vector<bool> visited;
    AdjMatrix graph;
    
    void search(int length, int vert, int start);
    
public:
    
    long countForGraph(const AdjMatrix& graph);
    CycleCounter(int n2, int length)
    : n(n2)
    , cycleLength(length)
    {
        visited.resize(n);
    }
};