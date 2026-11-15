
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "scheduler.h"
#include "logger.h"
#include "ipc.h"
#include "banker.h"
#include "worker.h"
#include "resource.h"

static void cpu_scheduling(void)
{
    Job jobs[MAX_JOBS];
    int n, quantum;

    printf("\n--- CPU SCHEDULING ---\n");
    printf("Enter number of jobs (1-%d): ", MAX_JOBS);

    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_JOBS) {
        printf("Invalid number of jobs.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        printf("\nJob %d\n", i + 1);

        printf("Job ID: ");
        if (scanf("%15s", jobs[i].job_id) != 1)
            return;

        printf("Arrival Time: ");
        if (scanf("%d", &jobs[i].arrival_time) != 1 ||
            jobs[i].arrival_time < 0) {
            printf("Invalid arrival time.\n");
            return;
        }

        printf("Burst Time: ");
        if (scanf("%d", &jobs[i].burst_time) != 1 ||
            jobs[i].burst_time <= 0) {
            printf("Invalid burst time.\n");
            return;
        }

        printf("Priority: ");
        if (scanf("%d", &jobs[i].priority) != 1 ||
            jobs[i].priority < 0) {
            printf("Invalid priority.\n");
            return;
        }

        jobs[i].remaining_time = jobs[i].burst_time;
        jobs[i].status = JOB_NEW;
    }

    printf("Round Robin Time Quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum <= 0) {
        printf("Invalid quantum.\n");
        return;
    }

    printf("\n========== CPU SCHEDULING ==========\n");
    run_fcfs(jobs, n);
    run_round_robin(jobs, n, quantum);
    run_priority(jobs, n);
}

static void logger_demo(void)
{
    LoggerQueue queue;
    pthread_t consumer;

    if (logger_init(&queue) != 0) {
        printf("Logger initialization failed.\n");
        return;
    }

    if (pthread_create(&consumer, NULL, logger_consumer,
                       &queue) != 0) {
        printf("Consumer thread creation failed.\n");
        logger_destroy(&queue);
        return;
    }

    for (int i = 1; i <= 5; i++) {
        char message[LOG_MESSAGE_SIZE];

        snprintf(message, sizeof(message),
                 "CampusCompute event %d", i);

        if (logger_produce(&queue, message) != 0)
            printf("Unable to produce event.\n");
    }

    logger_finish(&queue);
    pthread_join(consumer, NULL);
    logger_destroy(&queue);

    printf("Logger demonstration completed.\n");
}

static void ipc_demo(void)
{
    char buffer[IPC_MESSAGE_SIZE];

    if (ipc_init() != 0) {
        printf("IPC initialization failed.\n");
        return;
    }

    printf("Shared memory and semaphore initialized.\n");

    if (ipc_write("CampusCompute job information") != 0) {
        printf("IPC write failed.\n");
        ipc_cleanup();
        return;
    }

    printf("Message written successfully.\n");

    int result = ipc_read(buffer, sizeof(buffer));

    if (result == 1)
        printf("Received message: %s\n", buffer);
    else
        printf("IPC message could not be read.\n");

    ipc_cleanup();
    printf("IPC resources cleaned up.\n");
}

static void banker_demo(void)
{
    int p, r;
    int available[MAX_RESOURCES];
    int allocation[MAX_PROCESSES][MAX_RESOURCES] = {{0}};
    int maximum[MAX_PROCESSES][MAX_RESOURCES] = {{0}};

    printf("\nNumber of processes (1-%d): ", MAX_PROCESSES);
    if (scanf("%d", &p) != 1 || p < 1 || p > MAX_PROCESSES) {
        printf("Invalid process count.\n");
        return;
    }

    printf("Number of resources (1-%d): ", MAX_RESOURCES);
    if (scanf("%d", &r) != 1 || r < 1 || r > MAX_RESOURCES) {
        printf("Invalid resource count.\n");
        return;
    }

    printf("\nEnter Available resources:\n");
    for (int j = 0; j < r; j++) {
        printf("Resource %d: ", j);
        if (scanf("%d", &available[j]) != 1 || available[j] < 0) {
            printf("Invalid resource value.\n");
            return;
        }
    }

    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < p; i++) {
        printf("Process P%d:\n", i);
        for (int j = 0; j < r; j++) {
            printf("Resource %d: ", j);
            if (scanf("%d", &allocation[i][j]) != 1 ||
                allocation[i][j] < 0) {
                printf("Invalid allocation.\n");
                return;
            }
        }
    }

    printf("\nEnter Maximum Matrix:\n");
    for (int i = 0; i < p; i++) {
        printf("Process P%d:\n", i);
        for (int j = 0; j < r; j++) {
            printf("Resource %d: ", j);
            if (scanf("%d", &maximum[i][j]) != 1 ||
                maximum[i][j] < allocation[i][j]) {
                printf("Invalid maximum value.\n");
                return;
            }
        }
    }

    banker_safety(p, r, available, allocation, maximum);
}

static void worker_demo(void)
{
    int choice;

    printf("\n1. Normal worker\n");
    printf("2. Simulated worker failure\n");
    printf("3. Run both\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return;
    }

    switch (choice) {
        case 1:
            run_worker(WORKER_NORMAL);
            break;
        case 2:
            run_worker(WORKER_SIMULATED_FAILURE);
            break;
        case 3:
            run_worker(WORKER_NORMAL);
            run_worker(WORKER_SIMULATED_FAILURE);
            break;
        default:
            printf("Invalid choice.\n");
    }
}

static void resource_demo(void)
{
    int choice, seconds;

    printf("\n1. Display CPU limit\n");
    printf("2. Set CPU limit\n");
    printf("Enter choice: ");

    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        return;
    }

    if (choice == 1) {
        display_resource_limit();
    } else if (choice == 2) {
        printf("Enter CPU time limit in seconds: ");

        if (scanf("%d", &seconds) != 1 || seconds <= 0) {
            printf("Invalid CPU limit.\n");
            return;
        }

        set_cpu_limit(seconds);
    } else {
        printf("Invalid choice.\n");
    }
}

int main(void)
{
    int choice;

    do {
        printf("\n========================================\n");
        printf("          CAMPUSCOMPUTE SYSTEM\n");
        printf(" Intelligent Job Scheduling & Resource Management\n");
        printf("========================================\n");

        printf("1. CPU Scheduling\n");
        printf("2. Concurrent Activity Logger\n");
        printf("3. Shared Memory IPC\n");
        printf("4. Banker's Safety Algorithm\n");
        printf("5. Worker Process Management\n");
        printf("6. Resource Limits\n");
        printf("7. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Enter a number from 1 to 7.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                cpu_scheduling();
                break;
            case 2:
                logger_demo();
                break;
            case 3:
                ipc_demo();
                break;
            case 4:
                banker_demo();
                break;
            case 5:
                worker_demo();
                break;
            case 6:
                resource_demo();
                break;
            case 7:
                printf("Exiting CampusCompute. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Select 1 to 7.\n");
        }

    } while (choice != 7);

    return 0;
}
