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
#define OPENNING_DIRECTORY_ERROR "Error openning directory\n"
#define MAX_ADDRESS_SIZE 2000
bool exists;
char* address;
char* repo_address;
char* cwd;
char* tmp_cwd;
char* argv_copy;
char* path;
char* stage_path;
char* stage_txt;
char* line;
char** name;
char* command;
char* count_path;
char* path_copy;
bool stage_exists;
bool found;
bool repeated;
bool is_in_repo;
bool is_file;
bool is_address;
bool is_name;
bool is_directory;
bool is_valid;
int line_count;
int r;
struct dirent* entry;
struct stat path_stat;
FILE* stage;
FILE* text;
void repo_check()
{
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
                getcwd(repo_address,MAX_ADDRESS_SIZE);
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
int add_dir(char* src, char* dest)
{
    DIR* dir = opendir(src);
    text = fopen(stage_txt, "a");
    if(text == NULL)
    {
        fprintf(stderr, "Error in openning stage lis\n");
        return 1;
    }
    if(dir == NULL)
    {
        fprintf(stderr, "Error in add_dir\n");
        return 1;
    }
    sprintf(command,"mkdir -p %s", dest);
    system(command);
    // if(mkdir(dest, 0755) != 0)
    // {
    //     fprintf(stderr, "Error making directory\n");
    //     return 1;
    // }
    while((entry = readdir(dir))!= NULL)
    {
        char* new_src = (char*)malloc(MAX_ADDRESS_SIZE);
        char* new_des = (char*)malloc(MAX_ADDRESS_SIZE);
        sprintf(new_src, "%s/%s", src, entry->d_name);
        sprintf(new_des, "%s/%s", dest, entry->d_name);
        if(stat(new_src, &path_stat) == -1)
        {
            fprintf(stderr, "Error in stat\n");
            return 1;
        }
        if(S_ISDIR(path_stat.st_mode)) {
            if((strcmp(entry->d_name, ".")== 0) || (strcmp(entry->d_name, "..") == 0))
            {
                continue;
            }
            fprintf(text, "%s  %s\n",new_src, entry->d_name);
            if(add_dir(new_src,new_des) != 0)
            {
                
                fprintf(stderr, "Error occured in recursive\n");
                continue;
            }
            line_count++;
        }
        else{
            line_count++;
            fprintf(text, "%s  %s\n",new_src, entry->d_name);
            sprintf(command, "cp %s %s", new_src, new_des);
            system(command);
        }
    } closedir(dir);
return 0;
}
int add(char* argv)
{
    r = 0;
    address = (char*)malloc(MAX_ADDRESS_SIZE);  //used
    repo_address = (char*)malloc(MAX_ADDRESS_SIZE); //used
    cwd = (char*)malloc(MAX_ADDRESS_SIZE); //used
    tmp_cwd = (char*)malloc(MAX_ADDRESS_SIZE);
    argv_copy = (char*)malloc(MAX_ADDRESS_SIZE); //used
    path = (char*)malloc(MAX_ADDRESS_SIZE); //used
    stage_path = (char*)malloc(MAX_ADDRESS_SIZE); //used
    stage_txt = (char*)malloc(MAX_ADDRESS_SIZE); //used
    line = (char*)malloc(MAX_ADDRESS_SIZE);
    name = (char**)malloc(100);
    for(int i = 0; i < 100; i++)
    {
        name[i]= (char*)malloc(100);
    }
    command = (char*)malloc(MAX_ADDRESS_SIZE);
    count_path = (char*)malloc(MAX_ADDRESS_SIZE);
    path_copy = (char*)malloc(MAX_ADDRESS_SIZE);
    stage_exists = false;
    found = false;
    repeated = false;
    is_in_repo = false;
    is_file = false;
    is_address = false;
    is_name = false;
    is_directory = false;
    is_valid = false;
    if(getcwd(cwd, 2000) == NULL)
    {
        fprintf(stderr, OPENNING_DIRECTORY_ERROR);
        return 1;
    }
    strcpy(tmp_cwd,cwd);
    repo_check(); //checking for initialization
    if(!exists){
        fprintf(stderr, "Repository hass not been initialized\n");
        return 1;
    }
    strcat(repo_address, "/.neogit");
    chdir(repo_address);
    // strcpy(count_path, repo_address);
    // strcat(count_path, "/line_count");
    // printf("%s", count_path);
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
        // line_counting = fopen(count_path, "w");
        // fprintf(line_counting, "%d", 1);
        // fclose(line_counting);
        if(mkdir("stage", 0755) != 0)
        {
            fprintf(stderr, "Error making stage directory\n");
            return 1;
        }
    }
    // line_counting = fopen(count_path, "r");
    strcpy(stage_txt, repo_address);
    strcat(stage_txt, "/stage_list.txt");
    text = fopen(stage_txt, "a");
    fclose(text); 
    strcpy(stage_path, repo_address);
    strcat(stage_path, "/stage"); //saving the address of the stage directory in repo

    closedir(dir);

    chdir(cwd);
    dir = opendir(".");//back in the current directory
    strcpy(argv_copy, argv);
    if(strstr(argv_copy, "/") != NULL)  is_address = true;  //checking if the command is address or name
    else{is_name = true;};

    strcpy(argv, argv_copy);
    if(is_address) //command is address
    {
        if(access(argv, F_OK) != -1) //is the address valid?
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
                char* path_copy = realpath(argv, path); //getting the absolute path
        }
        if(strstr(path_copy, repo_address) != NULL) //is it in the repo or not
        {
            is_in_repo = true;
        }
        if(!is_in_repo) //the address isn't in the repo
        {
            fprintf(stderr, "The given address does not exist in repository\n");
            return 1;
        }
    }
    
    closedir(dir);
    if(is_name) //command is name
    {    
        chdir(cwd);
        dir = opendir(".");                      //now we're in the current working directory
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, argv) == 0)
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            fprintf(stderr, "%s does not exist\n", argv);
            return 1;
        }
        else if(found)
        { 
            
           strcpy(path, cwd);
           strcat(path, "/");
           strcat(path, argv);  //absolute path is done   
           if(strstr(path, repo_address) == NULL)
           {
            fprintf(stderr, "The given name does not exist in repository\n");
            return 1;
           }
        }
    }
    strcpy(path_copy,path);
    name[0] = strtok(path_copy, "/");
    int word_count = 0;
    while(name[word_count] != NULL)
    {
        word_count++;
        name[word_count] = strtok(NULL, "/");
    }
    word_count--;
    if(stat(path, &path_stat) == -1)
    {
        fprintf(stderr, "Error in stat\n");
        return 1;
    }
    //finding out the type of the file
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
    text = fopen(stage_txt, "r");
    if(text == NULL)
    {
        fprintf(stderr, "Error openning stage_list.txt\n");
        return 1;
    }
    while(fgets(line, 1000, text) != NULL)
    {
        size_t ln = strlen(line);
        line[ln - 1] = '\0';
        if(strstr(line, path) != 0)  //the address of the previously added file is in line
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
        // line_count = getw(line_counting);
        // fclose(line_counting);
        text = fopen(stage_txt, "a");
        fprintf(text, "%s  %s\n", path, name[word_count]);
        fclose(text);
        // line_count++;
        // line_counting = fopen(count_path, "w");
        // fprintf(line_counting, "%d", line_count);
        // fclose(line_counting);
        closedir(dir);
        chdir(stage_path);
        dir = opendir(".");
        char* stage_copy = (char*)malloc(MAX_ADDRESS_SIZE);
        strcpy(stage_copy,stage_path);
        strcat(stage_copy, "/");
        strcat(stage_copy, name[word_count]);
        sprintf(command, "cp %s %s", path, stage_copy);
        system(command);
        closedir(dir); 
    }
    if(repeated)
    {
        chdir(stage_path);
        dir = opendir(".");
        // remove(line);
        char* stage_copy = (char*)malloc(MAX_ADDRESS_SIZE);
        strcpy(stage_copy,stage_path);
        strcat(stage_copy, "/");
        strcat(stage_copy, name[word_count]);
        remove(stage_copy);
        sprintf(command, "cp %s %s", path, stage_copy);
        system(command);
        closedir(dir);
    }
}
if(is_directory)
{
    // line_count = getw(line_counting);
    // printf("%d", line_count);
    // fclose(line_counting);
    // line_counting = fopen(count_path, "w");
    text = fopen(stage_txt, "r");
    char* str = (char*)malloc(MAX_ADDRESS_SIZE);
    str = strstr(path, ".neogit/");
    str = str + strlen(".neogit/");

    while(fgets(line, 1000, text) != NULL)
    {
        if(strstr(line, path) != NULL)
        {
            repeated = true;
            break;
        }
    }
    fclose(text);
    char* in_stage_path = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(in_stage_path, "%s/%s", stage_path, str);
    text = fopen(stage_txt, "a");
    fprintf(text, "%s  %s\n", path, name[word_count]);
    fclose(text);
    int result;
    if(repeated)
    {
        sprintf(command, "rm -r %s", in_stage_path);
        system(command);
        result = add_dir(path, in_stage_path);
        if(result == 1)
        {
            fprintf(stderr, "Error in adding directory\n");
            return 1;
        }
    }
    else{
        result = add_dir(path, in_stage_path);
        if(result == 1)
        {
            fprintf(stderr, "Error in adding directory\n");
            return 1;
        }
    }                       
}
    return 0;
}