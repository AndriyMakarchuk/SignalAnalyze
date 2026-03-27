#include<stdio.h>
#include<math.h>
#include"inputOutput.h"
#include"mathFunctions.h"


double* calcNpropValues(double* funcValues, int size);

int main()
{
	double* yOrig, * yInput;

	int origSize, inputSize;

	yOrig = readArrayFromFile("orig.txt", &origSize);
	yInput = readArrayFromFile("Input.txt", &inputSize);

	double* nPropOrig;
	double* nPropInput;

	try {
		nPropOrig = calcNpropValues(yOrig, origSize);
		nPropInput = calcNpropValues(yInput, inputSize);
	}
	catch (const std::invalid_argument* e) {
		printf("Reading error. Each file must contain only numbers, and at least 13 numbers");
		return 1;
	}
	catch (const std::system_error e) {
		printf("System error:");
		printf(e.what());
		return 2;
	}
	catch (const std::exception e) {
		printf("Internal error: ");
		printf(e.what());
		return 3;
	}

	printf("Rechognized sygnals: \n");
	bool atLeastOneSygnalFound = false;

	for (int i = 2; i < inputSize - 12; i++) {
		if (abs(nPropInput[i] - nPropOrig[2]) < 0.02f * nPropInput[i]) {

			float k = -1;
			int t = -1;
			float a = -1;

			for (int j = 2; j < origSize - 12; j++) {
				if (abs(nPropInput[i] - nPropOrig[j]) < abs(0.02f * nPropInput[i + 1])) {
					k = 1.0 / (j - 2);
					t = j;
					a = yInput[i + 1] / yOrig[j];
				}
			}

			for (int j = i + 1; j < inputSize - 12; j++) {
				if (abs(nPropInput[j] - nPropOrig[3]) < 0.02f * nPropInput[j]) {
					k = j - i;
					t = j;
					a = yInput[j] / yOrig[3];
				}
			}

			bool sygnalFound = true;

			if (k > 0 && k < 1) {
				for (int j = 1; j < floor(origSize * k); j++) {
					if (abs(yInput[t + j] - yOrig[2 + (int)round(j * k)] * a) > abs(0.02 * yOrig[j])
						&& yOrig[j] > 0.0001) {
						printf("%f - %f\n", yInput[t + j], yOrig[2 + (int)round(j * k)]);
						sygnalFound = false;
					}
				}
			}
			else if (k > 1) {
				for (int j = 3; j < origSize; j++) {
					if (abs(yInput[(int)round((j - 3) * k + t)] - yOrig[j] * a) > abs(0.02 * yOrig[j])
						&& yOrig[j] > 0.0001) {
						sygnalFound = false;
					}
				}
			}
			else {
				sygnalFound = false;
			}

			if (sygnalFound) {
				atLeastOneSygnalFound = true;
				printf("Sygnal found. k=%f, t = %i, A = %f", k, t, a);
			}
		}
	}

	if (!atLeastOneSygnalFound) {
		printf("No sygnals were found");
	}

	free(yOrig);
	free(yInput);
	free(nPropInput);
	free(nPropOrig);
}

double* calcNpropValues(double* funcValues, int size) {
	if (size < 13) {
		throw std::invalid_argument("Can not calculate nprop values. At least 13 values required");
	}

	double* result = (double*)tryCalloc(size, sizeof(double));
	double* der1 = (double*)tryCalloc(size - 6, sizeof(double));
	double* der2 = (double*)tryCalloc(size - 12, sizeof(double));
	derivative(funcValues, der1, size, 1);
	derivative(der1, der2, size - 6, 1);

	for (int i = 2; i < size - 12; i++)
	{
		if (funcValues[i] && der2[i])
		{
			result[i] = 1 - pow(der1[i], 2) / (funcValues[i] * der2[i]);
		}
	}

	free(der1);
	free(der2);

	return result;
}

void* tryCalloc(size_t count, size_t size) {
	void* result = calloc(count, size);

	if (result == nullptr){
		throw std::system_error(std::error_code(), "Failed memory allocation");
	}

	return result;
}
