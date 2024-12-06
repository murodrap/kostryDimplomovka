#include <cstring>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <type_traits>
#include <vector>

#include "processingRegularGraphs.h"
#include "grafyPreVsetkych.h"

#include "main.h"

using Edges = std::vector<std::vector<int>>;

int main(int argc, char *argv[])

{
    std::vector<std::thread> thready;
    
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int j = atoi(argv[3]);
    int t = 3;
    bool cyucleCounting = false;
    bool irrecucible = false;
    if (argc >= 5) {
        
        t = atoi(argv[4]);
        std::cout << "minimal girth specified to " << t << std::endl;
    }
    if (argc == 6) {
        if (std::strcmp(argv[5], "c") == 0) {
            std::cout << "counting cycles of girth length " << t << std::endl;
            cyucleCounting = true;
        }
        if (std::strcmp(argv[5], "r") == 0) {
            irrecucible = true;
        }
    }

    int pocetSpracovacov = 2 * j;
    //if (j == 1 && t > 3) {
    //    pocetSpracovacov = 1;
    //}
    //pocetSpracovacov = 1;
    nastavVelkosti(pocetSpracovacov);
    nastavPocetGeneratorov(j);
    pripravaGrafov(n, k);
    
    std::vector<GenregSpustanie*> generatory;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "generators: " << j << std::endl;
    if (j == 1) {
        GenregSpustanie* gs = new GenregSpustanie(n, k, t);
        generatory.emplace_back(gs);
        thready.emplace_back(std::thread(&GenregSpustanie::mainGenreg, gs));
    }
    else {
        for (int i = 1; i <= j; i++) {
            
            GenregSpustanie* gs = new GenregSpustanie(n, k, t,  i, j);
            generatory.emplace_back(gs);
            thready.emplace_back(std::thread(&GenregSpustanie::mainGenreg, gs));
        }
    }

    std::vector<ProcRegular*> spracovania;
    for (int i = 1; i <= pocetSpracovacov; i++) {
        ProcRegular *pro = new ProcRegular(i, pocetSpracovacov, k, n, t, cyucleCounting, irrecucible);
        spracovania.emplace_back(pro);

        thready.emplace_back(std::thread(&ProcRegular::processAll, pro));
    }
    
   
    for (int i = 0; i < j + pocetSpracovacov; i++) {
        if (thready[i].joinable()) {
            thready[i].join();
        }
    }

    std::string file;
    file = "maxMinReg" +  std::to_string(k) + "-" +  std::to_string(n) + "-" + std::to_string(t)  + "-" +  std::to_string(j) + "-" + std::to_string(pocetSpracovacov) + ".txt";
    ProcRegular proc(file, k, n, t, cyucleCounting, irrecucible);
    for (int i = 0; i < pocetSpracovacov; i++) {
        //std::cout << "index  " << i << ", spracovacov " << pocetSpracovacov << std::endl;
        const std::pair<long long, const std::vector<Edges>> vysledky = spracovania[i]->getMax();
        //std::cout << "vusl dl " << std::endl;
        long long pocet = vysledky.first;
        //std::cout << "xxxxxx " << pocet << std::endl;
        const std::vector<Edges>& grafy = vysledky.second;
        //std::cout << grafy.size() << " graphs" << std::endl;
        for (const Edges& graf : grafy) {
            //std::cout << "graf" << std::endl;
            proc.updateValues(pocet, graf);
            //std::cout << "updated\n";
        }
        
        auto vysledky2 = spracovania[i]->getMin();
        auto pocet2 = vysledky2.first;
        auto grafy2 = vysledky2.second;
        if (pocet2 != pocet) {
            for (auto graf : grafy2) {
            proc.updateValues(pocet2, graf);
        }
        }
        

        auto vysledkyGirth = spracovania[i]->getMinCycles();
        auto pocetGirth = vysledkyGirth.first;
        auto grafyGirth = vysledkyGirth.second;
        for (auto graf : grafyGirth) {
            proc.updateCycles(pocetGirth, graf.first, graf.second);
        }
        if (irrecucible) {
            auto vysledkyIrreducible = spracovania[i]->getIrreducibleGraphs();
            for (auto graphPair : vysledkyIrreducible) {
                proc.updateIrreducibleGraphs(graphPair.first, graphPair.second);
            }
            
            
        }
        
       //spracovania[i]->uuu();
       //std::cout << "idem mazat " << i << std::endl;
        delete spracovania[i];
        //std::cout << "vyamazal som\n";
    }
    //std::cout << "konecny zapis\n";
    proc.writeToFile();
    //std::cout << "zapisal som do suboru\n";
    for (int i = 0; i < j; i++) {
        delete generatory[i];
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto processingTime = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "generating and processing finished after " << processingTime << " seconds\n";
    //std::cout << "koniec" << std::endl;
    return 0;

    //return gs1.mainGenreg(4, vstup1);
    /*
    std::string file;
    int n;
    int k;
    if (argc == 3) {
        n = std::atoi(argv[1]);
        k = std::atoi(argv[2]);
        file = "maxMinReg" +  std::to_string(k) + "-" +  std::to_string(n) + ".txt";
    }
    else if (argc == 4) {
        std::string s(argv[1]);
        file = s;
        n = std::atoi(argv[2]);
        k = std::atoi(argv[3]);
    }
    else {
        std::cout << "incorrect number of arguments" << std::endl;
        return 1;
    }

    ProcRegular pro(file, k, n);

    auto start = std::chrono::high_resolution_clock::now();
    pro.processAll();
    auto end = std::chrono::high_resolution_clock::now();
    auto processingTime = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    std::cout << "processing finished after " << processingTime << " seconds\n";
    */
}