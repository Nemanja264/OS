d) --- lab vezba za procfs 

#include <stdlib.h> #include <string.h>
int main(int argc, char* argv[])
{
char* buf1 = NULL;
int i;
FILE* f1;

f1 =fopen("/proc/devices","r");

if(!fl)
{
printf("Greska pri otvaranju!");
return -1;
}

size_t len = 0;

while (!feof(fl))
{
  if(getline(&buf1, &len, f1) != -1) {
    
    if(strstr(buf1, "Block devices:"))
      i=1;
    
    if(i == 1)
      printf("%s", buf1);
  }
}

fclose(f1);
return 0;
}
