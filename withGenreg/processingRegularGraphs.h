#pragma once
#include <stdio.h>
#include <vector>
#include <climits>
#include <string>
#include <fstream>
#include <iostream>
#include <limits>
#include "spanningTreeCounting.h"


class ProcRegular {
    unsigned int maxStroredGraphs = 1000;

    
    using Edges = std::vector<std::vector<int>>;
    std::vector<Edges> maxG;
    std::vector<Edges> minG;
    long long maxK = 1;
    
    long long minK = LLONG_MAX;
    
    
    using Lines = std::vector<std::string>;
    
    virtual void processGraph(int index);
    long podlaVzorca();

    void graphsToFile(long long number, const std::vector<Edges>& graphs, std::ofstream& sub);
    

protected:
    int reg;
    int n;
    std::ofstream fileTo;
    SpanningTreeCounter counter;
    


public:
    ProcRegular(int part, int parts,  int reg2, int n2)
    : reg(reg2)
    , n(n2)
    , counter(SpanningTreeCounter(n, reg))
    {
        std::cout << reg << "-regular graphs on " << n << " vertices, part "<< part << " out of " << parts << std::endl;
    };

    ProcRegular(const std::string& file,  int reg2, int n2)
    : reg(reg2)
    , n(n2)
    , fileTo(file)
    , counter(SpanningTreeCounter(n, reg))
    {
        std::cout << reg << "-regular graphs on " << n << " vertices to file " << file << std::endl;

        if (!fileTo.is_open()) {
            std::cout << "Failed to open output file and write the results" << std::endl;
            exit(1);
        }
    };

    void processAll();
    const std::pair<long long, const std::vector<Edges>> getMax() {
        //std::cout << "max " << maxG.size() << std::endl;
        return std::pair(maxK, maxG);
    }
    const std::pair<long long, const std::vector<Edges>> getMin() {
        return std::pair(minK, minG);
    }
    void updateValues(long long number, const Edges& edges);
    virtual void writeToFile();

    
    
};
