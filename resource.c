
#include <stdio.h>
#include <sys/resource.h>
#include "resource.h"

/* Display the current CPU time limit. */
int display_resource_limit(void)
{
    struct rlimit limit;

    if (getrlimit(RLIMIT_CPU, &limit) == -1) {
        perror("getrlimit");
        return -1;
    }

    printf("\nCurrent CPU Resource Limit:\n");

    if (limit.rlim_cur == RLIM_INFINITY)
        printf("Soft limit: Unlimited\n");
    else
        printf("Soft limit: %ld seconds\n",
               (long)limit.rlim_cur);

    if (limit.rlim_max == RLIM_INFINITY)
        printf("Hard limit: Unlimited\n");
    else
        printf("Hard limit: %ld seconds\n",
               (long)limit.rlim_max);

    return 0;
}

/* Set the CPU time limit for the process. */
int set_cpu_limit(int seconds)
{
    if (seconds <= 0) {
        printf("CPU limit must be positive.\n");
        return -1;
    }

    struct rlimit limit;

    if (getrlimit(RLIMIT_CPU, &limit) == -1) {
        perror("getrlimit");
        return -1;
    }

    if ((rlim_t)seconds > limit.rlim_max) {
        printf("Requested limit exceeds the hard limit.\n");
        return -1;
    }

    limit.rlim_cur = (rlim_t)seconds;

    if (setrlimit(RLIMIT_CPU, &limit) == -1) {
        perror("setrlimit");
        return -1;
    }

    printf("CPU time limit set to %d seconds.\n", seconds);

    return display_resource_limit();
}
