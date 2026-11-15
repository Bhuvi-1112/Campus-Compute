
#include <stdio.h>
#include "scheduler.h"

int main(void)
{
    Job jobs[MAX_JOBS];
    int n, quantum;

    printf("\n====================================\n");
    printf("     CAMPUSCOMPUTE JOB SCHEDULER\n");
    printf("====================================\n");

    printf("\nEnter number of jobs (1-%d): ", MAX_JOBS);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_JOBS) {
        printf("Error: Invalid number of jobs.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        printf("\n---------- Job %d ----------\n", i + 1);

        printf("Enter Job ID: ");
        if (scanf("%15s", jobs[i].job_id) != 1) {
            printf("Error: Invalid Job ID.\n");
            return 1;
        }

        printf("Enter Arrival Time: ");
        if (scanf("%d", &jobs[i].arrival_time) != 1 ||
            jobs[i].arrival_time < 0) {
            printf("Error: Arrival time cannot be negative.\n");
            return 1;
        }

        printf("Enter Burst Time: ");
        if (scanf("%d", &jobs[i].burst_time) != 1 ||
            jobs[i].burst_time <= 0) {
            printf("Error: Burst time must be positive.\n");
            return 1;
        }

        printf("Enter Priority (smaller number = higher priority): ");
        if (scanf("%d", &jobs[i].priority) != 1 ||
            jobs[i].priority < 0) {
            printf("Error: Priority must be non-negative.\n");
            return 1;
        }

        jobs[i].remaining_time = jobs[i].burst_time;
        jobs[i].completion_time = 0;
        jobs[i].turnaround_time = 0;
        jobs[i].waiting_time = 0;
        jobs[i].status = JOB_NEW;
    }

    printf("\nEnter Round Robin Time Quantum: ");

    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Error: Time quantum must be positive.\n");
        return 1;
    }

    printf("\n\n========== SCHEDULING RESULTS ==========\n");

    run_fcfs(jobs, n);

    run_round_robin(jobs, n, quantum);

    run_priority(jobs, n);

    printf("\nAll scheduling algorithms executed successfully.\n");

    return 0;
}
