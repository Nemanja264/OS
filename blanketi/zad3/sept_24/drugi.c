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

int main(int argc, char* argv[])
{
    struct msgbuf buff;

    int qid = msgget(RID, 0666);
    if(qid < 0)
    {
        perror("MsgID error");
        exit(1);
    }

    int sum = 0;
    while(sum < 1000)
    {
        if(msgrcv(qid, &buff, LEN, 1, 0) < 0)
        {
            perror("Receiving error");
            break;
        }

        sum += atoi(buff.mtext);
        printf("%s, trenutna suma %d\n", buff.mtext, sum);
    }

    return 0;
}
