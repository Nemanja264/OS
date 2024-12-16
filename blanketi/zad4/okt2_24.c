#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 1024

void dir_process(char* path)
{
    struct dirent* entry;
    struct stat sfile;
    char fullPath[LEN];

    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("Dir failure");
        exit(1);
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullPath, sizeof(fullPath), "%s%s", path, entry->d_name);

        if(stat(fullPath, &sfile) == -1)
        {
            perror("stat");
            continue;
        }

        if(S_ISDIR(sfile.st_mode))
        {
            dir_process(fullPath);
        }
        else if(S_ISREG(sfile.st_mode))
        {
            if(strstr(entry->d_name, ".sh") != NULL)
            {
                printf("Executing %s script\n", fullPath);
                if(fork() == 0)
                {
                    execlp("sh", "sh", fullPath, (char*)NULL);
                    perror("execlp failure");
                    exit(1);
                }

                wait(NULL);
            }
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Argument missing\n");
        exit(1);
    }

    dir_process(argv[1]);

    return 0;
}
