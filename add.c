#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<time.h>
#include "repo_check.c"
#define MAX_ADDRESS_SIZE 1000
#define OPENNING_DIRECTORY_ERROR "Error openning directory\n"

void repo_check(bool exist, char* address, char* repo_address);
int add(int argc, char const* argv[])
{
    char* address = (char*)malloc(MAX_ADDRESS_SIZE);  //used
    char* repo_address = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* cwd = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* tmp_cwd = (char*)malloc(MAX_ADDRESS_SIZE);
    char* argv = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* path = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* stage_path = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* stage_txt = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* line = (char*)malloc(MAX_ADDRESS_SIZE);
    bool stage_exists = false;
    bool found = false;
    bool exists = false;
    bool repeated = false;
    bool is_in_repo = false;
    bool is_file = false;
    bool is_address = false;
    bool is_name = false;
    bool is_directory = false;
    bool is_valid = false;
    struct dirent* entry;
    struct stat* path_stat;
    FILE* stage;
    if(getcwd(cwd, 2000) == NULL)
    {
        fprintf(stderr, OPENNING_DIRECTORY_ERROR);
        return 1;
    }
    repo_check(&exists, address, repo_address); //checking for initialization
    if(!exists){
        fprintf(stderr, "Repository hass not been initialized\n");
        return 1;
    }
    strcat(repo_address, "/.neogit");
    chdir(repo_address);
    DIR* dir = opendir("."); //changing to repo directory 
    while((entry = readdir(dir)) != NULL) //searching if the stage directory exists
    {
        if(entry->d_type == DT_DIR && strcmp(entry->d_name, "stage") == 0)
        {
            stage_exists = true;
            break;
        }
    }
    if(!stage_exists)
    {
        if(mkdir("stage", 0755) != 0)
        {
            fprintf(stderr, "Error making stage directory\n");
            return 1;
        }
    }
    strcpy(stage_txt, repo_address);
    strcat(stage_txt, "/stage_list.txt");
    strcpy(stage_path, repo_address);
    strcat(stage_path, "/stage"); //saving the address of the stage directory in repo

    closedir(dir);

    chdir(cwd);
    dir = opendir(".");//back in the current directory

    strcpy(argv, argv[2]);

    if(strstr(argv, "/") != NULL)  is_address = true;  //checking if the command is address or name
    else{is_name = true;};

    strcpy(argv[2], argv);
    if(is_address) //command is address
    {
        if(strstr(argv, ".neogit") != NULL) //is it in the repo or not
        {
            is_in_repo = true;
        }
        if(!is_in_repo) //the address isn't in the repo
        {
            fprintf(stderr, "The given address does not exist in repository\n");
            return 1;
        }
        if(is_in_repo) //the address is in the repo
        {
            if(access(argv[2], F_OK) != -1) //is the address valid?
            {
                is_valid = true;
            }
            if(!is_valid) //the address isn't valid
            {
                fprintf(stderr, "The given address is not valid\n");
                return 1;
            }
            if(is_valid) //address is valid
            {
                path = realpath(argv[2], NULL); //getting the absolute path
            }
        }
    }
    if(is_name) //command is name
    {           //now we're in the current working directory
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, argv[2]) == 0)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            fprintf(stderr, "The given file does not exist\n");
            return 1;
        }
        else if(found)
        {
           strcpy(path, cwd);
           strcat(path, "/");
           strcat(path, argv[2]);  //absolute path is done   
        }
    }
    if(stat(path_stat, &path) == -1)
    {
        fprintf(stderr, "Error in stat\n");
        return 1;
    }
    //finding out the tyoe of the file
    if(S_ISREG(path_stat.st_mode))   is_file = true;
    else if(S_ISDIR(path_stat.st_mode))   is_directory = true;
    else{
        fprintf(stderr, "The reffered file is not neither a file nor directory\n");
        return 1;
    }
//first add file
if(is_file)
{
    int line_count = 0;
    stage = fopen(stage_txt, "r");
    if(stage == NULL)
    {
        fprintf(stderr, "Error openning stage_list.txt\n");
        return 1;
    }
    while(fgets(line, 1000, stage) != NULL)
    {
        line_count++;
        size_t ln = strlen(line);
        line[ln - 1] = '\0';
        if(strcmp(line, path) == 0)
        {
            repeated = true;
            break;
        }
    }
    rewind(stage);
    fclose(stage);
    if(!repeated)
    {
        
    }
    


}




    return 0;
}