#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <semaphore.h>
#include "ipc.h"

static int shm_fd = -1;
static SharedData *shared_data = NULL;
static sem_t *ipc_sem = SEM_FAILED;

/* Create and initialize shared memory and semaphore. */
int ipc_init(void)
{
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (shm_fd == -1) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(shm_fd, sizeof(SharedData)) == -1) {
        perror("ftruncate");
        ipc_cleanup();
        return -1;
    }

    shared_data = mmap(NULL, sizeof(SharedData),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED, shm_fd, 0);

    if (shared_data == MAP_FAILED) {
        perror("mmap");
        shared_data = NULL;
        ipc_cleanup();
        return -1;
    }

    shared_data->message[0] = '\0';
    shared_data->data_ready = 0;

    ipc_sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0600, 1);

    if (ipc_sem == SEM_FAILED) {
        perror("sem_open");
        ipc_cleanup();
        return -1;
    }

    return 0;
}

/* Write a message into shared memory. */
int ipc_write(const char *message)
{
    if (shared_data == NULL || ipc_sem == SEM_FAILED ||
        message == NULL) {
        return -1;
    }

    if (sem_wait(ipc_sem) == -1) {
        perror("sem_wait");
        return -1;
    }

    snprintf(shared_data->message,
             IPC_MESSAGE_SIZE, "%s", message);

    shared_data->data_ready = 1;

    if (sem_post(ipc_sem) == -1) {
        perror("sem_post");
        return -1;
    }

    return 0;
}

/* Read a message from shared memory. */
int ipc_read(char *buffer, int size)
{
    if (shared_data == NULL || ipc_sem == SEM_FAILED ||
        buffer == NULL || size <= 0) {
        return -1;
    }

    if (sem_wait(ipc_sem) == -1) {
        perror("sem_wait");
        return -1;
    }

    if (!shared_data->data_ready) {
        sem_post(ipc_sem);
        return 0;
    }

    snprintf(buffer, (size_t)size, "%s", shared_data->message);

    shared_data->data_ready = 0;

    if (sem_post(ipc_sem) == -1) {
        perror("sem_post");
        return -1;
    }

    return 1;
}

/* Release shared memory and semaphore resources. */
void ipc_cleanup(void)
{
    if (shared_data != NULL) {
        munmap(shared_data, sizeof(SharedData));
        shared_data = NULL;
    }

    if (shm_fd != -1) {
        close(shm_fd);
        shm_fd = -1;
    }

    if (ipc_sem != SEM_FAILED) {
        sem_close(ipc_sem);
        ipc_sem = SEM_FAILED;
    }

    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
}
