#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "train_yard.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TrainYardTests
{
	TEST_CLASS(TrainYardTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			int a = 2;
			int b = 2;

			Assert::AreEqual(4, a + b);
		}
	};
}
