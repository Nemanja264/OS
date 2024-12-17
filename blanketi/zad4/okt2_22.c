#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096

void pronadjiDatoteku(char* directory, char* file)
{
    struct dirent* entry;
    struct stat file_stat;
    char fullPath[LEN];

    DIR* dir = opendir(directory);
    if(dir == NULL)
    {
        perror("dir");
        exit(1);
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(fullPath, sizeof(fullPath), "%s/%s", directory, entry->d_name);

        if(stat(fullPath, &file_stat) == -1)
            continue;
        
        if(S_ISREG(file_stat.st_mode) && strcmp(entry->d_name, file) == 0)
        {
            printf("File location: %s\n", fullPath);
            printf("File size: %ld bytes\n", file_stat.st_size);
            closedir(dir);
            return;
        }

        if(S_ISDIR(file_stat.st_mode))
            pronadjiDatoteku(fullPath, file);
    }
    
    closedir(dir);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Not enough arguments\n");
        exit(1);
    }

    pronadjiDatoteku(argv[1], argv[2]);

    return 1;
}
