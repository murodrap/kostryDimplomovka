#pragma once
#include "spanningTreeCounting.h"
#include <atomic>
#include <exception>
#include <ostream>
#include <stdexcept>
#include <stdio.h>
#include <utility>
#include <vector>
#include <climits>
#include <string>
#include <set>

#include "cycleReducer.h"
#include "grafyPreVsetkych.h"


bool CycleReducer::processLength(int length) {
    auto cycles = finder.countForGraph(*graph, length);
    if (!findFirstReduction) {
        std::cout << "processing length " << length << ", " << cycles.size() << " cycles\n";
    }
    
    if (!cycles.size()) {
        return false; 
    }
    for (auto cycle : cycles) {
        if (processCycle(cycle, length)) {
            return true;
        }
    }
    return false;

}

bool CycleReducer::processCycle(const std::vector<int>& cycle, int length) {

    for (int remIndex = 0; remIndex < length; remIndex++) {
        for (int attIndex = 0; attIndex < length; attIndex++) {
            if (remIndex == attIndex /*|| !graph->at(remIndex).at(attIndex)*/) {
                continue;
            }
            
            if (performReduction(remIndex, attIndex, cycle, length)) {
                return true;
            }
            
        }
    }
    return false;
}

int CycleReducer::getLeftNeighbour(int vertexIndex, const std::vector<int>& cycle, int length) {
    return cycle[(vertexIndex + length - 1) % length];
}

int CycleReducer::getRightNeighbour(int vertexIndex, const std::vector<int>& cycle, int length) {
    return cycle[(vertexIndex + 1) % length];
}

int CycleReducer::getNeighbourOutsideCycle(int vertex, int leftNeighbour, int rightNeighbour) {
    for (int possibleNeighbour = 0; possibleNeighbour < n; ++possibleNeighbour) {
        if (graph->at(vertex).at(possibleNeighbour)) {
            if (possibleNeighbour == leftNeighbour || possibleNeighbour == rightNeighbour) {
                continue;
            }
            return possibleNeighbour;
        }
    }
    throw std::invalid_argument("newer go here");
}

bool CycleReducer::tryReduction(int removedVertexIndex, int attachToVertexIndex, const std::vector<int>& cycle, int length) {
    if (length == 4) {
        if (graph->at(cycle[0]).at(cycle[2]) || graph->at(cycle[1]).at(cycle[3])) {
            return false;
        }
    }

    int leftToRemoved = getLeftNeighbour(removedVertexIndex, cycle, length);
    int rightToRemoved = getRightNeighbour(removedVertexIndex, cycle, length);

    if (graph->at(leftToRemoved).at(rightToRemoved)) {
        return false;
    }



    int leftToAttached = getLeftNeighbour(attachToVertexIndex, cycle, length);
    int rightToAttached = getRightNeighbour(attachToVertexIndex, cycle, length);
    int removedVertex = cycle[removedVertexIndex];
    int attachTioVertex = cycle[attachToVertexIndex];


    int removedOtherNeighbour = getNeighbourOutsideCycle(removedVertex, leftToRemoved, rightToRemoved);
    int attachedOtherNeighbour = getNeighbourOutsideCycle(attachTioVertex, leftToAttached, rightToAttached);
    if (removedOtherNeighbour == attachedOtherNeighbour) {
        return false;
    }
    

    if (attachedOtherNeighbour == removedVertex) {
        return false;
    }

    if (leftToRemoved == attachedOtherNeighbour || rightToRemoved == attachedOtherNeighbour) {
        return false;
    }
    
    
    removedEdges.push_back(std::make_pair(attachTioVertex, attachedOtherNeighbour));
    addedEdges.push_back(std::make_pair(removedVertex, attachedOtherNeighbour));
    
    addedEdges.push_back(std::make_pair(leftToRemoved, rightToRemoved));

    if (attachTioVertex != leftToRemoved) {
        removedEdges.push_back(std::make_pair(leftToRemoved, removedVertex));
    }
    if (attachTioVertex != rightToRemoved) {
        removedEdges.push_back(std::make_pair(rightToRemoved, removedVertex));
    }
    if (attachTioVertex != leftToRemoved && attachTioVertex != rightToRemoved) {
        addedEdges.push_back(std::make_pair(removedVertex, attachTioVertex));
    }
    return true;
}

