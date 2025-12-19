#include "pch.h"
#include "../SygnalAnalyzer/InputOutput.cpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Testing
{
	TEST_CLASS(Testing)
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
	};
}
