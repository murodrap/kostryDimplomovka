#include <algorithm>
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
#include <condition_variable>
#include <mutex>

#include "spanningTreeCounting.h"
#include "processingRegularGraphs.h"
#include "grafyPreVsetkych.h"

using Edges = std::vector<std::vector<int>>;
using AdjMatrix = std::vector<std::vector<int>>;

#include "grafyPreVsetkych.h"
#include "cycleCounter.h"
#include "cycleReducer.h"

std::mutex m, m2, irreducibleFile; 
int velkostBuffera;

std::vector<std::condition_variable> empty;
std::vector<std::condition_variable> fill;

std::vector<std::mutex> lockyPolickaBuffera;
std::vector<int> plnostPoliciekBuffera;
std::vector<Edges> grafyNaSpracovanie;
int pocetGeneratorov;
int nG;
int kG;

int fillInd = 0;
int useInd = 0;

void nastavVelkosti(int spracov) {
    velkostBuffera = 2 * spracov;
    empty = std::vector<std::condition_variable>(velkostBuffera);
    fill = std::vector<std::condition_variable>(velkostBuffera);
    lockyPolickaBuffera = std::vector<std::mutex>(velkostBuffera);
    plnostPoliciekBuffera = std::vector<int>(velkostBuffera);
    grafyNaSpracovanie = std::vector<AdjMatrix>(velkostBuffera);
}



void nastavPocetGeneratorov(int j) {
    pocetGeneratorov = j;
}

void pripravaGrafov(int n, int k) {
    nG = n;
    kG = k;
    int hran = nG * kG / 2;
    for (int i = 0; i < velkostBuffera; i++) {
        for (int r = 0; r < n; r++) {
            grafyNaSpracovanie[i].push_back(std::vector<int>(n));
        }
    }
}

AdjMatrix& grafNaIndexe(int index) {
    return grafyNaSpracovanie[index];
}

void zapisGrafu(const SCHAR* stupne, SCHAR** susedia) {
    m.lock();
    int indexBuffera = fillInd;
    fillInd = (fillInd + 1) % velkostBuffera;
    m.unlock();

    std::unique_lock<std::mutex> lck(lockyPolickaBuffera[indexBuffera]);
    empty[indexBuffera].wait( lck, [ & ] ( )
        {
            return plnostPoliciekBuffera[indexBuffera] == 0;
        });
    
    for (int i = 0; i < nG; i++) {
        for (int j = 0; j < nG; j++) {
            grafyNaSpracovanie[indexBuffera][i][j] = 0;
        }
        
    }
    for(int i= 1; i <= nG; i++) {
        for(int j = 1; j <= stupne[i]; j++) {
            grafyNaSpracovanie[indexBuffera][i - 1][susedia[i][j] - 1] = 1;
        }
    }
    
    plnostPoliciekBuffera[indexBuffera]++;
    
    
    lck.unlock();  
    fill[indexBuffera].notify_one(); 
}

void koniecGeneratora() {
    m2.lock();
    pocetGeneratorov--;
    if (!pocetGeneratorov) {
        for (int i = 0; i < velkostBuffera; i++) {
            //m.unlock();
            fill[i].notify_all();
        }
        
    }
    m2.unlock();
}


 Edges matrixToEdgeList(const AdjMatrix& matrix, int n, int reg) {
    int numberOfEdges = n * reg / 2;
    Edges edgeList = Edges();
    edgeList.reserve(numberOfEdges);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (matrix[i][j]) {
                edgeList.push_back({i, j});
            }
        }
    }

    return edgeList;
 }


/**
 * checks whether the given graph doesn't have equal or lower/higher nuber of spanning trees that the currently stored min/max values
 * updates the according values if necessary
 *
 * @param number lines with adjacency lists for s graph in string format
 * @param edges lines with adjacency lists for s graph in string format
 */