void CycleReducer::processEdges(const std::vector<std::pair<int, int>>& edges, int operation) {
    for (auto edge : edges) {
        int u = edge.first;
        int v = edge.second;
        graph->at(u).at(v) = operation;
        graph->at(v).at(u) = operation;
    }
}

void CycleReducer::updateMatrix() {
    processEdges(removedEdges, 0);
    processEdges(addedEdges, 1);
    
}

void CycleReducer::deupdateMatrix() {
    
    processEdges(removedEdges, 1);
    processEdges(addedEdges, 0);

}

bool CycleReducer::performReduction(int removedVertexIndex, int attachToVertexIndex, const std::vector<int>& cycle, int length) {
    if (!tryReduction(removedVertexIndex, attachToVertexIndex, cycle, length)) {
        return false;
    }
    updateMatrix();
    long long newSpanningTrees = counter.countForGraph(*graph);
    
    if (newSpanningTrees < originalSpanningTreeCount) {
        numberOfDecreasingReductions++;
        if (!findFirstReduction) {
            printReduction(cycle, cycle[removedVertexIndex], cycle[attachToVertexIndex], newSpanningTrees);
        }

        if (!findFirstReduction) {
            std::cout << "new matrix\n";
            std::cout << std::endl;
        
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    std::cout << graph->at(i).at(j) << " ";
                }
                std::cout << std::endl;
            }
        }
        
        
        removedEdges.clear();
        addedEdges.clear();
        if (findFirstReduction) {
            deupdateMatrix();
            return true;
        }
        processGraph(newSpanningTrees);
        return true;
    }
    deupdateMatrix();
    removedEdges.clear();
    addedEdges.clear();
    return false;
}

bool CycleReducer::processGraph(AdjMatrix* graph2, long long spanningTrees) {
    numberOfDecreasingReductions = 0;
    graph = graph2;

    bool reducible = processGraph(spanningTrees);
    if (!findFirstReduction) {
        std::cout << std::endl << std::endl << "POSSIBLE REDUCTIONS: " << numberOfDecreasingReductions << std::endl;
    }
    

    return reducible;
}

bool CycleReducer::processGraph(long long spanningTrees) {
    if (!findFirstReduction) {
        std::cout << "\n========================================================\n========================================================\n";
    }
    

    originalSpanningTreeCount = spanningTrees;
    for (int length = 4; length <= n; length++) {
        if (processLength(length)) {
            return true;
        }
    }
    return false;
}
void CycleReducer::printEdges(const std::vector<std::pair<int, int>>& edges) {
    for (auto edge : edges) {
        int u = edge.first;
        int v = edge.second;
        std::cout << "(" << u << ", " << v << "), ";
    }
}

void CycleReducer::printReduction(const std::vector<int>& cycle, int removedVertex,  int attachToVertex,long long newSpanningTrees) {
    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "reduction of cycle ";
    for (int vertex : cycle) {
        std::cout << vertex << " ";
    }
    std::cout << std::endl << "removed vertex: " << removedVertex << ", attached to vertex: " << attachToVertex << std::endl;
    std::cout << "removed edges: ";
    printEdges(removedEdges);
    std::cout << "added edges: ";
    printEdges(addedEdges);
    std::cout << std::endl;

    std::cout << "spanning trees ";
    if (originalSpanningTreeCount > newSpanningTrees) {
        std::cout << "decreased ";
    }
    else if (originalSpanningTreeCount < newSpanningTrees) {
        std::cout << "increased ";
    }
    else {
        std::cout << "not changed ";
    }
    std::cout << "from " << originalSpanningTreeCount << " to " << newSpanningTrees << std::endl;
}