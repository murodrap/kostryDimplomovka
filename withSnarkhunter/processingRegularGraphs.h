#pragma once
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <fstream>
#include <iostream>
#include <cfloat>
#include "spanningTreeCounting.h"


class ProcRegular {
    unsigned int maxStroredGraphs = 1000;

    
    using Edges = std::vector<std::vector<int>>;
    std::vector<Edges> maxG;
    std::vector<Edges> minG;
    long double maxK = 1;
    long double minK = LDBL_MAX;

    size_t sizeOfGraph;
    
    
    using Lines = std::vector<std::string>;
    
    virtual void processGraph(const Lines& graph);
    virtual void processGraphBinary(const char* graph);
    long podlaVzorca();

    void graphsToFile(long double number, const std::vector<Edges>& graphs, std::ofstream& sub);
    virtual void writeToFile();

protected:
    int reg;
    int n;
    std::ofstream fileTo;
    SpanningTreeCounter counter;
    void updateValues(long double number, const Edges& edges);
    Edges getEdges(const Lines& lines);
    Edges getEdgesBinary(const char* graph);


public:
    ProcRegular(std::string file, int reg2, int n2)
    : reg(reg2)
    , n(n2)
    , fileTo(file)
    , counter(SpanningTreeCounter(n, reg))
    {
        std::cout << reg << "-regular graphs on " << n << " vertices to file "<< file << std::endl;

        if (!fileTo.is_open()) {
            std::cout << "Failed to open output file and write the results" << std::endl;
            exit(1);
        }
        sizeOfGraph = n + (n * reg) / 2;
    };

    void processAll();
    void processAllBinary();
    
    
};
