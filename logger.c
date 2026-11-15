
#include <stdio.h>
#include <string.h>
#include "logger.h"

/* Initialize queue and synchronization objects. */
int logger_init(LoggerQueue *queue)
{
    queue->front = 0;
    queue->rear = 0;
    queue->count = 0;
    queue->producer_done = 0;

    if (pthread_mutex_init(&queue->mutex, NULL) != 0)
        return -1;

    if (pthread_cond_init(&queue->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&queue->mutex);
        return -1;
    }

    if (pthread_cond_init(&queue->not_full, NULL) != 0) {
        pthread_cond_destroy(&queue->not_empty);
        pthread_mutex_destroy(&queue->mutex);
        return -1;
    }

    return 0;
}

/* Producer adds a message to the bounded queue. */
int logger_produce(LoggerQueue *queue, const char *message)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->count == LOG_QUEUE_SIZE &&
           !queue->producer_done) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    if (queue->producer_done) {
        pthread_mutex_unlock(&queue->mutex);
        return -1;
    }

    snprintf(queue->messages[queue->rear],
             LOG_MESSAGE_SIZE, "%s", message);

    queue->rear = (queue->rear + 1) % LOG_QUEUE_SIZE;
    queue->count++;

    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);

    return 0;
}

/* Consumer thread removes and displays messages. */
void *logger_consumer(void *arg)
{
    LoggerQueue *queue = (LoggerQueue *)arg;

    while (1) {
        pthread_mutex_lock(&queue->mutex);

        while (queue->count == 0 && !queue->producer_done) {
            pthread_cond_wait(&queue->not_empty, &queue->mutex);
        }

        if (queue->count == 0 && queue->producer_done) {
            pthread_mutex_unlock(&queue->mutex);
            break;
        }

        char message[LOG_MESSAGE_SIZE];

        snprintf(message, LOG_MESSAGE_SIZE, "%s",
                 queue->messages[queue->front]);

        queue->front = (queue->front + 1) % LOG_QUEUE_SIZE;
        queue->count--;

        pthread_cond_signal(&queue->not_full);
        pthread_mutex_unlock(&queue->mutex);

        printf("[LOGGER] %s\n", message);
    }

    printf("[LOGGER] Consumer thread terminated.\n");

    return NULL;
}

/* Notify consumer that production has finished. */
void logger_finish(LoggerQueue *queue)
{
    pthread_mutex_lock(&queue->mutex);

    queue->producer_done = 1;

    pthread_cond_broadcast(&queue->not_empty);
    pthread_cond_broadcast(&queue->not_full);

    pthread_mutex_unlock(&queue->mutex);
}

/* Destroy synchronization objects. */
void logger_destroy(LoggerQueue *queue)
{
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
    pthread_mutex_destroy(&queue->mutex);
}
