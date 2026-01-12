#include "pch.h"

#pragma once
#include "../SygnalAnalyzer/inputOutput.cpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace InputOutput
{
	TEST_CLASS(SignalsAddingTesting)
	{
	public:

		TEST_METHOD(TestReadArrayFromFile)
		{
			//Arrange
			const char* filename = "readarray_test_values.txt";

			std::ofstream ofs(filename);
			Assert::IsTrue(ofs.is_open(), L"Failed to create test input file.");

			std::vector<double> expected = { 1.5, 2.25, -3.0, 0.0, 4.125 };
			for (double v : expected) ofs << v << std::endl;
			ofs.close();

			//Act
			int size = 0;
			double* arr = readArrayFromFile(filename, &size);

			//Assert
			Assert::IsNotNull(arr, L"readArrayFromFile returned null.");
			Assert::AreEqual(static_cast<int>(expected.size()), size, L"Array size mismatch.");

			for (int i = 0; i < size; ++i) {
				Assert::AreEqual(expected[i], arr[i], 1e-9, L"Value mismatch at index.");
			}

			//Cleanup
			free(arr);
			std::remove(filename);
		}

		TEST_METHOD(TestReadArrayNameNullptr)
		{
			//Assert
			Assert::ExpectException<std::invalid_argument>([&]() { readArrayFromFile(nullptr, nullptr); });
		}

		TEST_METHOD(TestReadArrayFromNonExistentFile)
		{
			//Arrange
			const char* filename = "non_existent_file.txt";

			std::remove(filename);
			//Assert
			Assert::ExpectException<std::invalid_argument>([&]() { readArrayFromFile(filename, nullptr); });
		}
	};


	TEST_CLASS(AddSignalTests)
	{
	public:

		TEST_METHOD(TestAddSignal_CorrectAdd)
		{
			const char* filename = "addsignal_correct.txt";
			std::remove(filename);

			std::vector<double> values = { 1.5, -2.0, 3.14159, 0.0 };
			int len = static_cast<int>(values.size());

			// Act
			addSignal(filename, values.data(), len);

			// Ensure stdio buffers are flushed so readArrayFromFile sees the data
#if defined(_MSC_VER)
			_flushall();
#else
			fflush(NULL);
#endif

			// Assert - read file back and compare
			int outSize = 0;
			double* arr = readArrayFromFile(filename, &outSize);
			Assert::IsNotNull(arr);
			Assert::AreEqual(len, outSize);

			for (int i = 0; i < outSize; ++i) {
				Assert::AreEqual(values[i], arr[i], 1e-9, L"Value mismatch at index");
			}

			// Cleanup
			free(arr);
			std::remove(filename);
		}

		TEST_METHOD(TestAddSignal_ExistingFile)
		{
			const char* filename = "addsignal_existing.txt";
			// create an existing file
			{
				std::ofstream ofs(filename);
				ofs << "0\n";
			}

			std::vector<double> values = { 1.0 };
			int len = static_cast<int>(values.size());

			Assert::ExpectException<std::invalid_argument>([&]() {
				addSignal(filename, values.data(), len);
				});

			std::remove(filename);
		}

		TEST_METHOD(TestAddSignal_NullptrName)
		{
			std::vector<double> values = { 1.0 };
			int len = static_cast<int>(values.size());

			Assert::ExpectException<std::invalid_argument>([&]() {
				addSignal(nullptr, values.data(), len);
				});
		}

		TEST_METHOD(TestAddSignal_NullptrValues)
		{
			const char* filename = "addsignal_nullvalues.txt";
			std::remove(filename);

			Assert::ExpectException<std::invalid_argument>([&]() {
				addSignal(filename, nullptr, 1);
				});

			std::remove(filename);
		}

		TEST_METHOD(TestAddSignal_EmptyArray)
		{
			const char* filename = "addsignal_empty.txt";
			std::remove(filename);

			std::vector<double> values; // empty
			Assert::ExpectException<std::invalid_argument>([&]() {
				addSignal(filename, values.data(), 0);
				});

			std::remove(filename);
		}
	};
}
