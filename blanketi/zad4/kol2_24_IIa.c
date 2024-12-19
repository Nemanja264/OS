#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N 10
#define KEY 10300

union semun
{
    int val;
};

int main()
{
    int shmid = shmget(KEY, sizeof(int), IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("Shmget");
        exit(1);
    }

    int semid = semget(KEY, 1, IPC_CREAT|0666);

    union semun semops;
    semops.val = 0;
    semctl(semid, 0, SETVAL, semops);

    struct sembuf sem_wait = {0, -1, 0};
    struct sembuf sem_post = {0, 1, 0};


    for(int i = 0; i<N; i++)
    {
        int pid = fork();
        if(pid == -1)
        {
            perror("Fork failed");
            break;
        }

        if(pid == 0)
        {
            int* shmem = (int*)shmat(shmid, NULL, 0);

            *shmem = getpid();

            semop(semid, &sem_post, 1);

            shmdt(shmem);

            return 0;
        }
        else
        {
            int* shmem = (int*)shmat(shmid, NULL, 0);

            semop(semid, &sem_wait, 1);
            int pidChild = *shmem;

            printf("Child PID: %d, Parent PID: %d\n", pidChild, getpid());
            if(pidChild == getpid()+1)
                printf("PID deteta je veci za jedan\n");
            else
                printf("PID deteta nije veci za jedan\n");

            wait(NULL);

            shmdt(shmem);
        }
    }

    semctl(semid, 0, IPC_RMID, 0);
    shmctl(shmid, 0, IPC_RMID);

    return 0;
}
