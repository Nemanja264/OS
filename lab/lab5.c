#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/wait.h>

#define n 100

struct mymsgbuf
{
    long mtype;
    char txt[n];
};

int main()
{
    int pid;
    int msqid;
    struct mymsgbuf buff;

    buff.mtype = 1;
    strcpy(buff.txt, "");
    msqid = msgget(10109, 0666 | IPC_CREAT);
    if(msqid == -1)
    {
        perror("MSG failed");
        exit(2);
    }

    pid = fork();

    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }
    else if(pid == 0)
    {
        FILE* f = fopen("izlaz.txt", "w");
        if(f == 0)
            exit(1);

        while(strcmp(buff.txt, "KRAJ") != 0)
        {
            if(msgrcv(msqid, &buff, n, 1, 0) == -1)
            {
                perror("msgrcv");
                break;
            }
            
            if(strcmp(buff.txt, "KRAJ") != 0)
                fprintf(f, "%s\n", buff.txt);
        }
    
        fclose(f);
    }
    else
    {
        
        while(strcmp(buff.txt, "KRAJ") !=0)
        {
            printf("Unesi text: ");
            fgets(buff.txt, n, stdin);
            buff.txt[strcspn(buff.txt, "\n")] = 0;
            
            msgsnd(msqid, &buff, strlen(buff.txt)+1, 0);
        }
 
        wait(NULL);
        msgctl(msqid, IPC_RMID, NULL);
    }

    return 0;
}
