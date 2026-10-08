/*
            COP 4610 Operating Systems Principles - Project 1 (Shared Memory)
                        Name: Nafiul Khalid (nkhal014@fiu.edu)
                        PID: 6436457
                        username: nkhal014 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

/* key number */
#define SHMKEY ((key_t) 6457)

#define NUM_CHILDREN 4

/* Layout of the shared memory segment */
typedef struct
{
    int value;
} shared_mem;

/* Pointer to the shared variable */
shared_mem *total;

/* Increment the shared variable "count" times, one at a time */
static void increment_total(int count)
{
    int i;

    for (i = 0; i < count; i++)
        total->value = total->value + 1;
}


/* Child 1: increment total by one, 100,000 times */
void process1(void)
{
    increment_total(100000);
    printf("From Process 1: counter = %d.\n", total->value);
}

/* Child 2: increment total by one, 200,000 times */
void process2(void)
{
    increment_total(200000);
    printf("From Process 2: counter = %d.\n", total->value);
}

/* Child 3: increment total by one, 300,000 times */
void process3(void)
{
    increment_total(300000);
    printf("From Process 3: counter = %d.\n", total->value);
}

/* Child 4: increment total by one, 500,000 times */
void process4(void)
{
    increment_total(500000);
    printf("From Process 4: counter = %d.\n", total->value);
}



int main(void)
{
    int shmid, i, status;
    pid_t pid, exited[NUM_CHILDREN];
    char *shmadd = (char *) 0;
    void (*child_work[NUM_CHILDREN])(void) = { process1, process2, process3, process4 };

    /* Create the shared memory segment */
    if ((shmid = shmget(SHMKEY, sizeof(shared_mem), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* Attach the segment to the parent's address space */
    if ((total = (shared_mem *) shmat(shmid, shmadd, 0)) == (shared_mem *) -1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        exit(1);
    }

    /* Initialize the shared variable to 0 */
    total->value = 0;

    /* Create the 4 children */
    for (i = 0; i < NUM_CHILDREN; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            child_work[i]();
            fflush(stdout);
            exit(0);
        }
    }

    /* Parent: wait for every child, recording each PID in the order the
     * children finish, so the exit messages print after all child output */
    for (i = 0; i < NUM_CHILDREN; i++) {
        exited[i] = wait(&status);
        if (exited[i] < 0) {
            perror("wait");
            exit(1);
        }
    }

    /* Print each child's PID in the order it exited */
    for (i = 0; i < NUM_CHILDREN; i++)
        printf("Child with ID: %d has just exited.\n", (int) exited[i]);

    /* All children are done: detach from the shared memory */
    if (shmdt(total) == -1) {
        perror("shmdt");
        exit(-1);
    }

    /* remove the segment from the system*/
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(-1);
    }

    printf("End of Program.\n");
    return 0;
}

