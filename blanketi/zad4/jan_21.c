#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096

void find_NM(char* path, int N, int M)
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
            find_NM(fullPath, N, M);
        else if(S_ISREG(file_stat.st_mode) && file_stat.st_size > N && strlen(fullPath) > M)
        {
            printf("File name: %s\n", entry->d_name);
            printf("File path: %s\n", fullPath);
            printf("File size: %dB\n\n", (int)file_stat.st_size);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        printf("Input file, N and M\n");
        exit(1);
    }

    find_NM(argv[1], atoi(argv[2]), atoi(argv[3]));

    return 0;
}
