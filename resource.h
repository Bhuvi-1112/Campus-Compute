
#ifndef RESOURCE_H
#define RESOURCE_H

#include <sys/resource.h>

/* Display current CPU resource limit. */
int display_resource_limit(void);

/* Set a CPU time limit for the process. */
int set_cpu_limit(int seconds);

#endif
