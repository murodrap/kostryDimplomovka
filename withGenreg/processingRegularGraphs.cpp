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

#include "grafyPreVsetkych.h"

std::mutex m, m2;
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
    grafyNaSpracovanie = std::vector<Edges>(velkostBuffera);
}



void nastavPocetGeneratorov(int j) {
    pocetGeneratorov = j;
}

void pripravaGrafov(int n, int k) {
    nG = n;
    kG = k;
    int hran = nG * kG / 2;
    for (int i = 0; i < velkostBuffera; i++) {
        for (int h = 0; h < hran; h++) {
            grafyNaSpracovanie[i].push_back({0, 0});
        }
    }
}

const Edges& grafNaIndexe(int index) {
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
    
    //std::cout << "pisem na ind " << fillInd << std::endl;
    int indexHrany = 0;
    for(int i= 1; i <= nG; i++) {
        for(int j = 1; j <= stupne[i]; j++) {
            if (susedia[i][j] >= i) {
                break;
            }
            grafyNaSpracovanie[indexBuffera][indexHrany][0] = susedia[i][j] - 1;
            grafyNaSpracovanie[indexBuffera][indexHrany][1] = i - 1;
            indexHrany++;
        }
    }
    
    plnostPoliciekBuffera[indexBuffera]++;
    
    //std::cout << "po zapise " << fillInd << std::endl;
    
    lck.unlock();  
    fill[indexBuffera].notify_one(); 
}

void koniecGeneratora() {
    //std::cout << "koncim gen\n";
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

/**
 * checks whether the given graph doesn't have equal or lower/higher nuber of spanning trees that the currently stored min/max values
 * updates the according values if necessary
 *
 * @param number lines with adjacency lists for s graph in string format
 * @param edges lines with adjacency lists for s graph in string format
 */
void ProcRegular::updateValues(long long number, const Edges& edges) {
    //std::cout << "upd\n";

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
 * writes results to file for a specified set of graphs
 *
 * @param number number of spanning trees of given graphs
 * @param graphs edge lists of graphs to write to file
 * @param sub file stream to write to
 */
void ProcRegular::graphsToFile(long long number, const std::vector<Edges>& graphs, std::ofstream& sub) {
    sub <<  graphs.size() << " " << number << "\n";
    
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
    fileTo.close();

}


/**
 * cumputes number of spanning trees for a given graph and updates max/min data structures
 *
 * @param graph lines with adjacency lists for s graph in string format
 */
void ProcRegular::processGraph(int indexGrafu) {
    const Edges& edges = grafNaIndexe(indexGrafu);

    long long numberOfSpanningTrees = counter.countForGraph(edges);
    //std::cout << numberOfSpanningTrees << std::endl;
    //long long numberOfSpanningTrees = 0;
    updateValues(numberOfSpanningTrees, edges);
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
        //std::cout<< pocetGeneratorov << " " << count << std::endl;
        std::unique_lock<std::mutex> lck(lockyPolickaBuffera[indexBuffera]);
        fill[indexBuffera].wait( lck, [ & ] ( )
        {
            return plnostPoliciekBuffera[indexBuffera] == 1 || !pocetGeneratorov;
        });
        //std::cout << "--------" << pocetGeneratorov << " " << count << std::endl;
        if (!plnostPoliciekBuffera[indexBuffera] && !pocetGeneratorov) {
            lck.unlock();
            break;
        }
        //std::cout << "citanie\n";
        processGraph(indexBuffera);

       
        plnostPoliciekBuffera[indexBuffera]--;
        //std::cout << "po spracovani " << useInd << std::endl;
     
        
        lck.unlock();
        empty[indexBuffera].notify_one(); 
    }
    //std::cout << "idem pisat do suboru\n";
}