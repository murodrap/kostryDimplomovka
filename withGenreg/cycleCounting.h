#pragma once
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <map>
#include <set>
using Matrix = std::map<int, std::set<int>>;
using Edges = std::vector<std::vector<int>>;


class CycleCounter
{
    int n;
    int reg;
    Matrix matrix;
    std::vector<int> visitedVertices;
    int foundCycles = 0;
    std::vector<int> path;

    void edgesToMatrix(const Edges& edges);
    void stringToMatrix(std::string& graph);
    void reset() {
        for (int i = 0; i < n; i++) {
            matrix[i] = {};
            visitedVertices[i] = 0;
        }
        foundCycles = 0;
    }

    int countCycles(int length);
    void findCycleFromVertex(int remainingLength, int currentVertex, int originalVertex);


    
public:
    
    
    int countForGraph(const Edges& edges, int length);
    int countForGraph(std::string& graph, int length);
    CycleCounter(int n2, int reg2)
    : n(n2)
    , reg(reg2)
    , visitedVertices(n)
    {
        reset();
        
    }
};