#pragma once
#include <stdio.h>
#include <utility>
#include <vector>
#include <climits>
#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include "cycleCounter.h"
#include "spanningTreeCounting.h"
#include "cycleReducer.h"


class ProcRegular {
    unsigned int maxStroredGraphs = 1000;

    
    using Edges = std::vector<std::vector<int>>;
    using AdjMatrix = std::vector<std::vector<int>>;
    std::vector<Edges> maxG;
    std::vector<Edges> minG;
    long long maxK = 1;
    long long minK = LLONG_MAX;

    std::vector<std::pair<Edges, long long>> minCyclesGraphs;
    long minCycles = LONG_MAX;
    bool printReductions;

    std::vector<std::pair<Edges, long long>> irreducibleGraphs;
    
    
    using Lines = std::vector<std::string>;
    
    virtual void processGraph(int index);

    void graphsToFile(long long number, const std::vector<Edges>& graphs, std::ofstream& sub);
    void cycleGraphsToFile(long number, const std::vector<std::pair<Edges, long long>>& graphs, std::ofstream& sub);
    void irreducibleToFile();
    
    void printGraph(const Edges& graph, std::ofstream& sub);
    

protected:
    int reg;
    int n;
    int minGirth;
    bool cycles;
    std::ofstream fileTo;
    std::ofstream fileCyclesTo;
    std::ofstream fileReductionsTo;
    SpanningTreeCounter counter;
    CycleCounter cycleCounter;
    CycleReducer reducer;
    


public:
    ProcRegular(int part, int parts,  int reg2, int n2, int girth = 3, bool cycleCount = false, bool reductions = false)
    : reg(reg2)
    , n(n2)
    , minGirth(girth)
    , cycles(cycleCount)
    , printReductions(reductions)
    , counter(SpanningTreeCounter(n, reg))
    , cycleCounter(n, minGirth)
    , reducer(n, reg, true)
    {
        std::cout << reg << "-regular graphs on " << n << " vertices, part "<< part << " out of " << parts << std::endl;
        
    };

    ProcRegular(const std::string& file,  int reg2, int n2, int girth = 3, bool cycleCount = false, bool irreducible = false)
    : reg(reg2)
    , n(n2)
    , minGirth(girth)
    , cycles(cycleCount)
    , printReductions(irreducible)
    , fileTo(file)
    , counter(SpanningTreeCounter(n, reg))
    , cycleCounter(n, minGirth)
    , reducer(n, reg, true)
    {
        std::cout << reg << "-regular graphs on " << n << " vertices to file " << file << std::endl;
        if (cycleCount) {
            
            std::string nameCycles = "minCycles" +  std::to_string(reg) + "-" +  std::to_string(n) + "-" + std::to_string(girth) + ".txt";
            std::cout << "storing graphs with min gith cycle count to file " << nameCycles << std::endl;
            fileCyclesTo.open (nameCycles, std::ofstream::out);
        }
        if (printReductions) {
            std::string fileName = "irreducibleGraphs" + std::to_string(reg)+"-"+ std::to_string(n) + ".txt";
            std::cout << "printing irreducible graphs to file " << fileName << std::endl;
            fileReductionsTo.open(fileName, std::ofstream::out);
        }

        if (!fileTo.is_open()) {
            std::cout << "Failed to open output file and write the results" << std::endl;
            exit(1);
        }
        if (cycleCount && !fileCyclesTo.is_open()) {
            std::cout << "Failed to open output file for cycles and write the results" << std::endl;
            exit(1);
        }
        if (printReductions && !fileReductionsTo.is_open()) {
            std::cout << "Failed to open output file for irreducible graphs and write the results" << std::endl;
            exit(1);
        }
    };

    void processAll();
    const std::pair<long long, const std::vector<Edges>> getMax() {
        //std::cout << "max " << maxG.size() << std::endl;
        return std::make_pair(maxK, maxG);
    }
    const std::pair<long long, const std::vector<Edges>> getMin() {
        return std::make_pair(minK, minG);
    }

    const std::pair<long, std::vector<std::pair<Edges, long long>>> getMinCycles() {
        return std::make_pair(minCycles, minCyclesGraphs);
    }
    const std::vector<std::pair<Edges, long long>> getIrreducibleGraphs() {
        return irreducibleGraphs;
    }
    void updateValues(long long number, const Edges& edges);
    void updateCycles(long count, const Edges& edges, long long spanningTrees);
    void updateIrreducibleGraphs(const Edges& edges, long long spanningTrees);
    virtual void writeToFile();

    
    
};
