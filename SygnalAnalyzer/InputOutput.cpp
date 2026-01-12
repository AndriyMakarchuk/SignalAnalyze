#include "InputOutput.h"
#pragma once
#include <iostream>

static void checkFileExistence(FILE* fp, const char* fileName);

void addSignal(const char* signalName, double* values, int valuesLength) {
	FILE* fp;

	if (signalName == nullptr) {
		throw std::invalid_argument("Signal name is nullptr");
	}
	if (values == nullptr) {
		throw std::invalid_argument("Values array is nullptr");
	}

	if (valuesLength <= 0) {
		throw std::invalid_argument("Array size must be greater than 0");
	}

	fopen_s(&fp, signalName, "r");

	if (fp != nullptr) {
		fclose(fp);
		throw std::invalid_argument("Signal already exists");
	}

	fopen_s(&fp, signalName, "w");
	char buffer[25];
	for (int i = 0; i < valuesLength; i++)
	{
		memset(buffer, 0, sizeof(buffer));
		sprintf_s(buffer, "%f", values[i]);
		fprintf_s(fp,buffer);
		fprintf_s(fp, "\n");
	}

	fclose(fp);
}

double* readArrayFromFile(const char* fileName, int* arraySizeOut) {
	if (fileName == nullptr)
	{
		throw std::invalid_argument("fileName is nullptr");
	}

	FILE* fp;

	fopen_s(&fp, fileName, "r");

	checkFileExistence(fp, fileName);

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

static void checkFileExistence(FILE* fp, const char* fileName) {
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
