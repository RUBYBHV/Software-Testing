#include "pch.h"
#include "CppUnitTest.h"

extern "C" {
#include "../train_yard.h"
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

		TEST_METHOD(WB009_AddCarWeightExceedsMax)
		{
			// White-box test covering branch: if (t->totalWeight + weight > MAX_WEIGHT)
			train t = { 0 };
			t.totalWeight = MAX_WEIGHT;
			t.numOfEngines = 1;

			int result = addCar(&t, TYPE_WOOD, 10.0f);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB010_AddCarPullCapExceeded)
		{
			// White-box test covering branch: if (t->totalWeight + weight > newNumOfEngines * ENGINE_PULL_CAP)
			train t = { 0 };
			t.numOfEngines = 1;
			t.numOfCars = 1;
			t.cars[0].type = TYPE_ENGINE;
			t.totalWeight = ENGINE_PULL_CAP; // Engine is at capacity

			int result = addCar(&t, TYPE_WOOD, 10.0f);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB011_AddCarEngineAfterFreight)
		{
			// White-box test covering branch: if (type == TYPE_ENGINE) inside numOfCars > numOfEngines
			train t = { 0 };
			t.numOfEngines = 1;
			t.numOfCars = 2; // more cars than engines implies freight exists
			t.totalWeight = 1000.0f;

			int result = addCar(&t, TYPE_ENGINE, 1000.0f);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB012_AddCarWoodOilAdjacent)
		{
			// White-box test covering branch: WOOD next to OIL
			train t = { 0 };
			t.numOfCars = 2;
			t.numOfEngines = 1;
			t.cars[1].type = TYPE_WOOD;
			
			int result = addCar(&t, TYPE_OIL, 500.0f);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB013_AddCarOilRightAfterEngine)
		{
			// White-box test covering branch: OIL directly after engines
			train t = { 0 };
			t.numOfEngines = 1;
			t.numOfCars = 1;
			t.cars[0].type = TYPE_ENGINE;

			int result = addCar(&t, TYPE_OIL, 1000.0f);
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

		TEST_METHOD(WB014_RemoveCarIndexTooHigh)
		{
			// White-box test covering branch: if (... || index >= t->numOfCars)
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 1000);
			int result = removeCar(&t, 1);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB015_RemoveCarWoodOilAdjacency)
		{
			// White-box test covering branch for WOOD-OIL adjacency caused by removal
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_WOOD, 500);
			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_OIL, 500);
			int result = removeCar(&t, 2);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB016_RemoveEngine)
		{
			// White-box test covering branch: if (t->cars[index].type == TYPE_ENGINE)
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_WOOD, 500);
			int result = removeCar(&t, 0);
			Assert::AreEqual(1, result);
			Assert::AreEqual(0, t.numOfEngines);
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

		TEST_METHOD(WB017_CheckSafetyNoEngines)
		{
			// White-box test covering branch: if (t->numOfEngines == 0)
			train t = { 0 };
			t.numOfEngines = 0;
			int result = checkTrainSafety(&t);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB018_CheckSafetyOilFirstFreight)
		{
			// White-box test covering branch: if (t->cars[t->numOfEngines].type == TYPE_OIL)
			train t = { 0 };
			t.numOfCars = 2;
			t.numOfEngines = 1;
			t.cars[0].type = TYPE_ENGINE;
			t.cars[1].type = TYPE_OIL;
			int result = checkTrainSafety(&t);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(WB019_CheckSafetyWoodOilAdjacent)
		{
			// White-box test covering loop branch matching WOOD/OIL adjacency
			train t = { 0 };
			t.numOfCars = 3;
			t.numOfEngines = 1;
			t.cars[0].type = TYPE_ENGINE;
			t.cars[1].type = TYPE_WOOD;
			t.cars[2].type = TYPE_OIL;
			int result = checkTrainSafety(&t);
			Assert::AreEqual(0, result);
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

		TEST_METHOD(WB020_DisplayTrainIterateCars)
		{
			// White-box test covering branch loop iteration: for (int i = 0; i < t->numOfCars; i++)
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_WOOD, 500);
			displayTrain(&t);
			Assert::IsTrue(true);
		}
	};

	TEST_CLASS(IntegrationTests) 
	{
	public:
		TEST_METHOD(IT001_AddAndSafetyValidation) 
		{
			// Verifies that adding an invalid OIL car after engine is blocked and train remains safe
			train t = { 0 };

			int r1 = addCar(&t, TYPE_ENGINE, 1000);
			int r2 = addCar(&t, TYPE_OIL, 500);

			int safe = checkTrainSafety(&t);

			Assert::AreEqual(1, r1);
			Assert::AreEqual(0, r2);
			Assert::AreEqual(1, safe);
		}

		TEST_METHOD(IT002_AddRemoveSafetyFlow)
		{
			// Ensures that adding and removing cars maintains a valid and safe train configuration
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_WOOD, 500);
			addCar(&t, TYPE_FOOD, 500);

			int r = removeCar(&t, 2);

			int safe = checkTrainSafety(&t);

			Assert::AreEqual(1, r);
			Assert::AreEqual(1, safe);
		}

		TEST_METHOD(IT003_EngineCapacityIntegration)
		{
			// Verifies that engine pull capacity is enforced when adding freight cars
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 2000);
			addCar(&t, TYPE_ENGINE, 2000);

			int r = addCar(&t, TYPE_WOOD, 7000);

			Assert::AreEqual(0, r);
		}

		TEST_METHOD(IT004_FullTrainWorkflow)
		{
			// Simulates a full workflow of adding and removing cars while enforcing safety constraints
			train t = { 0 };

			addCar(&t, TYPE_ENGINE, 1000);
			addCar(&t, TYPE_ENGINE, 1000);

			addCar(&t, TYPE_FOOD, 500);
			addCar(&t, TYPE_WOOD, 500);

			int r1 = removeCar(&t, 2);
			int r2 = addCar(&t, TYPE_OIL, 500);

			int safe = checkTrainSafety(&t);

			Assert::AreEqual(1, r1);
			Assert::AreEqual(0, r2);
			Assert::AreEqual(1, safe);
		}
	};

	TEST_CLASS(AcceptanceTests) 
	{
	public:
		TEST_METHOD(AT001_REQ1_AddCarEngineFirst)
		{
			// Requirement: Engines appear only at the beginning
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 2000.0f);
			addCar(&t, TYPE_WOOD, 1000.0f);
			int r = addCar(&t, TYPE_ENGINE, 2000.0f); // Engine after freight
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT002_REQ2_AddCarMaxWeight)
		{
			// Requirement: System weight does not exceed the maximum (20000.0f)
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_ENGINE, 4000.0f);
			addCar(&t, TYPE_ENGINE, 4000.0f); // 16000 kg total
			int r = addCar(&t, TYPE_WOOD, 5000.0f); // Would be 21000
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT003_REQ3_AddCarPullingCap)
		{
			// Requirement: Engine pulling capacity (5000 per engine) not exceeded
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 1000.0f);
			int r = addCar(&t, TYPE_WOOD, 4500.0f); // 5500 total, exceeds 5000 
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT004_REQ4_RemoveCarSafely)
		{
			// Requirement: Function checks that removing a car does not violate safety conditions
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 2000.0f);
			addCar(&t, TYPE_WOOD, 1000.0f);
			addCar(&t, TYPE_FOOD, 1000.0f);
			addCar(&t, TYPE_OIL, 1000.0f);
			int r = removeCar(&t, 2); // Removing FOOD makes WOOD and OIL adjacent
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT005_REQ5_CheckSafetyHasEngine)
		{
			// Requirement: Train must have at least one engine
			train t = { 0 };
			t.numOfEngines = 0;
			t.numOfCars = 1;
			t.cars[0].type = TYPE_WOOD;
			int r = checkTrainSafety(&t);
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT006_REQ6_CheckSafetyNoOilAfterEngine)
		{
			// Requirement: Must not have an oil car right after any engine car
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 2000.0f);
			int r = addCar(&t, TYPE_OIL, 1000.0f);
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT007_REQ7_CheckSafetyNoWoodOilAdj)
		{
			// Requirement: No wood and oil cars next to each other
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 2000.0f);
			addCar(&t, TYPE_WOOD, 1000.0f);
			int r = addCar(&t, TYPE_OIL, 1000.0f);
			Assert::AreEqual(0, r);
		}

		TEST_METHOD(AT008_REQ8_DisplayTrainOutput)
		{
			// Requirement: Function displays current train cars
			train t = { 0 };
			addCar(&t, TYPE_ENGINE, 2000.0f);
			addCar(&t, TYPE_WOOD, 1000.0f);
			displayTrain(&t); // Execute to ensure no crashes
			Assert::IsTrue(true);
		}
	};
}
