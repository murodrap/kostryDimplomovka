#include <ostream>
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <iomanip>
#include <regex>

#include "cycleCounting.h"
//#include "spanningTreeCounting.h"



void CycleCounter::edgesToMatrix(const Edges& edges) {
    for (auto edge : edges) {
        int u = edge[0];
        int v = edge[1];
        matrix[u].insert(v);
        matrix[v].insert(u);
    }

}

void CycleCounter::stringToMatrix(std::string& graph) {
    std::regex edgeRegex("[0-9]+, [0-9]+");
    std::regex verticesRegex("([0-9]+), ([0-9]+)");
    std::smatch edgesFromRegex;
    std::smatch verticesFromRegex;

    while (std::regex_search(graph, edgesFromRegex, edgeRegex)) {
   
        std::string vr = edgesFromRegex[0];
        std::regex_search(vr, verticesFromRegex, verticesRegex);
        std::string v = verticesFromRegex[1];
        std::string u = verticesFromRegex[2];
        int v1 = std::stoi(v);
        int u1 = std::stoi(u);
        matrix[v1].insert(u1);
        matrix[u1].insert(v1);
        graph = edgesFromRegex.suffix();
    }

}

void CycleCounter::findCycleFromVertex(int remainingLength, int currentVertex, int originalVertex) {
    
    path[path.size() - remainingLength] = currentVertex;
    if (remainingLength == 1) {
        //std::cout << "end " << currentVertex << std::endl << "path: ";
        for (auto vertex : path) {
            //std::cout << vertex << " ";
        }
        //std::cout << std::endl;
        if (matrix[currentVertex].find(originalVertex) != matrix[currentVertex].end()) {
            //std::cout << "cycle\n";
            foundCycles++;
        }
        return;
    }
    

    for (int neighbour : matrix[currentVertex]) {
        if (visitedVertices[neighbour]) {
            continue;
        }
        visitedVertices[neighbour] = 1;
        findCycleFromVertex(remainingLength -1, neighbour, originalVertex);
        visitedVertices[neighbour] = 0;
    }
}

int CycleCounter::countCycles(int length) {
    for (int i = 0; i < length; i++) {
        path.emplace_back(0);
    }

    for (int vertex = 0; vertex < n - length + 1; vertex++) {
        visitedVertices[vertex] = 1;
        findCycleFromVertex(length, vertex, vertex);
        visitedVertices[vertex] = 0;
    }
    return foundCycles/2;

}

int CycleCounter::countForGraph(const Edges& edges, int length) {
    reset();
    edgesToMatrix(edges);

    for (int v = 0; v < n; v++) {
        //std::cout << v << ": ";
        for (auto u : matrix[v]) {
            //std::cout << u << " ";
        }
        //std::cout << std::endl;
    }
    return countCycles(length);
}

int CycleCounter::countForGraph(std::string& graph, int length) {
    reset();
    stringToMatrix(graph);
    
    return countCycles(length);
}

int main(int argc, char *argv[]) {
    while (true) {
        int n, length;
        std::cin >> n;
        std::cin >> length;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::string graph;
        std::getline(std::cin, graph);

        CycleCounter counter = CycleCounter(n, 3);
        //SpanningTreeCounter counter2 = SpanningTreeCounter(n, 3);
        //int cycles = counter.countForGraph({{0, 1}, {0, 2}, {0, 3}, {1, 2}, {1, 3}, {2, 3}}, 4);
        int cycles = counter.countForGraph(graph, length);
        //int trees = counter2.countForGraph(const Edges &edges)
        std::cout << cycles << std::endl;
    }
    
}