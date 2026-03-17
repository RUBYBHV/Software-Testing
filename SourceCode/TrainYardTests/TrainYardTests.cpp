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
		
		TEST_METHOD(BB009_NoEngineIsUnsafe)
		{
			// Creates a train that has two freight cars and no engine
			train t{};

			t.numOfCars = 2;
			t.numOfEngines = 0;
			t.totalWeight = 1000;

			t.cars[0].type = TYPE_FOOD;
			t.cars[0].weight = 500;

			t.cars[1].type = TYPE_WOOD;
			t.cars[1].weight = 500;

			//Runs the safety check function
			int result = checkTrainSafety(&t);

			//Verifies that the function returns 0
			//marking the train as unsafe
			Assert::AreEqual(0, result);
		}
	};
}
