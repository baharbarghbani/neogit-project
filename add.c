#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<time.h>
#include<limits.h>
#include<libgen.h>
#include "repo_check.c"
#define OPENNING_DIRECTORY_ERROR "Error openning directory\n"

void repo_check(bool exist, char* address, char* repo_address);
int add(int argc, char* argv[])
{
    char* address = (char*)malloc(MAX_ADDRESS_SIZE);  //used
    char* repo_address = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* cwd = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* tmp_cwd = (char*)malloc(MAX_ADDRESS_SIZE);
    char* argv_copy = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* path = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* stage_path = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* stage_txt = (char*)malloc(MAX_ADDRESS_SIZE); //used
    char* line = (char*)malloc(MAX_ADDRESS_SIZE);
    char* name = (char*)malloc(MAX_ADDRESS_SIZE);
    char* command = (char*)malloc(MAX_ADDRESS_SIZE);
    char* count_path = (char*)malloc(MAX_ADDRESS_SIZE);
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
    int line_count;
    struct dirent* entry;
    struct stat path_stat;
    FILE* stage;
    FILE* line_counting;
    if(getcwd(cwd, 2000) == NULL)
    {
        fprintf(stderr, OPENNING_DIRECTORY_ERROR);
        return 1;
    }
    printf("1");
    repo_check(&exists, address, repo_address); //checking for initialization
    printf("2");
    if(!exists){
        fprintf(stderr, "Repository hass not been initialized\n");
        return 1;
    }
    strcat(repo_address, "/.neogit");
    chdir(repo_address);
    strcpy(count_path, repo_address);
    strcat(count_path, "/line_count");
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
        line_counting = fopen(count_path, "w");
        fprintf(line_counting, "%d", 1);
        fclose(line_counting);
        if(mkdir("stage", 0755) != 0)
        {
            fprintf(stderr, "Error making stage directory\n");
            return 1;
        }
    }
    line_counting = fopen(count_path, "r");
    strcpy(stage_txt, repo_address);
    strcat(stage_txt, "/stage_list.txt");
    strcpy(stage_path, repo_address);
    strcat(stage_path, "/stage"); //saving the address of the stage directory in repo

    closedir(dir);

    chdir(cwd);
    dir = opendir(".");//back in the current directory

    strcpy(argv_copy, argv[2]);

    if(strstr(argv_copy, "/") != NULL)  is_address = true;  //checking if the command is address or name
    else{is_name = true;};

    strcpy(argv[2], argv_copy);
    if(is_address) //command is address
    {
        if(strstr(argv_copy, ".neogit") != NULL) //is it in the repo or not
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
                char* path_copy = realpath(argv[2], path); //getting the absolute path
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
    while(1)
    {
        if((name = strstr(path, "/"))!= NULL)  break;
    }
    name = name + 1;
    if(stat(path, &path_stat) == -1)
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
    closedir(dir);
//first add file
if(is_file)
{
    FILE* text = fopen(stage_txt, "r");
    if(text == NULL)
    {
        fprintf(stderr, "Error openning stage_list.txt\n");
        return 1;
    }
    while(fgets(line, 1000, text) != NULL)
    {
        size_t ln = strlen(line);
        line[ln - 1] = '\0';
        if(strcmp(line, path) == 0)  //the address of the previously added file is in line
        {
            repeated = true;
            break;
        }
    }
    rewind(text);
    fclose(text);
    if(!repeated) //line_count is open in read mode
    {
        chdir(repo_address);
        dir = opendir("."); //we're in .neogit repo
        fread(&line_count, sizeof(int), 1, line_counting);
        fclose(line_counting);
        text = fopen(stage_txt, "a");
        fprintf(text, "%s  %s\n", path, name);
        fclose(text);
        line_count++;
        line_counting = fopen(count_path, "w");
        fprintf(line_counting, "%d", line_count);
        fclose(line_counting);
        closedir(dir);
        chdir(stage_path);
        dir = opendir(".");
        char* stage_copy = (char*)malloc(MAX_ADDRESS_SIZE);
        strcpy(stage_copy,stage_path);
        strcat(stage_copy, "/");
        strcat(stage_copy, name);
        sprintf(command, "cp %s %s", path, stage_copy);
        system(command);
        closedir(dir); 
    }
    if(repeated)
    {
        chdir(stage_path);
        dir = opendir(".");
        remove(line);
        char* stage_copy = (char*)malloc(MAX_ADDRESS_SIZE);
        strcpy(stage_copy,stage_path);
        strcat(stage_copy, "/");
        strcat(stage_copy, name);
        sprintf(command, "cp %s %s", path, stage_copy);
        system(command);
        closedir(dir);
    }
}




    return 0;
}