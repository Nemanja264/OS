#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define N 5
#define KEY 10300

union semun
{
    int val;
};

int main()
{
    srand(time(NULL));

    int shmid = shmget(KEY, sizeof(int)*N*N, IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("Shmget");
        exit(1);
    }

    int* mat = (int*)shmat(shmid, NULL, 0);
    int semid = semget(KEY, 1, IPC_CREAT|0666);

    union semun semopts;
    semopts.val = 0;
    semctl(semid, 0, SETVAL, semopts);

    struct sembuf sem_wait = {0, -1, 0};
    struct sembuf sem_post = {0, 1, 0};

    int pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0)
    {
        semop(semid, &sem_wait, 1);

        for(int i=0; i<N; i++)
        {
            int max = mat[i*N];
            for(int j = 1; j<N; j++)
                if(max < mat[i*N+j])
                    max = mat[i*N+j];
                    
            printf("Max element kolone %d: %d\n", i+1, max);
        }

        shmdt(mat);
        return 0;
    }
    else
    {
        for(int i=0; i<N; i++){
            for(int j = 0; j<N; j++)
            {
                mat[i*N+j] = rand()%100 + 1;
                printf("%d ", mat[i*N+j]);
            }
            printf("\n");
        }

        semop(semid, &sem_post, 1);

        wait(NULL);
        shmdt(mat);
    }

    semctl(semid, 0, IPC_RMID, 0);
    shmctl(shmid, 0, IPC_RMID);
    return 0;
}
