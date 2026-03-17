#ifndef TRAIN_YARD_H
#define TRAIN_YARD_H

#define TYPE_ENGINE 0
#define TYPE_FOOD   1
#define TYPE_WOOD   2
#define TYPE_OIL    3

#define MAX_CARS 99
#define ENGINE_PULL_CAP 5000.0f
#define MAX_WEIGHT   20000.0f

typedef struct {
    int type;
    float weight;
} traincar;

typedef struct {
    traincar cars[MAX_CARS];
    int numOfCars;
    int numOfEngines;
    float totalWeight;
} train;

/*
 Function Name: addCar

 Description:
 This function is meant to add a car to the train while
 checking the order, weight, and safety concerns before
 doing so.

 The function must verify that engines appear only at
 the beginning of the train, system weight does not
 exceed the maximum, and that the engine pulling
 capacity is not exceeded.

 Parameters:

 train* t = pointer to the train struct

 int type = an integer to specify the type of car(engine, food, wood, oil)

 float weight = a floating point number to specify the weight of the car

 Returns:
 1 = car added successfully
 0 = failed to add car
 */
 int addCar(train* t, int type, float weight);

 /*
 Function Name: removeCar

 Description:
 Removes a train car from a chosen position from the train.
 Function will check that removing a car does not
 violate a safety condition before removing a car for
 example a wood car ending up next to an oil car due to
 the previous car being removed.

 Parameters:
 train* t = pointer to the train struct

 int index = position of car to remove from train

 Returns:
 1 = car was removed successfully
 0 = car was not removed
 */
 int removeCar(train* t, int index);

 /*
 Function Name: checkTrainSafety

 Description:
 Function for checking if the train passes all safety concerns.
 The train must have atleast one engine, must not have an
 oil car right after any engine car, and no wood and oil
 cars next to eachother.

 Parameters:
 const train* t = pointer to the train struct

 Returns:
 1 = Train passes the check
 0 = Train does not pass the check
 */
 int checkTrainSafety(const train* t);

 /*
 Function Name: displayTrain

 Description:
 Function displays the current train cars that the train
 has from first to last.
 The function will print the cars positions in the
 train, the car types, and the car weights.

 Parameters:
 const train* t = pointer to the train struct to display
                  the cars

 Returns:
 void = this function only displays the train
        information and does not return a value

 */
 void displayTrain(const train* t);

#endif