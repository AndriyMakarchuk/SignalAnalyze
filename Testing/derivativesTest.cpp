#include "pch.h"
#include "../SygnalAnalyzer/derivative.cpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MathFunctionsTests
{
	TEST_CLASS(DerivativesTest)
	{
	public:
		void testFunctionDerivatives(double (*func)(double x), double (*der)(double x)) {

			//Arrange
			float relativePrecision = 0.05;
			float precision = 0.05;
			int size = 50;
			int h = 1;
			double* input = (double*)calloc(size, sizeof(double));
			double* expectedResult = (double*)calloc(size, sizeof(double));

			for (int i = 0; i < size; i++) {
				input[i] = func(i - size / 2.0);
				expectedResult[i] = der(i - size / 2.0);
			}

			//Act
			double* result = derivative(input, size, h);

			//Assert
			for (int i = 3; i < size - 6; i++) {
				double maxAbs = abs(result[i]) > abs(expectedResult[i]) ? abs(result[i]) : abs(expectedResult[i]);
				bool assert = (abs(expectedResult[i]) < 0.00001 && abs(result[i]) < 0.00001)
					|| (abs(expectedResult[i] - result[i]) / maxAbs < relativePrecision || abs(expectedResult[i] - result[i]) < precision);
				if (!assert) {
					std::wostringstream woss;
					woss << L"Error at index " << i
						<< L": result = " << result[i]
						<< L", expected = " << expectedResult[i];
					Logger::WriteMessage(woss.str().c_str());
				}
				Assert::IsTrue(assert);
			}

			//Cleanup
			free(input);
			free(expectedResult);
			free(result);
		}

		static double square(double x) {
			return x * x;
		}

		static double squareDerivative(double x) {
			return x * 2;
		}

		TEST_METHOD(TestDerivativeSquareFunction) {
			testFunctionDerivatives(square, squareDerivative);
		}

		TEST_METHOD(TestDerivativeSinusFunction) {
			testFunctionDerivatives(sin, cos);
		}

		TEST_METHOD(TestSmallArray) {
			//Arrange

			int size = 3;
			int h = 1;
			double* input = (double*)calloc(size, sizeof(double));

			//Assert
			Assert::ExpectException<std::invalid_argument>([&]() { derivative(input, size, h); });

			//Cleanup
			free(input);
		}


		TEST_METHOD(TestNullPtr) {
			//Arrange

			int size = 10;
			int h = 1;
			double* input = nullptr;

			//Assert
			Assert::ExpectException<std::invalid_argument>([&]() { derivative(input, size, h); });
		}
	};
}