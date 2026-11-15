
#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>

#define LOG_QUEUE_SIZE 10
#define LOG_MESSAGE_SIZE 256

typedef struct {
    char messages[LOG_QUEUE_SIZE][LOG_MESSAGE_SIZE];

    int front;
    int rear;
    int count;

    int producer_done;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

} LoggerQueue;

/* Initialize the logger queue and synchronization objects. */
int logger_init(LoggerQueue *queue);

/* Add an event to the bounded queue. */
int logger_produce(LoggerQueue *queue, const char *message);

/* Consumer thread function. */
void *logger_consumer(void *arg);

/* Signal that no more messages will be produced. */
void logger_finish(LoggerQueue *queue);

/* Destroy synchronization objects. */
void logger_destroy(LoggerQueue *queue);

#endif
