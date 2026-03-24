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
    // Checks that train exists and index is valid
    if (t == NULL || index < 0 || index >= t->numOfCars) {
        return 0;
    }

    // Stops WOOD car being next to OIL after removal
    if (index > 0 && index < t->numOfCars - 1) {
        int left = t->cars[index - 1].type;
        int right = t->cars[index + 1].type;

        if ((left == TYPE_WOOD && right == TYPE_OIL) ||
            (left == TYPE_OIL && right == TYPE_WOOD)) {
            return 0;
        }
    }

    // Update engine count if removing an engine
    if (t->cars[index].type == TYPE_ENGINE) {
        t->numOfEngines--;
    }

    // Update total weight
    t->totalWeight -= t->cars[index].weight;

    // Shift cars left to fill gap
    for (int i = index; i < t->numOfCars - 1; i++) {
        t->cars[i] = t->cars[i + 1];
    }

    // Reduce car count
    t->numOfCars--;

    return 1;
}

int checkTrainSafety(const train* t)
{
    // Validate input
    if (t == NULL) {
        return 0;
    }

    // Must have at least one engine
    if (t->numOfEngines == 0) {
        return 0;
    }

    // First freight car cannot be OIL
    if (t->numOfCars > t->numOfEngines) {
        if (t->cars[t->numOfEngines].type == TYPE_OIL) {
            return 0;
        }
    }

    // Ensure WOOD and OIL are not adjacent
    for (int i = 0; i < t->numOfCars - 1; i++) {
        int current = t->cars[i].type;
        int next = t->cars[i + 1].type;

        if ((current == TYPE_WOOD && next == TYPE_OIL) ||
            (current == TYPE_OIL && next == TYPE_WOOD)) {
            return 0;
        }
    }

    return 1;
}

void displayTrain(const train* t) 
{
    // Validate input
    if (t == NULL) {
        return;
    }

    // Handle empty train
    if (t->numOfCars == 0) {
        printf("Train is empty.\n");
        return;
    }

    // Print each car in order
    for (int i = 0; i < t->numOfCars; i++) {
        printf("Car %d - Type: %d, Weight: %.1f\n",
            i,
            t->cars[i].type,
            t->cars[i].weight);
    }

    // Print total weight
    printf("Total Weight: %.1f\n", t->totalWeight);
}
