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
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define INITIAL_EMPTY 1
#define INITIAL_FULL 0

// Producer constants
#define TOTAL_RUNS 5

/// MAIN FUNCTION ///
int main()
{
    /// SHARED MEMORY ///
    // Create the shared memory object
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);

    // Configure the size of the shared memory object
    ftruncate(shm_fd, BUFFER_SIZE * sizeof(int));

    // Memory map the shared memory object
    int *buffer = mmap(0, BUFFER_SIZE * sizeof(int), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    /// SEMAPHORE /// 
    sem_t *empty = sem_open(EMPTY_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_EMPTY);
    sem_t *full = sem_open(FULL_NAME, O_CREAT | O_EXCL, SEM_PERMS, INITIAL_FULL);

    /// PRODUCER ///
    // Produce elements when buffer is empty
   int productNum = 1;
   int runs = 0;
   while(1)
   {
        // Wait for mutex to open
        sem_wait(empty);

        // Start of critical section
        printf("\n");
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = productNum;
            printf("Produced [Item %d].\n", buffer[i]);
            productNum++;
        }
        runs++;
        printf("Buffer full.\n");
        // End of critical section

        // Post mutex open
        sem_post(full);

        // Check if productNum exceeds the number of total uses
        if (runs >= TOTAL_RUNS) {
            printf("Force stop producer.\n");
            break;
        }
   }

    return 0;
}
