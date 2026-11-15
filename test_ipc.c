
#include <stdio.h>
#include "ipc.h"

int main(void)
{
    char buffer[IPC_MESSAGE_SIZE];

    printf("\n====================================\n");
    printf("    CAMPUSCOMPUTE IPC TEST\n");
    printf("====================================\n");

    /* Initialize shared memory and semaphore. */
    if (ipc_init() != 0) {
        fprintf(stderr, "IPC initialization failed.\n");
        return 1;
    }

    printf("Shared memory initialized successfully.\n");
    printf("Named semaphore initialized successfully.\n");

    /* Write a message into shared memory. */
    const char *message =
        "CampusCompute job data transferred using shared memory.";

    if (ipc_write(message) != 0) {
        fprintf(stderr, "IPC write failed.\n");
        ipc_cleanup();
        return 1;
    }

    printf("\nMessage written to shared memory.\n");

    /* Read the message back. */
    int result = ipc_read(buffer, sizeof(buffer));

    if (result == -1) {
        fprintf(stderr, "IPC read failed.\n");
        ipc_cleanup();
        return 1;
    }

    if (result == 0) {
        printf("No message available.\n");
    } else {
        printf("Message received: %s\n", buffer);
    }

    /* Verify that the message was consumed. */
    result = ipc_read(buffer, sizeof(buffer));

    if (result == 0) {
        printf("Shared memory message successfully consumed.\n");
    } else {
        printf("Unexpected additional message.\n");
    }

    /* Clean up shared memory and semaphore. */
    ipc_cleanup();

    printf("\nShared memory and semaphore cleaned up.\n");
    printf("IPC test completed successfully.\n");

    return 0;
}
