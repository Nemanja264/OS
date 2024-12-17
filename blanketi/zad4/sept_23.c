#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096
#define LINE_LEN 100*sizeof(char)

void read_m_from_n(char* path, int n, int m)
{
    struct dirent* entry;
    struct stat file_stat;
    char fullPath[LEN];

    DIR* dir = opendir(path);
    if(dir == NULL)
    {
        perror("dir");
        exit(1);
    }

    int i = 0;
    while((entry = readdir(dir)) != NULL && i < n)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(fullPath, sizeof(fullPath), "%s%s", path, entry->d_name);

        if(stat(fullPath, &file_stat) == -1)
            continue;
        
        if(S_ISREG(file_stat.st_mode))
        {
            FILE* f = fopen(fullPath, "r");
            if(f == NULL)
            {
                perror("File failure");
                continue;
            }

            char line[LINE_LEN];
            int j = 0;
            printf("File number %d, %s:\n", n+1, entry->d_name);
            while(fgets(line, LINE_LEN, f) && j<m)
            {
                printf("line%d: %s", j+1, line);
                j++;
            }

            printf("\n");
            fclose(f);
            i++;
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    int n = atoi(argv[2]);
    int m = atoi(argv[3]);

    printf("Reading first %d lines from first %d files in directory %s\n\n", m, n, argv[1]);
    read_m_from_n(argv[1], n, m);

    return 0;
}
