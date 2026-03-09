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

#endif
