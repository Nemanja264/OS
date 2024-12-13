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
    buff.mtype = 1;
    
    int qid = msgget(RID, 0666);
    if(qid < 0)
    {
        perror("MsgID error");
        exit(1);
    }

    int sum = 0;
    int broj;
    while(sum < 1000)
    {
        printf("Unesi broj:\n");
        scanf("%d", &broj);

        sprintf(buff.mtext, "%d", broj);
        if(msgsnd(qid, &buff, LEN, 0) < 0)
        {
            perror("Sending error");
            break;
        }
        sum += broj;
    }

    return 0;
}
