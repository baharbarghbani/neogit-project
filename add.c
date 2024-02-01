#include "add.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<time.h>
void repo_check(bool exist, 
int add(int argc, char const* argv[])
{
    char* address = (char*)malloc(2000);
    char* repo_address = (char*)malloc(2000);
    char* cwd = (char*)malloc(2000);
    char* tmp_cwd = (char*)malloc(2000);
    bool found = false;
    bool exists = false;
    bool is_file = false;
    bool is_directory = false;
    bool is_valid = false;
    struct dirent* entry;
    
    if(getcwd(cwd, 2000) == NULL)
    {
        fprintf(stderr, "Error in current directory\n");
        return 1;
    }




}