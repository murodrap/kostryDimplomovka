#include <cstring>
#include <iterator>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <type_traits>
#include <vector>
#include <regex>
#include <bits/stdc++.h>

#include "spanningTreeCounting.h"
#include "cycleFinder.h"
#include "cycleReducer.h"
using AdjMatrix = std::vector<std::vector<int>>;


void DFS(int V, AdjMatrix graph, std::vector<bool> visited, int n, int vert, int start, int& count)
{
    visited[vert] = true;
 
    if (n == 0) {
        visited[vert] = false;
 
        if (graph[vert][start] && graph[start][vert]) {
            count++;
            return;
        }
        else
            return;
    }
 
    for (int i = 0; i < V; i++)
        if (!visited[i] && graph[vert][i]) {
            DFS(V, graph, visited, n - 1, i, start, count);
        }
            
    visited[vert] = false;
}
 
int countCycles(int V, AdjMatrix graph, int n)
{
    std::vector<bool> visited(V);
    int count = 0;
    for (int i = 0; i < V - (n - 1); i++) {
        DFS(V, graph, visited, n - 1, i, i, count);
        visited[i] = true;
    }
 
    return count / 2;
}

AdjMatrix stringToAdjList(std::string& listOfEdges, int n) {
    AdjMatrix adjList(n);
    for (int v = 0; v < n; v++) {
        adjList[v] = std::vector<int>(n);
    }

    std::regex edgeRegex("[0-9]+, [0-9]+");
    std::regex verticesRegex("([0-9]+), ([0-9]+)");
    std::smatch edgesFromRegex;
    std::smatch verticesFromRegex;

    while (std::regex_search(listOfEdges, edgesFromRegex, edgeRegex)) {
        std::string vr = edgesFromRegex[0];
        std::regex_search(vr, verticesFromRegex, verticesRegex);
        int v = std::stoi(verticesFromRegex[1]);
        int u = std::stoi(verticesFromRegex[2]);
        adjList[v][u] = 1;
        adjList[u][v] = 1;
        listOfEdges = edgesFromRegex.suffix();
    }

    return adjList;

}

int main(int argc, char *argv[]) {
    std::string graf = argv[1];
    int n = atoi(argv[2]);
    std::cout << n << " : " << graf << std::endl;
    AdjMatrix grafMatica = stringToAdjList(graf, n);
    //std::cout << "created matrix\n";
    SpanningTreeCounter counter = SpanningTreeCounter(n, 3);
    long long trees = counter.countForGraph(grafMatica);
    std::cout << trees << " spanning trees\n";
   

    CycleReducer reducer = CycleReducer(n, 3, false);
    reducer.processGraph(&grafMatica, trees);

    /*

    CycleFinder finder = CycleFinder(n);
    auto cykly = finder.countForGraph(grafMatica, 6);
    for (auto cycle : cykly) {
        for (auto v : cycle) {
                std::cout << v << " ";
            }
            std::cout << std::endl;
    }
    
    */
    int cyklovSpolu = 0;
    for (int i = 3; i <= n; i++) {
        int pocetCyklov = countCycles(n, grafMatica, i);
        cyklovSpolu += pocetCyklov;
        std::cout << "pocet cyklov dlzky " << i << ": " << pocetCyklov << std::endl;
    }
    std::cout << "dokopy " << cyklovSpolu << std::endl;
    
    

}