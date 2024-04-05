#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

/// CONSTANTS ///
// Shared memory constants
#define SHM_NAME "/buffer"
#define BUFFER_SIZE 2

// Semaphore constants
#define EMPTY_NAME "/empty"
#define FULL_NAME "/full"

// Consumer constants
#define TOTAL_RUNS 5

/// MAIN FUNCTION ///
int main()
{
    /// SHARED MEMORY ///
    // Open the shared memory object
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);

    // Memory map the shared memory object
    int *buffer = mmap(0, BUFFER_SIZE * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /// SEMAPHORE ///
    sem_t *empty = sem_open(EMPTY_NAME, O_RDWR);
    sem_t *full = sem_open(FULL_NAME, O_RDWR);

    /// CONSUMER ///
    // Consume elements when buffer is full.
    int runs = 0;
    while(1)
    {
        // Wait for mutex to open
        sem_wait(full);

        // Start of critical section
        printf("\n");
        for (int i = (BUFFER_SIZE - 1); i >= 0; i--) {
            printf("Consumed [Item %d].\n", buffer[i]);
            buffer[i] = 0;
        }
        runs++;
        printf("Buffer empty.\n");
        // End of critical section

        // Post mutex open
        sem_post(empty);

        // Check if itemsConsumed exceeds the number of total uses
        if (runs >= TOTAL_RUNS) {
            printf("Force stop consumer.\n");
            break;
        }
    }

    // Remove the shared memory objects and semaphore
    shm_unlink(SHM_NAME);
    sem_unlink(EMPTY_NAME);
    sem_unlink(FULL_NAME);
    return 0;
}