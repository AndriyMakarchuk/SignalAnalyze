#pragma once
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>

double* readArrayFromFile(const char* fileName, int* arraySizeOut);
void addSignal(const char* signalName, double* values, int valuesLength);
