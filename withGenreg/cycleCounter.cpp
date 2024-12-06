#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <iomanip>

#include "cycleCounter.h"

using AdjMatrix = std::vector<std::vector<int>>;


long CycleCounter::countForGraph(const AdjMatrix& matrix) {
    std::fill(visited.begin(), visited.end(), false);
    graph = matrix;
    count = 0;
    for (int i = 0; i < n - (cycleLength - 1); i++) {
        search(cycleLength - 1, i, i);
        visited[i] = true;
    }
 
    return count / 2;

}

void CycleCounter::search(int length, int vert, int start) {
    visited[vert] = true;
 
    if (length == 0) {
        visited[vert] = false;
 
        if (graph[vert][start] && graph[start][vert]) {
            count++;
            return;
        }
        else
            return;
    }
 
    for (int i = 0; i < n; i++)
        if (!visited[i] && graph[vert][i]) {
            search(length - 1, i, start);
        }
            
    visited[vert] = false;
}