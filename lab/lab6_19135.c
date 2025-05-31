#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdbool.h>

#define KEY 10110
#define SHM_SIZE 256
#define child 1
#define parent 0

union semun
{
    int val;
};

int create_shared_memory(int key, int size)
{
    int shmid = shmget(key, size, IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("Shmget");
        exit(1);
    }

    return shmid;
}

void initialize_semaphore(union semun sem, int semid, int sem_num)
{
    sem.val = 0;
    if(semctl(semid, sem_num, SETVAL, sem) == -1)
    {
        perror("Semaphore initialization failed");
        exit(1);
    }
}

int create_semaphores(int key)
{
    int semid = semget(key, 2, IPC_CREAT|0666);
    if(semid < 0)
    {
        perror("Semget");
        exit(1);
    }

    union semun sems;
    initialize_semaphore(sems, semid, parent);
    initialize_semaphore(sems, semid, child);

    return semid;
}

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

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Not enough arguments in command line\n");
        exit(1);
    }

    int shmid = create_shared_memory(KEY, SHM_SIZE);
    char* sh_mem = (char*)shmat(shmid, NULL, 0);

    int semid = create_semaphores(KEY);
    
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
            

            if(strcmp(sh_mem, "END\n") == 0)
            {
                sem_signal(semid, parent);
                break;
            }
            
            printf("%s", sh_mem);
            sem_signal(semid, parent);
        }

        shmdt(sh_mem);
        return 0;
    }
    else
    {
        FILE* f = fopen(argv[1], "r");
        if(f == NULL)
        {
            perror("File failed");
            exit(1);
        }

        while(fgets(sh_mem, SHM_SIZE, f))
        {
            sem_signal(semid, child);
            sem_wait(semid, parent);
        }
        fclose(f);
        strncpy(sh_mem, "END\n", SHM_SIZE);

        sem_signal(semid, child);
        wait(NULL);

        shmdt(sh_mem);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }

    return 0;
}
