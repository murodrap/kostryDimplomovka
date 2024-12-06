#pragma once
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <set>



class CycleFinder
{
    using AdjMatrix = std::vector<std::vector<int>>;
    int n;
    int cycleLength;
    std::set<std::set<std::pair<int, int>>> cyclesEdges;
    std::vector<std::vector<int>> cycleList;
    std::vector<int> currentCycle;
    std::vector<bool> visited;
    AdjMatrix graph;
    
    void search(int length, int vert, int start, int cycleIndex);
    
public:
    
    std::vector<std::vector<int>> countForGraph(const AdjMatrix& graph, int length);
    CycleFinder(int n2)
    : n(n2)
    {
        visited.resize(n);
    }
};