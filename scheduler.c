
#include <stdio.h>
#include <string.h>
#include "scheduler.h"

/* Sort jobs by arrival time.
   Equal arrival times retain their input order. */
static void sort_by_arrival(Job jobs[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (jobs[j].arrival_time > jobs[j + 1].arrival_time) {
                Job temp = jobs[j];
                jobs[j] = jobs[j + 1];
                jobs[j + 1] = temp;
            }
        }
    }
}

/* Display final scheduling statistics. */
static void display_results(const char *name, Job jobs[], int n)
{
    float total_tat = 0;
    float total_wt = 0;

    printf("\n========== %s ==========\n", name);
    printf("%-10s %-8s %-8s %-8s %-8s %-8s\n",
           "Job", "AT", "BT", "CT", "TAT", "WT");

    for (int i = 0; i < n; i++) {
        printf("%-10s %-8d %-8d %-8d %-8d %-8d\n",
               jobs[i].job_id,
               jobs[i].arrival_time,
               jobs[i].burst_time,
               jobs[i].completion_time,
               jobs[i].turnaround_time,
               jobs[i].waiting_time);

        total_tat += jobs[i].turnaround_time;
        total_wt += jobs[i].waiting_time;
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
    printf("Average Waiting Time   : %.2f\n", total_wt / n);
}

/* First Come First Serve scheduling. */
void run_fcfs(const Job input[], int n)
{
    Job jobs[MAX_JOBS];
    memcpy(jobs, input, n * sizeof(Job));

    sort_by_arrival(jobs, n);

    int time = 0;

    printf("\nFCFS Execution Timeline:\n");

    for (int i = 0; i < n; i++) {
        if (time < jobs[i].arrival_time) {
            printf("| IDLE (%d-%d) ",
                   time, jobs[i].arrival_time);
            time = jobs[i].arrival_time;
        }

        int start = time;

        jobs[i].status = JOB_RUNNING;
        time += jobs[i].burst_time;

        printf("| %s (%d-%d) ",
               jobs[i].job_id, start, time);

        jobs[i].completion_time = time;
        jobs[i].turnaround_time =
            time - jobs[i].arrival_time;
        jobs[i].waiting_time =
            jobs[i].turnaround_time - jobs[i].burst_time;

        jobs[i].remaining_time = 0;
        jobs[i].status = JOB_COMPLETED;
    }

    printf("|\n");

    display_results("FCFS Scheduling", jobs, n);
}

/* Round Robin scheduling using a circular ready queue. */
void run_round_robin(const Job input[], int n, int quantum)
{
    Job jobs[MAX_JOBS];
    int queue[MAX_JOBS];

    memcpy(jobs, input, n * sizeof(Job));

    for (int i = 0; i < n; i++) {
        jobs[i].remaining_time = jobs[i].burst_time;
        jobs[i].status = JOB_NEW;
    }

    sort_by_arrival(jobs, n);

    int front = 0;
    int rear = 0;
    int count = 0;
    int next = 0;
    int completed = 0;
    int time = 0;

    printf("\nRound Robin Execution Timeline:\n");

    while (completed < n) {

        /* Add newly arrived jobs to the ready queue. */
        while (next < n && jobs[next].arrival_time <= time) {
            queue[rear] = next;
            rear = (rear + 1) % MAX_JOBS;
            count++;

            jobs[next].status = JOB_READY;
            next++;
        }

        /* CPU remains idle until the next job arrives. */
        if (count == 0) {
            int next_arrival = jobs[next].arrival_time;

            if (time < next_arrival) {
                printf("| IDLE (%d-%d) ", time, next_arrival);
            }

            time = next_arrival;
            continue;
        }

        int index = queue[front];
        front = (front + 1) % MAX_JOBS;
        count--;

        jobs[index].status = JOB_RUNNING;

        int run_time = jobs[index].remaining_time;

        if (run_time > quantum) {
            run_time = quantum;
        }

        int start = time;
        time += run_time;

        jobs[index].remaining_time -= run_time;

        printf("| %s (%d-%d) ",
               jobs[index].job_id, start, time);

        /* Add jobs that arrived during this time slice. */
        while (next < n && jobs[next].arrival_time <= time) {
            queue[rear] = next;
            rear = (rear + 1) % MAX_JOBS;
            count++;

            jobs[next].status = JOB_READY;
            next++;
        }

        if (jobs[index].remaining_time == 0) {
            jobs[index].completion_time = time;

            jobs[index].turnaround_time =
                time - jobs[index].arrival_time;

            jobs[index].waiting_time =
                jobs[index].turnaround_time - jobs[index].burst_time;

            jobs[index].status = JOB_COMPLETED;
            completed++;
        } else {
            jobs[index].status = JOB_READY;

            queue[rear] = index;
            rear = (rear + 1) % MAX_JOBS;
            count++;
        }
    }

    printf("|\n");

    display_results("Round Robin Scheduling", jobs, n);
}

/* Non-preemptive Priority scheduling.
   Smaller priority number means higher priority.
   Ties: earlier arrival first; then input order. */
void run_priority(const Job input[], int n)
{
    Job jobs[MAX_JOBS];
    int completed[MAX_JOBS] = {0};

    memcpy(jobs, input, n * sizeof(Job));

    int time = 0;
    int finished = 0;

    printf("\nPriority Execution Timeline:\n");

    while (finished < n) {
        int selected = -1;

        /* Find the highest-priority arrived job. */
        for (int i = 0; i < n; i++) {
            if (completed[i] ||
                jobs[i].arrival_time > time) {
                continue;
            }

            if (selected == -1 ||
                jobs[i].priority < jobs[selected].priority ||
                (jobs[i].priority == jobs[selected].priority &&
                 jobs[i].arrival_time < jobs[selected].arrival_time)) {
                selected = i;
            }
        }

        /* No job is ready: move to the next arrival. */
        if (selected == -1) {
            int earliest = -1;

            for (int i = 0; i < n; i++) {
                if (!completed[i] &&
                    (earliest == -1 ||
                     jobs[i].arrival_time < jobs[earliest].arrival_time)) {
                    earliest = i;
                }
            }

            printf("| IDLE (%d-%d) ",
                   time, jobs[earliest].arrival_time);

            time = jobs[earliest].arrival_time;
            continue;
        }

        int start = time;

        jobs[selected].status = JOB_RUNNING;
        time += jobs[selected].burst_time;

        printf("| %s (%d-%d) ",
               jobs[selected].job_id, start, time);

        jobs[selected].completion_time = time;

        jobs[selected].turnaround_time =
            time - jobs[selected].arrival_time;

        jobs[selected].waiting_time =
            jobs[selected].turnaround_time - jobs[selected].burst_time;

        jobs[selected].remaining_time = 0;
        jobs[selected].status = JOB_COMPLETED;

        completed[selected] = 1;
        finished++;
    }

    printf("|\n");

    display_results("Priority Scheduling", jobs, n);
}