void ProcRegular::updateValues(long long number, const Edges& edges) {

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

 void ProcRegular::updateCycles(long count, const Edges& edges, long long spanningTrees) {
    if (count == minCycles) {
        if (minCyclesGraphs.size() < maxStroredGraphs) {
            minCyclesGraphs.push_back({edges, spanningTrees});
        }
    }

    if (count < minCycles) {
        minCycles = count;
        minCyclesGraphs.clear();
        minCyclesGraphs.push_back({edges, spanningTrees});
    }
 }




/**
 * writes results to file for a specified set of graphs
 *
 * @param number number of spanning trees of given graphs
 * @param graphs edge lists of graphs to write to file
 * @param sub file stream to write to
 */
void ProcRegular::graphsToFile(long long number, const std::vector<Edges>& graphs, std::ofstream& sub) {
    sub <<  graphs.size() << " graphs, " << number << "\n";
    
    for (const Edges& graph : graphs) {
        printGraph(graph, sub);
    }
}

void ProcRegular::cycleGraphsToFile(long number, const std::vector<std::pair<Edges, long long>>& graphs, std::ofstream& sub) {
    sub << "graphs with minimum number of cycles of girth=" << minGirth << std::endl;
    sub <<  graphs.size() << " graphs, " << number << "\n";
    
    for (auto graphInfo : graphs) {
        sub << graphInfo.second << " spanning trees" << std::endl;
        printGraph(graphInfo.first, sub);
        sub <<std::endl;
    }
}

void ProcRegular::printGraph(const Edges& graph, std::ofstream& sub) {
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

/**
 * writes results to output file, first for graphs with minimum and maximum number of spanning trees
 */
void ProcRegular::writeToFile(){
    if (printReductions) {
        irreducibleToFile();
        fileReductionsTo.close();
        return;
    }
    fileTo << "min ";
    graphsToFile(minK, minG, fileTo);
    fileTo << "max ";
    graphsToFile(maxK, maxG, fileTo);
    fileTo.close();
    if (cycles) {
        cycleGraphsToFile(minCycles, minCyclesGraphs, fileCyclesTo);
        fileCyclesTo.close();
    }
    

}

void ProcRegular::updateIrreducibleGraphs(const Edges& edges, long long spanningTrees) {
    irreducibleGraphs.push_back({edges, spanningTrees});
}
void ProcRegular::irreducibleToFile(){
    for (auto graph : irreducibleGraphs) {
        fileReductionsTo  << graph.second << " ";
        printGraph(graph.first, fileReductionsTo);
    }
    
}


/**
 * cumputes number of spanning trees for a given graph and updates max/min data structures
 *
 * @param graph lines with adjacency lists for s graph in string format
 */
void ProcRegular::processGraph(int indexGrafu) {
    AdjMatrix& matrix = grafNaIndexe(indexGrafu);


    long long numberOfSpanningTrees = counter.countForGraph(matrix);
    
    Edges edgeList = matrixToEdgeList(matrix, n , reg);
    //std::cout << numberOfSpanningTrees << std::endl;

    if (printReductions) {
        //printGraph(edgeList, std::cout);
        if (!reducer.processGraph(&matrix, numberOfSpanningTrees)) {
            updateIrreducibleGraphs(edgeList, numberOfSpanningTrees);
            //irreducibleFile.lock();
            //std::cout << numberOfSpanningTrees << std::endl;
            //fileReductionsTo <<"-" << numberOfSpanningTrees << "- ";
            //printGraph(edgeList, fileReductionsTo);
            //irreducibleFile.unlock();
        }
        return;
    }
    updateValues(numberOfSpanningTrees, edgeList);
    if (cycles) {
        long cycleCount = cycleCounter.countForGraph(matrix);
        updateCycles(cycleCount, edgeList, numberOfSpanningTrees);
    }
}

/**
 * reads and processes all graphs from std in
 */
void ProcRegular::processAll() {
    while (true) {
        m.lock();
        int indexBuffera = useInd;
        useInd = (useInd + 1) % velkostBuffera;
        m.unlock();
        std::unique_lock<std::mutex> lck(lockyPolickaBuffera[indexBuffera]);
        fill[indexBuffera].wait( lck, [ & ] ( )
        {
            return plnostPoliciekBuffera[indexBuffera] == 1 || !pocetGeneratorov;
        });

        if (!plnostPoliciekBuffera[indexBuffera] && !pocetGeneratorov) {
            lck.unlock();
            break;
        }

        processGraph(indexBuffera);


       
        plnostPoliciekBuffera[indexBuffera]--;

     
        
        lck.unlock();
        empty[indexBuffera].notify_one(); 
    }
    if (printReductions) {
        fileReductionsTo.close();
    }

}