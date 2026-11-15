
#include <stdio.h>
#include <pthread.h>
#include "logger.h"

int main(void)
{
    LoggerQueue queue;
    pthread_t consumer_thread;

    printf("\n====================================\n");
    printf("   CAMPUSCOMPUTE LOGGER TEST\n");
    printf("====================================\n");

    /* Initialize the bounded queue. */
    if (logger_init(&queue) != 0) {
        fprintf(stderr, "Logger initialization failed.\n");
        return 1;
    }

    /* Start the consumer thread. */
    if (pthread_create(&consumer_thread, NULL,
                       logger_consumer, &queue) != 0) {
        fprintf(stderr, "Consumer thread creation failed.\n");
        logger_destroy(&queue);
        return 1;
    }

    /* Main thread acts as the producer. */
    for (int i = 1; i <= 15; i++) {
        char message[LOG_MESSAGE_SIZE];

        snprintf(message, sizeof(message),
                 "Job event %d: Job processed successfully", i);

        if (logger_produce(&queue, message) != 0) {
            fprintf(stderr, "Failed to produce log message.\n");
            logger_finish(&queue);
            pthread_join(consumer_thread, NULL);
            logger_destroy(&queue);
            return 1;
        }
    }

    /* Tell consumer that no more messages will arrive. */
    logger_finish(&queue);

    /* Wait for consumer to finish processing all messages. */
    pthread_join(consumer_thread, NULL);

    printf("\nAll log messages processed successfully.\n");
    printf("Producer and consumer threads terminated.\n");

    /* Release synchronization resources. */
    logger_destroy(&queue);

    printf("Logger resources cleaned up.\n");

    return 0;
}
