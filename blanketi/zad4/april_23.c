#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096
#define maxDepth 5

struct largest_file
{
    char name[LEN];
    char fpath[LEN];
    int size;
};
void maxFile(char* path, int depth, struct largest_file* lf)
{
    if(depth > maxDepth)
        return;

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
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name,"..") == 0)
            continue;

        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if(stat(fullPath, &file_stat) == -1)
            continue;
        

        if(S_ISREG(file_stat.st_mode))
            if(file_stat.st_size > lf->size)
            {
                lf->size = file_stat.st_size;
                strncpy(lf->fpath, fullPath, LEN);
                strncpy(lf->name, entry->d_name, LEN);
            }
        
        if(S_ISDIR(file_stat.st_mode))
            maxFile(fullPath, depth+1, lf);
        
    }

    closedir(dir);
}

int main(int argc, char* argv[])
{
    struct largest_file lf = {"", 0};

    maxFile(argv[1], 0, &lf);

    printf("Largest file in directories and subdirectories:\n");
    printf(" name: %s\n path: %s\n size: %d\n", lf.name, lf.fpath, lf.size);

    return 0;
}
