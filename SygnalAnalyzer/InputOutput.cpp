#include "InputOutput.h"
#pragma once
#include <iostream>


double* readArrayFromFile(const char* fileName, int* arraySizeOut) {
	FILE* fp;

	fopen_s(&fp, fileName, "r");

	char buffer[100];

	int counter = 0;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		counter++;
	}

	double* result = (double*)calloc(counter, sizeof(double));
	counter = 0;

	rewind(fp);

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		result[counter] = atof(buffer);
		counter++;
	}

	*arraySizeOut = counter;

	fclose(fp);
	return result;
}
