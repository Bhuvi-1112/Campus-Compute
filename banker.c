
#include <stdio.h>
#include "banker.h"

/* Banker's Safety Algorithm */
int banker_safety(
    int processes,
    int resources,
    int available[],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int maximum[MAX_PROCESSES][MAX_RESOURCES])
{
    int need[MAX_PROCESSES][MAX_RESOURCES];
    int finish[MAX_PROCESSES] = {0};
    int safe_sequence[MAX_PROCESSES];
    int work[MAX_RESOURCES];

    /* Calculate Need = Maximum - Allocation. */
    printf("\nNeed Matrix:\n");

    for (int i = 0; i < processes; i++) {
        printf("P%d: ", i);

        for (int j = 0; j < resources; j++) {
            need[i][j] = maximum[i][j] - allocation[i][j];

            if (need[i][j] < 0) {
                printf("\nInvalid input: Allocation exceeds Maximum.\n");
                return 0;
            }

            printf("%d ", need[i][j]);
        }

        printf("\n");
    }

    /* Initialize Work = Available. */
    for (int j = 0; j < resources; j++) {
        work[j] = available[j];
    }

    int count = 0;

    /* Find a process whose Need <= Work. */
    while (count < processes) {
        int found = 0;

        for (int i = 0; i < processes; i++) {
            if (finish[i])
                continue;

            int possible = 1;

            for (int j = 0; j < resources; j++) {
                if (need[i][j] > work[j]) {
                    possible = 0;
                    break;
                }
            }

            if (possible) {
                /* Process can finish and release resources. */
                for (int j = 0; j < resources; j++) {
                    work[j] += allocation[i][j];
                }

                safe_sequence[count] = i;
                finish[i] = 1;
                count++;
                found = 1;
            }
        }

        if (!found)
            break;
    }

    /* Display safety result. */
    if (count == processes) {
        printf("\nSystem is in a SAFE state.\n");
        printf("Safe Sequence: ");

        for (int i = 0; i < processes; i++) {
            printf("P%d", safe_sequence[i]);

            if (i < processes - 1)
                printf(" -> ");
        }

        printf("\n");
        return 1;
    }

    printf("\nSystem is in an UNSAFE state.\n");
    printf("No safe sequence exists.\n");

    return 0;
}
