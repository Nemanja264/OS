#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096
#define MAX_DEPTH 3
#define SIZE 15*1024

void delete_file(char* path, int depth)
{
    if(depth > MAX_DEPTH)
        return;

    struct dirent* entry;
    struct stat file_stat;
    char fullPath[LEN];

    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("dir");
        exit(1);
    }

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(fullPath, sizeof(fullPath), "%s%s", path, entry->d_name);

        if(stat(fullPath, &file_stat) == -1)
            continue;
        
        if(S_ISREG(file_stat.st_mode) && file_stat.st_size > SIZE && strstr(entry->d_name, "log") != NULL)
        {
            char command[LEN + 10];
            snprintf(command, sizeof(command), "rm %s", fullPath);

            if(system(command) == -1)
            {
                perror("Error deleting file");
            }
            else
            {
                printf("File %s deleted\n", entry->d_name);
            }
        }

        if(S_ISDIR(file_stat.st_mode))
            delete_file(fullPath, depth+1);
    }

    closedir(dir);
}

int main()
{
    char dir_path[LEN];

    printf("Unesi put do direktorijuma: ");
    scanf("%s", dir_path);

    delete_file(dir_path, 0);

    return 0;
}
