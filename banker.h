
#ifndef BANKER_H
#define BANKER_H

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

/* Check whether the system is in a safe state. */
int banker_safety(
    int processes,
    int resources,
    int available[],
    int allocation[MAX_PROCESSES][MAX_RESOURCES],
    int maximum[MAX_PROCESSES][MAX_RESOURCES]
);

#endif
