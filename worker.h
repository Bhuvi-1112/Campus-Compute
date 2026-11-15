
#ifndef WORKER_H
#define WORKER_H

/* Worker execution modes. */
typedef enum {
    WORKER_NORMAL,
    WORKER_SIMULATED_FAILURE
} WorkerMode;

/* Create, monitor and report a worker process. */
int run_worker(WorkerMode mode);

#endif
