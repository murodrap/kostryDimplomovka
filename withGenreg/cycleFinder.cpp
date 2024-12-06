#include <set>
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <iomanip>
#include <algorithm>
#include <set>

#include "cycleFinder.h"

using AdjMatrix = std::vector<std::vector<int>>;


std::vector<std::vector<int>> CycleFinder::countForGraph(const AdjMatrix& matrix, int length) {
    cycleLength = length;
    currentCycle = std::vector<int>(cycleLength);
    cycleList.clear();
    cyclesEdges.clear();
    std::fill(visited.begin(), visited.end(), false);
    graph = matrix;

    for (int i = 0; i < n - (cycleLength - 1); i++) {
        currentCycle[0] = i;
        search(cycleLength - 1, i, i, 1);
        visited[i] = true;
    }
 
    return cycleList;

}

void CycleFinder::search(int length, int vert, int start, int cycleIndex) {
    visited[vert] = true;
 
    if (length == 0) {
        visited[vert] = false;
        cycleIndex--;
 
        if (graph[vert][start] && graph[start][vert]) {
            std::set<std::pair<int, int>> edgesOfCycle = std::set<std::pair<int, int>>();
            for (int i = 0; i < cycleLength; i++) {
                int u = currentCycle[i];
                int v = currentCycle[(i+1)%cycleLength];
                if (u > v) {
                    int o = v;
                    v = u;
                    u = o;
                }
                edgesOfCycle.insert({u, v});
            }
            if (cyclesEdges.find(edgesOfCycle) == cyclesEdges.end()) {
                cyclesEdges.insert(edgesOfCycle);
                cycleList.emplace_back(currentCycle);
            }
            
            
            return;
        }
        else
            return;
    }
 
    for (int i = 0; i < n; i++)
        if (!visited[i] && graph[vert][i]) {
            currentCycle[cycleIndex] = i;
            search(length - 1, i, start, cycleIndex + 1);
        }
            
    visited[vert] = false;
}