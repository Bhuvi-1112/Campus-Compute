
#include <stdio.h>
#include "banker.h"

int main(void)
{
    int processes, resources;

    int available[MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int maximum[MAX_PROCESSES][MAX_RESOURCES];

    printf("\n====================================\n");
    printf("    CAMPUSCOMPUTE BANKER'S TEST\n");
    printf("====================================\n");

    printf("\nEnter number of processes (1-%d): ",
           MAX_PROCESSES);

    if (scanf("%d", &processes) != 1 ||
        processes < 1 || processes > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    printf("Enter number of resources (1-%d): ",
           MAX_RESOURCES);

    if (scanf("%d", &resources) != 1 ||
        resources < 1 || resources > MAX_RESOURCES) {
        printf("Invalid number of resources.\n");
        return 1;
    }

    printf("\nEnter Available resources:\n");

    for (int j = 0; j < resources; j++) {
        printf("Resource %d: ", j);

        if (scanf("%d", &available[j]) != 1 ||
            available[j] < 0) {
            printf("Invalid available resource value.\n");
            return 1;
        }
    }

    printf("\nEnter Allocation Matrix:\n");

    for (int i = 0; i < processes; i++) {
        printf("Process P%d:\n", i);

        for (int j = 0; j < resources; j++) {
            printf("Resource %d: ", j);

            if (scanf("%d", &allocation[i][j]) != 1 ||
                allocation[i][j] < 0) {
                printf("Invalid allocation value.\n");
                return 1;
            }
        }
    }

    printf("\nEnter Maximum Matrix:\n");

    for (int i = 0; i < processes; i++) {
        printf("Process P%d:\n", i);

        for (int j = 0; j < resources; j++) {
            printf("Resource %d: ", j);

            if (scanf("%d", &maximum[i][j]) != 1 ||
                maximum[i][j] < 0 ||
                maximum[i][j] < allocation[i][j]) {
                printf("Invalid maximum value.\n");
                return 1;
            }
        }
    }

    printf("\n========== BANKER'S SAFETY CHECK ==========\n");

    banker_safety(processes, resources,
                  available, allocation, maximum);

    return 0;
}
