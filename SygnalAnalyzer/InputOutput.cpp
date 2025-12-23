#include "InputOutput.h"
#pragma once
#include <iostream>

static void CheckFileExistence(FILE* fp, const char* fileName);

double* readArrayFromFile(const char* fileName, int* arraySizeOut) {
	if(fileName == nullptr)
	{
		throw std::invalid_argument("fileName is nullptr");
	}

	FILE* fp;

	fopen_s(&fp, fileName, "r");

	CheckFileExistence(fp, fileName);

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

static void CheckFileExistence(FILE* fp, const char* fileName) {
	if (fp == nullptr) {
		char message[255];
		try {
			strcpy_s(message, "File with name ");
			strcat_s(message, fileName);
			strcat_s(message, " wasn't found");
		}
		catch (std::exception e) {
			char longMessage[2048];
			try {
				strcpy_s(message, "File with name ");
				strcat_s(message, fileName);
				strcat_s(message, " wasn't found");
			}
			catch (std::exception e)
			{
				throw std::invalid_argument("File name is too long");
			}
			throw std::invalid_argument(message);
		}
		throw std::invalid_argument(message);
	}


}
