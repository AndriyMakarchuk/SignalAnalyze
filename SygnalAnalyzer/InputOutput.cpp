#include "InputOutput.h"
#pragma once
#include <iostream>

static void checkFileExistence(const FILE* fp, const char* fileName);

/// <summary>
/// Adds information about reference signal to file
/// </summary>
/// <param name="signalName">Name of signal. Will be used as file name</param>
/// <param name="values">Array of values</param>
/// <param name="valuesLength">Array length</param>
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

	if (fp == nullptr) {
		throw std::invalid_argument("failed to open file");
	}

	checkFileExistence(fp, fileName);

	char buffer[100];

	int counter = 0;

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		counter++;
	}

	double* result = static_cast<double*>(calloc(counter, sizeof(double)));

	if (result == nullptr) {
		throw new std::system_error(std::error_code(), "Failed to allocate memory");
	}

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

static void checkFileExistence(const FILE* fp, const char* fileName) {
	if (fp == nullptr) {
		char message[255];
		try {
			strcpy_s(message, "File with name ");
			strcat_s(message, fileName);
			strcat_s(message, " wasn't found");
		}
		catch (const std::exception* e) {
			char longMessage[2048];
			try {
				strcpy_s(longMessage, "File with name ");
				strcat_s(longMessage, fileName);
				strcat_s(longMessage, " wasn't found");
			}
			catch (const std::exception* e1)
			{
				throw std::invalid_argument("File name is too long");
			}
			throw std::invalid_argument(longMessage);
		}
		throw std::invalid_argument(message);
	}
}
