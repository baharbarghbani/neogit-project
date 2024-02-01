
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<time.h>
#define MAX_ADDRESS_SIZE 2000
#define OPENNING_DIRECTORY_ERROR "Error openning directory\n"
void repo_check(bool exists, char* address, char* repo_address)
{
    address = (char*)malloc(MAX_ADDRESS_SIZE);
    repo_address = (char*)malloc(MAX_ADDRESS_SIZE);
    struct dirent* entry;
    do{
        DIR* dir = opendir(".");
        if(dir == NULL)
        {
            fprintf(stderr, OPENNING_DIRECTORY_ERROR);
            return;
        }
        if(getcwd(address, MAX_ADDRESS_SIZE) == NULL)
        {
            fprintf(stderr,OPENNING_DIRECTORY_ERROR);
            return;
        }
        while((entry = readdir(dir)) != NULL)
        {
            if(entry->d_type == DT_DIR && (strcmp(entry->d_name, ".neogit") == 0))
            {
                if(getcwd(repo_address,MAX_ADDRESS_SIZE) == NULL)
                {
                    fprintf(stderr, OPENNING_DIRECTORY_ERROR);
                    return;
                }
                exists = true;
                break;
            }
        }
        if(exists) break;
        if(strcmp(address, "/") != 0)
        {
            if(chdir("..") != 0)
            {
                fprintf(stderr, OPENNING_DIRECTORY_ERROR);
                return;
            }
        }
        closedir(dir);
    }while(strcmp(address, "/") != 0);
    return;
}
