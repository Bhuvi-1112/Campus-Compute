
#ifndef IPC_H
#define IPC_H

#define SHM_NAME "/campuscompute_shm"
#define SEM_NAME "/campuscompute_sem"
#define IPC_MESSAGE_SIZE 256

typedef struct {
    char message[IPC_MESSAGE_SIZE];
    int data_ready;
} SharedData;

/* Create and initialize shared memory and semaphore. */
int ipc_init(void);

/* Write a message into shared memory. */
int ipc_write(const char *message);

/* Read a message from shared memory. */
int ipc_read(char *buffer, int size);

/* Release shared memory and semaphore resources. */
void ipc_cleanup(void);

#endif
