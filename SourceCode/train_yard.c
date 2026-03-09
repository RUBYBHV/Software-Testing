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
