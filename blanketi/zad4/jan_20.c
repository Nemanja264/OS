#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096
#define LOW_END 100*1024
#define HIGH_END 750*1024

void stampajDatoteke(char* path)
{
    struct dirent* entry;
    struct stat file_stat;
    char fullPath[LEN];

    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("Dir");
        exit(1);
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if(stat(fullPath, &file_stat) == -1)
            continue;

        if(S_ISDIR(file_stat.st_mode))
            stampajDatoteke(fullPath);
        else if(S_ISREG(file_stat.st_mode) && (file_stat.st_size > LOW_END && file_stat.st_size < HIGH_END))
        {
            printf("File name: %s\n", entry->d_name);
            printf("File path: %s\n\n", fullPath);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("Not enough arguments\n");
        exit(1);
    }

    stampajDatoteke(argv[1]);

    return 0;
}
