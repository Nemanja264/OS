#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MEM_KEY 10105
#define SHM_SIZE 1024

int main()
{
    int shmid = shmget(MEM_KEY, SHM_SIZE, IPC_CREAT|0666);
    if(shmid < 0)
    {
        perror("Shmget failed");
        exit(1);
    }

    char* sh_mem = (char*)shmat(shmid, NULL, 0);

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
            if(strlen(sh_mem) > 0)
            {
                if(strcmp(sh_mem, "KRAJ") == 0)
                    break;

                printf("Child received: %s\n", sh_mem);
                fflush(stdout);

                memset(sh_mem, 0, SHM_SIZE);
            }
        }

        shmdt(sh_mem);
    }
    else
    {
        char txt[SHM_SIZE];

        while(1)
        {
            printf("Unesi text:\n");
            fgets(txt, SHM_SIZE, stdin);

            txt[strcspn(txt, "\n")] = '\0';

            strncpy(sh_mem, txt, SHM_SIZE);

            if(strcmp(txt, "KRAJ") == 0)
            {
                printf("KRAJ unosa\n");
                break;
            }

            while(strlen(sh_mem) > 0)
                usleep(100);
        }

        wait(NULL);

        shmdt(sh_mem);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
