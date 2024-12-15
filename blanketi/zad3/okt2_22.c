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
    int qid;
    int pid;
    qid = msgget(KEY, IPC_CREAT|0666);
    struct msgbuf buff;

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        while(1)
        {
            if(msgrcv(qid, &buff, LEN, 1, 0) < 0)
            {
                perror("Second process rcv");
                exit(1);
            }

            if(strcmp(buff.mtext, "EXIT") == 0)
            {
                printf("Exiting 2nd process\n");
                break;
            }

            printf("Second process: %s, length %d", buff.mtext, (int)strlen(buff.mtext));
        }
    }
    else
    {
        pid = fork();
        if(pid == -1)
        {
            perror("Fork failed");
            exit(1);
        }
        else if(pid == 0)
        {
            while(1)
            {
                if(msgrcv(qid, &buff, LEN, 2, 0) < 0)
                {
                    perror("Third process rcv");
                    exit(1);
                }

                if(strcmp(buff.mtext, "EXIT") == 0)
                {
                    printf("Exiting 3rd process\n");
                    break;
                }

                for(int i = 0; i<strlen(buff.mtext); i++)
                    buff.mtext[i] = toupper(buff.mtext[i]);

                printf("Third process: %s", buff.mtext);
            }
        }
        else
        {
            while(1)
            {
                int cifra;
                printf("Unesi cifru:\n");
                scanf("%d", &cifra);
                getchar();

                if(cifra != 1 && cifra != 2)
                {
                    strcpy(buff.mtext, "EXIT");
                    buff.mtype = 1;
                    msgsnd(qid, &buff, LEN, 0);
                    buff.mtype = 2;
                    msgsnd(qid, &buff, LEN, 0);
                    break;
                }
                buff.mtype = (long)cifra;

                printf("Unesi string:\n");
                fgets(buff.mtext, LEN, stdin);
                buff.mtext[strcspn(buff.mtext,"\n")] = '\0';

                if(msgsnd(qid, &buff, LEN, 0) < 0)
                {
                    perror("Msgsnd");
                    exit(1);
                }

                sleep(5);
            }
            wait(NULL);
            wait(NULL);
            msgctl(qid, IPC_RMID, NULL);
        }
    }

    return 0;
}
