#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_JOBS 100

typedef enum {
    JOB_NEW,
    JOB_READY,
    JOB_RUNNING,
    JOB_COMPLETED
} JobStatus;

typedef struct {
    char job_id[16];

    int arrival_time;
    int burst_time;
    int priority;

    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;

    JobStatus status;
} Job;

/* CPU scheduling algorithms */

void run_fcfs(const Job jobs[], int n);

void run_round_robin(const Job jobs[], int n, int quantum);

void run_priority(const Job jobs[], int n);

#endif
