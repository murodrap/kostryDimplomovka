#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <type_traits>
#include <vector>

#include "processingRegularGraphs.h"
#include "grafyPreVsetkych.h"

#include "main.h"

int main(int argc, char *argv[])

{
    std::vector<std::thread> thready;
    
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int j = atoi(argv[3]);
    int t = 3;
    if (argc == 5) {
        std::cout << "mame aj minimalny girth" << std::endl;
        t = atoi(argv[4]);
    }


    
    

    int pocetSpracovacov = 2 * j;
    nastavVelkosti(pocetSpracovacov);
    nastavPocetGeneratorov(j);
    pripravaGrafov(n, k);
    
    std::vector<GenregSpustanie*> generatory;
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "pocet generatorov " << j << std::endl;
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
        ProcRegular *pro = new ProcRegular(i, pocetSpracovacov, k, n);
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
    ProcRegular proc(file, k, n);
    for (int i = 0; i < pocetSpracovacov; i++) {
        
        const std::pair<long long, const std::vector<Edges>> vysledky = spracovania[i]->getMax();
        //std::cout << "vusl dl " << std::endl;
        long long pocet = vysledky.first;
        //std::cout << "xxxxxx " << std::endl;
        const std::vector<Edges>& grafy = vysledky.second;
        //std::cout << grafy.size() << std::endl;
        for (const Edges& graf : grafy) {
            //std::cout << "graf" << std::endl;
            proc.updateValues(pocet, graf);
        }
        
        auto vysledky2 = spracovania[i]->getMin();
        auto pocet2 = vysledky2.first;
        auto grafy2 = vysledky2.second;
        for (auto graf : grafy2) {
            proc.updateValues(pocet2, graf);
        }
        
       //spracovania[i]->uuu();
        delete spracovania[i];
    }
    proc.writeToFile();
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