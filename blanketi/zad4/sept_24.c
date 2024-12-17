#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MEM_KEY 10106
#define SHM_SIZE sizeof(int)*6
#define N 5
#define child 1
#define parent 0

union semun
{
    int val;
};

void sem_wait(int semid, int semnum)
{
    struct sembuf op = {semnum, -1, 0};
    semop(semid, &op, 1);
}

void sem_signal(int semid, int semnum)
{
    struct sembuf op = {semnum, 1, 0};
    semop(semid, &op, 1);
}

int main()
{
    int shmid = shmget(MEM_KEY, SHM_SIZE, IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("Shmget");
        exit(1);
    }

    int* sh_mem = (int*)shmat(shmid, NULL, 0);

    int semid = semget(MEM_KEY, 2, IPC_CREAT|0666);
    if(semid == -1)
    {
        perror("Semget");
        exit(1);
    }

    union semun sems;
    sems.val = 0;
    if(semctl(semid, parent, SETVAL, sems) == -1)
    {
        perror("parent failed");
        exit(1);
    }

    sems.val = 0;
    if(semctl(semid, child, SETVAL, sems) == -1)
    {
        perror("child failed");
        exit(1);
    }

    int pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        while(1)
        {
            sem_wait(semid, child);
            
            int sum = 0;
            for(int i = 0; i<N; i++)
                sum += sh_mem[i];
            sh_mem[N+1] = sum;

            sem_signal(semid, parent);

            if(sum == 0)
                break;
        }

        shmdt(sh_mem);
        return 0;
    }
    else
    {
        
        while(1)
        {
            printf("Unesi 5 broja:\n");
            for(int i = 0; i<N; i++)
                scanf("%d", &sh_mem[i]);
            
            sem_signal(semid, child);

            sem_wait(semid, parent);

            int sum = sh_mem[N+1];
            printf("Suma: %d\n", sum);

            if(sum == 0)
                break;
        }

        wait(NULL);
        shmdt(sh_mem);
        shmctl(shmid, IPC_RMID, NULL);

        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}
