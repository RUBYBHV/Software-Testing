#define _CRT_SECURE_NO_WARNINGS //for anyone using visual studio since I changed scanf_s to scanf
#include <stdio.h>
#include "train_yard.h"

int main(void) {

   char choice = ' ';

    while (choice != 'Q' && choice != 'q') {
        printf("\n[A]dd | [V]iew | [W]eight | [S]afety | [R]emove | [Q]uit: ");
        scanf(" %c", &choice);

        switch (choice) {
        case 'A': case 'a':
            printf(">> Add a car\n");
            break;

        case 'R': case 'r':
            printf(">> Remove a car\n");
            break;

        case 'V': case 'v':
            printf(">> Display Train train cars\n");
            break;

        case 'W': case 'w':
            printf(">> Show train cars and weight\n");
            break;

        case 'S': case 's':
            printf(">> Status: SAFE\n");
            break;
        }
    }
    return 0;
}

int addCar(train* t, int type, float weight) 
{
    if (t == NULL || t->numOfCars >= MAX_CARS) {
        return 0;
    }

    if (t->totalWeight + weight > MAX_WEIGHT) {
        return 0;
    }

    int newNumOfEngines = t->numOfEngines;
    if (type == TYPE_ENGINE) {
        newNumOfEngines++;
    }

    if (t->totalWeight + weight > newNumOfEngines * ENGINE_PULL_CAP) {
        return 0;
    }

    if (type == TYPE_ENGINE) {
        // Cannot add an engine if there are non-engine cars already
        if (t->numOfCars > t->numOfEngines) {
            return 0;
        }
    }

    t->cars[t->numOfCars].type = type;
    t->cars[t->numOfCars].weight = weight;
    t->numOfCars++;
    t->totalWeight += weight;

    if (type == TYPE_ENGINE) {
        t->numOfEngines++;
    }

    return 1;
}

int removeCar(train* t, int index) 
{
    return 0;
}

//placeholder to check if test works
int checkTrainSafety(const train* t)
{
    //if statement that returns a 0
    //if the train lacks an engine
    if (t->numOfEngines == 0) {
        return 0;
    }

    return 1;
}

void displayTrain(const train* t) 
{
    return;
}
