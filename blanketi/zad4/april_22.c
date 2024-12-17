#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define LEN 4096

void find_similar(char* path1, char* path2)
{
    struct dirent *entry1, *entry2;
    struct stat file_stat1, file_stat2;
    char fullPath1[LEN], fullPath2[LEN];

    DIR* dir1 = opendir(path1);
    if(dir1 == NULL)
    {
        perror("dir1");
        exit(1);
    }

    while((entry1 = readdir(dir1)) != NULL)
    {
        if(strcmp(entry1->d_name, ".") == 0 || strcmp(entry1->d_name, "..") == 0)
            continue;
        
        snprintf(fullPath1, sizeof(fullPath1), "%s/%s", path1, entry1->d_name);

        if(stat(fullPath1, &file_stat1) == -1)
            continue;

        if(S_ISREG(file_stat1.st_mode))
        {
            DIR* dir2 = opendir(path2);
            if(dir2 == NULL)
            {
                perror("dir2");
                exit(1);
            }

            while((entry2 = readdir(dir2)) != NULL)
            {
                if(strcmp(entry2->d_name, ".") == 0 || strcmp(entry2->d_name, "..") == 0)
                    continue;
                
                snprintf(fullPath2, sizeof(fullPath2), "%s/%s", path2, entry2->d_name);

                if(stat(fullPath2, &file_stat2) == -1)
                    continue;

                if(S_ISREG(file_stat2.st_mode) && strcmp(entry1->d_name, entry2->d_name) == 0 && file_stat1.st_size == file_stat2.st_size)
                {
                    printf("Datoteka %s iz %s, postoj i u %s\n", entry1->d_name, path1, fullPath2);
                }
            }
            closedir(dir2);
        }
    }

    closedir(dir1);
}

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("Not enough arguments\n");
        exit(1);
    }

    find_similar(argv[1], argv[2]);
    
    return 0;
}
