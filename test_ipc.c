
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "ipc.h"

int main(void)
{
    char buffer[IPC_MESSAGE_SIZE];

    printf("\n====================================\n");
    printf("       CAMPUSCOMPUTE IPC TEST\n");
    printf("====================================\n");

    /* Initialize shared memory and named semaphore */
    if (ipc_init() != 0) {
        printf("IPC initialization failed.\n");
        return 1;
    }

    printf("Shared memory initialized successfully.\n");
    printf("Named semaphore initialized successfully.\n");

    fflush(stdout);

    /* Create child process */
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        ipc_cleanup();
        return 1;
    }

    /* Child process writes the message */
    if (pid == 0) {
        const char *message =
            "Message sent from child process to parent process.";

        if (ipc_write(message) != 0) {
            _exit(1);
        }

        _exit(0);
    }

    /* Parent waits for child process */
    int status;

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        ipc_cleanup();
        return 1;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        printf("Child process failed.\n");
        ipc_cleanup();
        return 1;
    }

    printf("Child process completed successfully.\n");

    /* Parent reads the message from shared memory */
    int result = ipc_read(buffer, sizeof(buffer));

    if (result == -1) {
        printf("IPC read failed.\n");
        ipc_cleanup();
        return 1;
    }

    if (result == 0) {
        printf("No message available.\n");
    } else {
        printf("Parent received message: %s\n", buffer);
    }

    /* Clean up IPC resources */
    ipc_cleanup();

    printf("Shared memory and semaphore cleaned up.\n");
    printf("IPC test completed successfully.\n");

    return 0;
}
