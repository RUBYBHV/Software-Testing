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
} train

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

#endif
