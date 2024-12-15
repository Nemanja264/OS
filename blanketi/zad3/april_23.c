#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define KEY 10302

struct msgbuf
{
    long mtype;
    int mnum;
};
int main()
{
    int pid;
    int qid;
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
        int i = 0;
        while(1)
        {
            if(msgrcv(qid, &buff, sizeof(buff.mnum), 0, 0) < 0)
            {
                perror("Msgrcv");
                exit(1);
            }
            i++;

            if(buff.mnum == 0)
            {
                printf("Primljeno je %d brojeva\n", i-1);
                exit(0);
            }

            char ascii = (char) buff.mnum;

            printf("%d = %c", buff.mnum, ascii);
        }
    }
    else
    {
        do
        {
            printf("Unesi broj [0-127], 0 for termination:");
            scanf("%d", &buff.mnum);

            buff.mtype = 0;

            if(buff.mnum < 128)
            {
                if(msgsnd(qid, &buff, sizeof(buff.mnum), 0) < 0)
                {
                    perror("Msgsnd");
                    exit(1);
                }
            }

        } while(buff.mnum != 0);

        wait(NULL);
        msgctl(qid, IPC_RMID, NULL);
    }

    return 0;
}
