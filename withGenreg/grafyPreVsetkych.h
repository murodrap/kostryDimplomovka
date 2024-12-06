#ifndef ____GRAFYPREVSETKYCH_H___H__
#define __GRAFYPREVSETKYCH_H__

#pragma once

#include "gendef.h"
#include "processingRegularGraphs.h"
#include <vector>

using AdjMatrix = std::vector<std::vector<int>>;
using Edges = std::vector<std::vector<int>>;

void nastavVelkosti(int spracov);
void nastavPocetGeneratorov(int j);
void pripravaGrafov(int n, int k);
void zapisGrafu(const SCHAR* stupne, SCHAR** susedia);
AdjMatrix& grafNaIndexe(int index);
void koniecGeneratora();
Edges matrixToEdgeList(const AdjMatrix& matrix, int n, int reg);

#endif