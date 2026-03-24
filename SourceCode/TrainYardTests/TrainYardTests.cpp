#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "train_yard.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TrainYardTests
{
	TEST_CLASS(BB_addCar)
	{
	public:
		
		TEST_METHOD(BB001_AddEngineSuccess)
		{
			train t{};
			int result = addCar(&t, TYPE_ENGINE, 4000.0f);
			Assert::AreEqual(1, t.numOfEngines);
			Assert::AreEqual(1, t.numOfCars);
			Assert::AreEqual(4000.0f, t.totalWeight);
		}

		TEST_METHOD(BB002_AddEngineAfterFreightViolatesLeadRule)
		{
			train t{};
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_FOOD, 500.0f);
			int result = addCar(&t, TYPE_ENGINE, 3000.0f);
			Assert::AreEqual(0, result);
			Assert::AreEqual(1, t.numOfEngines);
			Assert::AreEqual(2, t.numOfCars);
		}

		TEST_METHOD(BB003_AddCarExceedsPullCapacity)
		{
			train t{};
			addCar(&t, TYPE_ENGINE, 1000.0f);
			// Engine pull capacity is 5000kg. Current weight = 1000kg.
			// Adding 4500kg would make total weight 5500kg > 5000kg.
			int result = addCar(&t, TYPE_WOOD, 4500.0f);
			Assert::AreEqual(0, result);
			Assert::AreEqual(1, t.numOfCars); // Only the engine was added successfully
		}

		TEST_METHOD(BB004_AddCarExceedsSystemWeight)
		{
			train t{};
			// Adds 4 engines, total pull capacity = 20000kg
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_ENGINE, 4000.0f);
			// Current weight is 16000kg. 
			// System limit is 20000kg.
			// Attempt to add a 5000kg car -> Total 21000kg > 20000kg -> should fail.
			int result = addCar(&t, TYPE_OIL, 5000.0f);
			Assert::AreEqual(0, result);
			Assert::AreEqual(4, t.numOfCars);
		}
	};

	TEST_CLASS(BB_removeCar)
	{
	public:

		TEST_METHOD(BB005_NoEngineIsUnsafe)
		{
			Assert::AreEqual(1, 1);
		}
			
	};

	TEST_CLASS(BB_safetycheck)
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

	TEST_CLASS(BB_displayTrain)
	{
	public:

		TEST_METHOD(BB013_displayempty)
		{
			Assert::AreEqual(1, 1);
		}
			
	};
}
