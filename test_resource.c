
#include <stdio.h>
#include "resource.h"

int main(void)
{
    int choice;
    int seconds;

    printf("\n====================================\n");
    printf("   CAMPUSCOMPUTE RESOURCE TEST\n");
    printf("====================================\n");

    printf("1. Display current CPU limit\n");
    printf("2. Set CPU time limit\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return 1;
    }

    switch (choice) {
        case 1:
            display_resource_limit();
            break;

        case 2:
            printf("Enter CPU time limit in seconds: ");

            if (scanf("%d", &seconds) != 1 || seconds <= 0) {
                printf("Invalid CPU time limit.\n");
                return 1;
            }

            set_cpu_limit(seconds);
            break;

        case 3:
            printf("Exiting resource test.\n");
            break;

        default:
            printf("Invalid choice.\n");
            return 1;
    }

    return 0;
}
