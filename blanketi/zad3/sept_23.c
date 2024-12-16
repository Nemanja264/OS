#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define KEY 10304
#define LEN 255

struct msgbuf
{
    long mtype;
    char mtext[LEN];
};

int main()
{
    int pid;
    int qid = msgget(KEY, IPC_CREAT|0666);
    struct msgbuf buff;
    FILE* s = fopen("sredjeno.txt", "a");
    if(s == NULL)
    {
            perror("File failure");
            exit(1);
    }
    
    pid = fork();
    if(pid == -1)
    {
        perror("Fork Failed");
    }

    if(pid == 0)
    {
        while(1)
        {
            if(msgrcv(qid, &buff, sizeof(buff.mtext), 1, 0) < 0)
            {
                perror("Msgrcv2");
                exit(1);
            }

            if(strcmp(buff.mtext, "KRAJ") == 0)
                break;
            
            for(int i = 0; i<strlen(buff.mtext); i++)
                buff.mtext[i] = toupper(buff.mtext[i]);
            
            fprintf(s, "%s", buff.mtext);
            fflush(s);
        }

        fclose(s);
        return 0;
    }

    pid = fork();
    if(pid == -1)
    {
        perror("Fork Failed");
    }

    if(pid == 0)
    {
        while(1)
        {
            if(msgrcv(qid, &buff, sizeof(buff.mtext), 2, 0) < 0)
            {
                perror("Msgrcv3");
                exit(1);
            }

            if(strcmp(buff.mtext, "KRAJ") == 0)
                break;
            
            for(int i = 0; i<strlen(buff.mtext); i++)
                buff.mtext[i] = tolower(buff.mtext[i]);
            
            fprintf(s, "%s", buff.mtext);
            fflush(s);
        }

        fclose(s);
        return 0;
    }

    FILE* f = fopen("poruka.txt", "r");
    if(f == NULL)
    {
        perror("File failure");
        exit(1);
    }

    int red = 0;
    while(fgets(buff.mtext, LEN, f))
    {
        if(red%2 == 0)
            buff.mtype = 1;
        else
            buff.mtype = 2;
        
        if(msgsnd(qid, &buff, sizeof(buff.mtext), 0) < 0)
        {
            perror("Msgsnd");
            exit(1);
        }
        red++;
    }
    fclose(f);

    strcpy(buff.mtext, "KRAJ");
    buff.mtype = 1;
    msgsnd(qid, &buff, sizeof(buff.mtext), 0);
    buff.mtype = 2;
    msgsnd(qid, &buff, sizeof(buff.mtext), 0);

    wait(NULL);
    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);

    return 0;
}
