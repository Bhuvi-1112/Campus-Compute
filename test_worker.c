
#include <stdio.h>
#include "worker.h"

int main(void)
{
    int choice;

    printf("\n====================================\n");
    printf("    CAMPUSCOMPUTE WORKER TEST\n");
    printf("====================================\n");

    printf("1. Run normal worker\n");
    printf("2. Simulate worker failure\n");
    printf("3. Run both tests\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    switch (choice) {
        case 1:
            run_worker(WORKER_NORMAL);
            break;

        case 2:
            run_worker(WORKER_SIMULATED_FAILURE);
            break;

        case 3:
            run_worker(WORKER_NORMAL);
            run_worker(WORKER_SIMULATED_FAILURE);
            break;

        default:
            printf("Invalid choice. Select 1, 2 or 3.\n");
            return 1;
    }

    return 0;
}
