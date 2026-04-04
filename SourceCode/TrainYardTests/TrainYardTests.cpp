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

	TEST_CLASS(WB_addCar)
	{
	public:

		TEST_METHOD(WB001_AddCarNullTrain)
		{
			// White-box test covering branch: if (t == NULL)
			int result = addCar(NULL, TYPE_ENGINE, 1000.0f);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB002_AddCarMaxCarsReached)
		{
			// White-box test covering branch: if (t->numOfCars >= MAX_CARS)
			train t{};
			// Manually configuring internal state to test the specific capacity boundary condition
			t.numOfCars = MAX_CARS;
			t.numOfEngines = MAX_CARS;
			t.totalWeight = 0.0f;

			int result = addCar(&t, TYPE_ENGINE, 10.0f);
			Assert::AreEqual(0, result);
		}
	};

	TEST_CLASS(BB_removeCar)
	{
	public:

		TEST_METHOD(BB005_RemoveValidCar)
		{
			// Verifies that removing a valid car index succeeds
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_WOOD, 500);

			int result = removeCar(&t, 1);

			Assert::AreEqual(1, result);
		}

		TEST_METHOD(BB006_RemoveInvalidIndexFails)
		{
			// Ensures removal fails when index is out of bounds
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);

			int result = removeCar(&t, 5);

			Assert::AreEqual(0, result);
		}

		TEST_METHOD(BB007_WoodOilAdjacencyBlocked)
		{
			// Ensures removal is blocked if it creates WOOD-OIL adjacency
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_WOOD, 500);
			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_OIL, 500);

			int result = removeCar(&t, 2);

			Assert::AreEqual(0, result);
		}

		TEST_METHOD(BB008_RemoveLastCar)
		{
			// Verifies that removing the last car works correctly
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_WOOD, 500);

			int result = removeCar(&t, 2);

			Assert::AreEqual(1, result);
		}
			
	};

	TEST_CLASS(BB_checktrainsafety)
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

		TEST_METHOD(BB010_OilAfterEngineUnsafe)
		{
			// Ensures that oil directly after engines fails safety rules
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			int result = addCar(&t, TYPE_OIL, 500);

			Assert::AreEqual(0, result);
			Assert::AreEqual(1, t.numOfCars);
		}

		TEST_METHOD(BB011_WoodNextToOilUnsafe)
		{
			// Verifies that WOOD next to OIL fails safety check
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_WOOD, 500);
			int result = addCar(&t, TYPE_OIL, 500);

			Assert::AreEqual(0, result);
			Assert::AreEqual(2, t.numOfCars);
		}

		TEST_METHOD(BB012_ValidTrainSafe)
		{
			// Confirms that a valid train configuration passes safety check
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_WOOD, 500);

			int result = checkTrainSafety(&t);

			Assert::AreEqual(1, result);
		}
	};

	TEST_CLASS(BB_displayTrain)
	{
	public:

		TEST_METHOD(BB013_DisplayEmpty)
		{
			// Ensures display function handles empty train without crashing
			train t = { 0 };

			displayTrain(&t);

			Assert::IsTrue(true);
		}

		TEST_METHOD(BB014_DisplayOneCar)
		{
			// Verifies display works with a single car
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);

			displayTrain(&t);

			Assert::IsTrue(true);
		}

		TEST_METHOD(BB015_DisplayMultipleCars)
		{
			// Ensures display handles multiple cars correctly
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_WOOD, 500);

			displayTrain(&t);

			Assert::IsTrue(true);
		}

		TEST_METHOD(BB016_DisplayMaxCars)
		{
			// Verifies display works when train is at maximum capacity
			train t = { 0 };

			for (int i = 0; i < MAX_CARS; i++) {
				addCar(&t, TYPE_ENGINE, 100);
			}

			displayTrain(&t);

			Assert::IsTrue(true);
		}
			
	};

	TEST_CLASS(WB_removeCar)
	{
	public:
		TEST_METHOD(WB003_RemoveCarNullTrain)
		{
			// White-box test covering branch: if (t == NULL || ...)
			int result = removeCar(NULL, 0);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB004_RemoveCarInvalidIndexNegative)
		{
			// White-box test covering branch: if (... || index < 0 || ...)
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 1000);
			int result = removeCar(&t, -1);
			Assert::AreEqual(0, result);
		}
	};

	TEST_CLASS(WB_checktrainsafety)
	{
	public:
		TEST_METHOD(WB005_CheckSafetyNullTrain)
		{
			// White-box test covering branch: if (t == NULL)
			int result = checkTrainSafety(NULL);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB006_CheckSafetySingleEngineSkipsLoop)
		{
			// White-box test covering branch bypass: for loop condition is bypassed when numOfCars == 1
			train t = { 0 };
			t.numOfCars = 1;
			t.numOfEngines = 1;
			t.cars[0].type = TYPE_ENGINE;
			t.cars[0].weight = 1000;
			int result = checkTrainSafety(&t);
			Assert::AreEqual(1, result);
		}
	};

	TEST_CLASS(WB_displayTrain)
	{
	public:
		TEST_METHOD(WB007_DisplayTrainNull)
		{
			// White-box test covering branch: if (t == NULL)
			// displayTrain should return immediately without crashing or executing rest of code
			displayTrain(NULL);
			Assert::IsTrue(true);
		}

		TEST_METHOD(WB008_DisplayTrainSkipsLoopEmpty)
		{
			// White-box test covering branch: if (t->numOfCars == 0)
			train t = { 0 };
			t.numOfCars = 0;
			// Call should trigger the empty train print and return before the loop execution
			displayTrain(&t);
			Assert::IsTrue(true);
		}
	};
}
