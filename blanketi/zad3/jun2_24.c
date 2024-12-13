#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define RID 10301
#define LEN 255

struct msgbuff
{
    long mtype;
    char mtext[LEN];    
};

int main()
{
    int pid;
    int qid = msgget(RID, IPC_CREAT|0666);
    struct msgbuff buff;

    buff.mtype = 1;

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        int broj;
        FILE* f = fopen("zbirCifara", "w");
        if(f == NULL)
        {
            perror("File failure");
            exit(1);
        }

        do{
            int zbir = 0;

            if(msgrcv(qid, &buff, LEN, 0, 0) < 0)
            {
                perror("Receiving failure");
                exit(1);
            }
            broj = atoi(buff.mtext);

            if(broj != 0)
            {
                for(int i=0; i<strlen(buff.mtext); i++)
                    zbir += buff.mtext[i] - '0';
                
                fprintf(f, "%d|%d\n", broj, zbir);
                fflush(f);
            }

        } while(broj != 0);

        fclose(f);
    }
    else
    {
        int broj;

        do{
            printf("Unesi broj:");
            scanf("%d", &broj);

            if((broj > 99 && broj < 1000) || broj == 0)
            {
                sprintf(buff.mtext, "%d", broj);
                if(msgsnd(qid, &buff, LEN, 0) < 0)
                {
                    perror("Sending failure");
                    exit(1);
                }
            }
        } while(broj != 0);

        wait(NULL);
        msgctl(qid,IPC_RMID, NULL);
    }

    return 0;
}
