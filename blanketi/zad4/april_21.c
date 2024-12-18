#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096
#define CAPACITY 1000
#define SIZE 100*1024

struct bigFiles
{
    char path[LEN];
    char name[LEN];
    int size;
};

struct bigFiles files[CAPACITY];
int fcount = 0;

void print_files(struct bigFiles bf[], int n)
{
    for(int i = 0; i<n; i++)
    {
        printf("File path: %s\n", bf[i].path);
        printf("File name: %s\n", bf[i].name);
        printf("File size: %d\n\n", bf[i].size);
    }
}

void sort_files(struct bigFiles bf[], int n)
{
    for(int i = 0; i<n-1; i++)
    {
        for(int j = 0; j<n-i-1; j++)
        {
            if(bf[j].size > bf[j+1].size)
            {
                struct bigFiles tmp = bf[j];
                bf[j] = bf[j+1];
                bf[j+1] = tmp;
            }
        }
    }
}

void moreThan100Kb(char* path)
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

    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if(stat(fullPath, &file_stat) == -1)
            continue;

        if(S_ISREG(file_stat.st_mode) && file_stat.st_size > SIZE)
        {
            if(fcount < CAPACITY)
            {
                strcpy(files[fcount].path, fullPath);
                strcpy(files[fcount].name, entry->d_name);
                files[fcount].size = file_stat.st_size;
                fcount++;
            }
            else
            {
                printf("Maximum capacity reached\n");
            }
        }

        if(S_ISDIR(file_stat.st_size))
            moreThan100Kb(fullPath);
        
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
    
    moreThan100Kb(argv[1]);

    sort_files(files, fcount);

    print_files(files, fcount);

    return 0;
}
