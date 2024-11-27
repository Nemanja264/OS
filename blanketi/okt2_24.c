
int main() int pid;
int pd1[2], pd2[2];
if (pipe(pdl) < 0) exit(1);
if (pipe(pd2) < 0) exit(1);

pid = fork();
-1)
perror("Fork failed");
pid
if (pid
==
{
}
else if (pid
==
0)
{
exit(1);
close(pd1[1]); close(pd2[0]);
char ime [24];
char key [20];
char line [100];
read (pd1[0], ime, 24);
read (pd1[0], key, 20);
FILE* f = fopen(ime, "r"); if(f == 0)
exit(2);
int lineNum = 0;
while(fgets(line, 100, f))
LineNum++;
