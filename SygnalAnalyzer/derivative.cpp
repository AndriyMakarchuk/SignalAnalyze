#pragma once
#include "mathFunctions.h"
#include <iostream>

void derivative(double* y, double* dy_r, int n, double h) {
	int i;

	if (n < 6) {
		throw std::invalid_argument("Exception: array must have at least 6 elements");
	}

	if (y == nullptr) {
		throw std::invalid_argument("Exception: array pointer is nullptr");
	}

	double* dy1 = (double*)calloc(n, sizeof(double));
	double* dy2 = (double*)calloc(n, sizeof(double));
	double* dy3 = (double*)calloc(n, sizeof(double));
	double* dy4 = (double*)calloc(n, sizeof(double));
	double* dy5 = (double*)calloc(n, sizeof(double));
	double* dy6 = (double*)calloc(n, sizeof(double));

	for (i = 0; i < n - 1; i++)
	{
		dy1[i] = y[i + 1] - y[i];
	}
	for (i = 0; i < n - 2; i++)
	{
		dy2[i] = dy1[i + 1] - dy1[i];
	}
	for (i = 0; i < n - 3; i++)
	{
		dy3[i] = dy2[i + 1] - dy2[i];
	}
	for (i = 0; i < n - 4; i++)
	{
		dy4[i] = dy3[i + 1] - dy3[i];
	}
	for (i = 0; i < n - 5; i++)
	{
		dy5[i] = dy4[i + 1] - dy4[i];
	}
	for (i = 0; i < n - 6; i++)
	{
		dy6[i] = dy5[i + 1] - dy5[i];
	}
	for (i = 1; i < n - 6; i++)
	{
		dy_r[i] = (dy1[i - 1] + dy2[i - 1] / 2 - dy3[i - 1] / 6 + dy4[i - 1] / 24 - dy5[i - 1] / 120 + dy6[i - 1] / 720) / h;
	}
	free(dy1);
	free(dy2);
	free(dy3);
	free(dy4);
	free(dy5);
	free(dy6);

}


double* derivative(double* y, int n, double h) {
	double* result = (double*)calloc(n - 6, sizeof(double));

	derivative(y, result, n, h);

	return result;
}