#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/types.h>

#define RID 10302
#define LEN 255

struct msgbuf
{
    long mtype;
    char mtext[LEN];
};
int main()
{
    struct msgbuf buff;
    int pid;
    int qid = msgget (RID, 0666|IPC_CREAT);

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        if(execl("prvi", "prvi", NULL) < 0)
        {
            perror("execl1");
            exit(1);
        }
    }

    pid = fork();
    if(pid == -1)
    {
        perror("Fork failed");
        exit(1);
    }

    if(pid == 0)
    {
        if(execl("drugi", "drugi", NULL) < 0)
        {
            perror("execl2");
            exit(1);
        }
    }

    wait(NULL);
    wait(NULL);
    msgctl(qid, IPC_RMID, NULL);
    return 0;
}
