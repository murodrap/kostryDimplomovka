#include <algorithm>
#include <cinttypes>
#include <cstddef>
#include <ostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <climits>
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>

#include "spanningTreeCounting.h"
#include "processingRegularGraphs.h"

using Edges = std::vector<std::vector<int>>;

/**
 * checks whether the given graph doesn't have equal or lower/higher nuber of spanning trees that the currently stored min/max values
 * updates the according values if necessary
 *
 * @param number lines with adjacency lists for s graph in string format
 * @param edges lines with adjacency lists for s graph in string format
 */
void ProcRegular::updateValues(long double number, const Edges& edges) {

    if (number == maxK) {
        if (maxG.size() < maxStroredGraphs) {
            maxG.push_back(edges);
        }
    }

    if (number > maxK) {
        maxK = number;
        maxG.clear();
        maxG.push_back(edges);
    }
    if (number == minK) {
        if (minG.size() < maxStroredGraphs) {
            minG.push_back(edges);
        }
    }

    if (number < minK) {
        minK = number;
        minG.clear();
        minG.push_back(edges);
    }

}

long ProcRegular::podlaVzorca() {
    std::map<int, int> hodn{{4, 16}, {6, 75}, {8, 256}, {12, 2112}};


    auto h = hodn.find(this->n);
    if (h != hodn.end()) {
        return h->second;
    }
   
    if (n%4 == 0) {
        long stvorkove = trunc(pow(8, trunc((n-3*5)/4)));
        return trunc(pow(24, 3)) * stvorkove;
    }
    long stvorkove = trunc(pow(8, trunc((n-2*5)/4)));
    return trunc(pow(24, 2)) * stvorkove;
}

/**
 * parses string with adjacency lists to obtain edge list
 *
 * @param lines lines with adjacency lists for s graph in string format
 * @return edge list, represented as two dimensional vector, of the given graph
 */
Edges ProcRegular::getEdges(const Lines& lines) {
    Edges edges(n*reg/2);
    int index = 0;
    for (const std::string& line : lines) {
        std::stringstream ss(line);
        std::string vertex;
        std::string neighbours;
        
        getline(ss, vertex, ':');
        vertex.pop_back();
        getline(ss, neighbours, ':');
        neighbours.erase(0, 1);
        
        int v = stoi(vertex);
        std::string neighbour;  
        std::stringstream ss2(neighbours);
        while (!ss2.eof()) {
            getline(ss2, neighbour, ' ');
            if (!neighbour.size()) {
                continue;
            }
            int u = stoi(neighbour);
            if (u > v) {
                std::vector<int> edge(2);
                edge[0] = v - 1;
                edge[1] = u - 1;
                edges[index] = edge;
                index++;
            }
        }
    }
    return edges;
}

/**
 * writes results to file for a specified set of graphs
 *
 * @param number number of spanning trees of given graphs
 * @param graphs edge lists of graphs to write to file
 * @param sub file stream to write to
 */
void ProcRegular::graphsToFile(long double number, const std::vector<Edges>& graphs, std::ofstream& sub) {
    sub <<  graphs.size() << " " << std::fixed << number << "\n";
    
    for (const Edges& graph : graphs) {
        sub << "[";
        bool firstEdge = true;
        for (const std::vector<int>& edge : graph) {
            if (firstEdge) {
                firstEdge = false;
            }
            else {
                sub << ", ";
            }
            sub << "(" << edge[0] << ", " << edge[1] << ")";
        }

        sub << "]\n";
    }


}

/**
 * writes results to output file, first for graphs with minimum and maximum number of spanning trees
 */
void ProcRegular::writeToFile(){
    fileTo << "min ";
    graphsToFile(minK, minG, fileTo);
    fileTo << "max ";
    graphsToFile(maxK, maxG, fileTo);

}


/**
 * cumputes number of spanning trees for a given graph and updates max/min data structures
 *
 * @param graph lines with adjacency lists for s graph in string format
 */
void ProcRegular::processGraph(const Lines& graph) {
    Edges edges = getEdges(graph);
    long double numberOfSpanningTrees = counter.countForGraph(edges);
    updateValues(numberOfSpanningTrees, edges);
}

/**
 * reads and processes all graphs from std in
 */
void ProcRegular::processAll() {
    Lines adjacencyListLines(n);
    int index = 0;
    std::string line;
    bool graphStarted = false;
   
    while (std::getline(std::cin, line)) {
        if (line[0] == 'G') {
            graphStarted = true;
        }
        else if (!graphStarted || line.empty()) {
            continue;
        }
        
        else if (graphStarted){
            adjacencyListLines.at(index) = line;
            index++;
            
            if (index == n) {
                processGraph(adjacencyListLines);
                index = 0;
                graphStarted = false;
            }
        }
    }

    writeToFile();
    fileTo.close();
}

Edges ProcRegular::getEdgesBinary(const char* graph) {
    Edges edges(n*reg/2);
    int index = 0;
    int vertex = 0;


    for (size_t i = 0; i < sizeOfGraph - 1; i++) {
            int neighbour = (int) graph[i];
            std::cout << neighbour << " ";
            if (!neighbour) {
                vertex++;
                continue;
            }

            std::vector<int> edge(2);
            //edge[0] = vertex;
            //edge[1] = neighbour - 1;
            //edges[index] = edge;
            index++;
    }
    std::cout << std::endl;
    return edges;
}

void ProcRegular::processGraphBinary(const char* graph) {
    Edges edges = getEdgesBinary(graph);
    //long double numberOfSpanningTrees = counter.countForGraph(edges);
    //std::cout << numberOfSpanningTrees << std::endl;
    //updateValues(numberOfSpanningTrees, edges);
}

/**
 * reads and processes all graphs generated by snarkhunter from std in
 */
void ProcRegular::processAllBinary() {
    //std::ifstream inputGraphs(, std::ios::in | std::ios::binary);
    char * graph = new char [sizeOfGraph];
    std::cin.read(graph, sizeof(char));
    std::cout << (int) graph[0] << " ";

    while (std::cin.good()) {
        std::cin.read(graph, sizeOfGraph);
        processGraphBinary(graph);
    }
    delete [] graph;
    writeToFile();
    fileTo.close();
}