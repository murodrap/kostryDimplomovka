#ifndef ____GRAFYPREVSETKYCH_H___H__
#define __GRAFYPREVSETKYCH_H__

#pragma once

#include "gendef.h"
#include "processingRegularGraphs.h"
#include <vector>

using Edges = std::vector<std::vector<int>>;

void nastavVelkosti(int spracov);
void nastavPocetGeneratorov(int j);
void pripravaGrafov(int n, int k);
void zapisGrafu(const SCHAR* stupne, SCHAR** susedia);
const Edges& grafNaIndexe(int index);
void koniecGeneratora();

#endif