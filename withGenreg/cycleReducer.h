#pragma once
#include "cycleCounter.h"
#include "spanningTreeCounting.h"
#include <stdio.h>
#include <utility>
#include <vector>
#include <climits>
#include <string>
#include <set>

#include "cycleFinder.h"


class CycleReducer
{
    using AdjMatrix = std::vector<std::vector<int>>;
    using Edge = std::pair<int, int>;
    int n;
    int reg;
    SpanningTreeCounter counter;
    CycleFinder finder;
    bool findFirstReduction;
    int numberOfDecreasingReductions;

    long long originalSpanningTreeCount;
    std::vector<std::pair<int, int>> removedEdges;
    std::vector<std::pair<int, int>> addedEdges;

 
    AdjMatrix* graph;
    
    bool processLength(int length);
    bool processCycle(const std::vector<int>& cycle, int length);
    bool performReduction(int removedVertexIndex, int attachToVertexIndex, const std::vector<int>& cycle, int length);
    bool tryReduction(int removedVertexIndex, int attachToVertexIndex, const std::vector<int>& cycle, int length);
    void printReduction(const std::vector<int>& cycle, int removedVertex, int attachToVertex,  long long newSpanningTrees);
    int getLeftNeighbour(int indexInCycle, const std::vector<int>& cycle, int length);
    int getRightNeighbour(int indexInCycle, const std::vector<int>& cycle, int length);
    int getNeighbourOutsideCycle(int vertex, int leftNeighbour, int rightNeighbour);
    void updateMatrix();
    void deupdateMatrix();
    void processEdges(const std::vector<std::pair<int, int>>& edges, int operation);
    void printEdges(const std::vector<std::pair<int, int>>& edges);
    bool processGraph(long long spanningTrees);
    
public:
    
    bool processGraph(AdjMatrix* graph2, long long spanningTrees);
    CycleReducer(int n2, int r, bool firstReduction = false)
    : n(n2)
    , reg(r)
    , counter(n, reg)
    , finder(n)
    , findFirstReduction(firstReduction)
    {

    }
};