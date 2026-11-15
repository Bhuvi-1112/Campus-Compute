
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "worker.h"

/* Create and monitor a worker process. */
int run_worker(WorkerMode mode)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        /* Child process: worker execution. */
        printf("[Worker] Child process started. PID: %d\n",
               getpid());
        fflush(stdout);

        if (mode == WORKER_SIMULATED_FAILURE) {
            printf("[Worker] Simulating controlled failure.\n");
            fflush(stdout);
            _exit(1);
        }

        printf("[Worker] Job completed successfully.\n");
        fflush(stdout);
        _exit(0);
    }

    /* Parent process: wait for the child. */
    printf("[Manager] Waiting for worker PID: %d\n", pid);

    int status;

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);

        if (exit_code == 0) {
            printf("[Manager] Worker completed normally.\n");
            return 0;
        }

        printf("[Manager] Worker failed with exit code %d.\n",
               exit_code);
        return 1;
    }

    if (WIFSIGNALED(status)) {
        printf("[Manager] Worker terminated by signal %d.\n",
               WTERMSIG(status));
        return 1;
    }

    printf("[Manager] Worker ended unexpectedly.\n");
    return 1;
}
