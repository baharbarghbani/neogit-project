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
#define MAX_ADDRESS_SIZE 2000
#define OPENNING_DIRECTORY_ERROR "Error openning directory\n"
char* copy;
char* stage_path;
char repo_address[2000];
char* delete;
char* address;
char* list;
char* list_copy;
char* line;
char path[2000];
char p[2000];
char* repo_copy;
char* command;
char* name2;
char* real_path;
struct dirent* entry;
bool is_valid;
bool staged;
bool exists;
bool is_valid;
bool stage_exists;
bool in_stage;
bool is_name;
bool is_address;
bool is_directory;
bool is_file;
FILE* deleted;
FILE* stage_list;
FILE* list_c;
void repo_checking()
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
        if(getcwd(address, MAX_ADDRESS_SIZE) == NULL)
        {
            fprintf(stderr,OPENNING_DIRECTORY_ERROR);
            return;
        }
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
int reset_file()
{
    // sprintf(repo_copy, "%s/%s", repo_address, name);
    // sprintf(command, "cp %s %s", path, repo_copy);
    if(remove(path) != 0)
    {
        fprintf(stderr, "Failed to remove the file from stage\n");
        return 1;
    }
    return 0;
}
// int reset_directory(char* src, char* des)
// {
//     int r;
//     stage_list = fopen(list, "r");
//     deleted = fopen(delete, "a");
//     // sprintf(command,"mkdir -p %s", des);
//     // system(command);
//     struct stat path_stat;
//     DIR* dir = opendir(src);
//     if(dir == NULL)
//     {
//         fprintf(stderr, "Falied to open the direcctory in stage when reseting\n");
//         return 1;
//     }
//     while((entry = readdir(dir)) != NULL)
//     {
//         char* new_src = (char*)malloc(MAX_ADDRESS_SIZE);
//         char* new_des = (char*)malloc(MAX_ADDRESS_SIZE);
//         sprintf(new_src, "%s/%s", src, entry->d_name);
//         sprintf(new_des, "%s/%s", des, entry->d_name);
//         if(stat(new_src, &path_stat) != -1)
//         {
//             fprintf(stderr, "Error in stat\n");
//             return 1;
//         }
//         if(S_ISDIR(path_stat.st_mode))
//         {
//             if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
//             {
//                 continue;
//             }
//             while(fgets(copy, MAX_ADDRESS_SIZE, stage_list) != NULL)
//             {
//                 sscanf(copy, "%s  %s\n", address, name);
//                 if(strcmp(name, entry->d_name) == 0)
//                 {
//                     fprintf(deleted, "%s  %s\n", address, entry->d_name);
//                     break;
//                 }
//             }
//             rewind(stage_list);
//             r = reset_directory(new_src, new_des);
//             if(r == 1)
//             {
//                 fprintf(stderr, "Error in reseting %s\n",entry->d_name);
//                 return 1;
//             }
//         }
//         else{
//             while(fgets(copy, MAX_ADDRESS_SIZE, stage_list) != NULL)
//             {
//                 sscanf(copy, "%s  %s\n", address, name);
//                 if(strcmp(name, entry->d_name) == 0)
//                 {
//                     fprintf(deleted, "%s  %s\n", address, entry->d_name);
//                     break;
//                 }
//             }
//             rewind(stage_list);
//             // sprintf(command, "cp %s %s", new_src, new_des);
//             // system(command);
//         }
//     }
//     sprintf(command, "rm -r %s", des);
//     system(command);
//     return 0;

// }
int reset(char* argv)
{
    copy = (char*)malloc(MAX_ADDRESS_SIZE);
    stage_path = (char*)malloc(MAX_ADDRESS_SIZE);
    list_copy = (char*)malloc(MAX_ADDRESS_SIZE);
    address = (char*)malloc(MAX_ADDRESS_SIZE);
    list = (char*)malloc(MAX_ADDRESS_SIZE);
    line = (char*)malloc(MAX_ADDRESS_SIZE);
    real_path = (char*)malloc(MAX_ADDRESS_SIZE);
    name2 = (char*)malloc(MAX_ADDRESS_SIZE);
    command = (char*)malloc(MAX_ADDRESS_SIZE);
    repo_copy = (char*)malloc(MAX_ADDRESS_SIZE);
    delete = (char*)malloc(MAX_ADDRESS_SIZE);
    staged = false;
    exists = false;
    stage_exists = false;
    in_stage = false;
    is_valid = false;
    is_name = false;
    is_address = false;
    is_directory = false;
    is_file = false;
    struct stat path_stat;
    repo_checking();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    if(strcat(repo_address, "/.neogit") == NULL)
    {
        fprintf(stderr, "Error in strcat /.neogit to repo_address\n");
        return 1;
    }
    DIR* dir = opendir(repo_address);
    if(dir == NULL)
    {
        fprintf(stderr, "Could not open repository\n");
        return 1;
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, "stage") != 0)
        {
            stage_exists = true;
            break;
        }
    }
    if(!stage_exists)
    {
        fprintf(stderr, "Stage area is empty\n");
        return 1;
    }
    sprintf(stage_path, "%s/%s", repo_address, "stage");
    sprintf(list, "%s/%s", repo_address, "stage_list.txt");
    stage_list = fopen(list, "r");
    if(stage_list == NULL)
    {
        fprintf(stderr, "Error openning stage list in reset\n");
        return 1;
    }
    int count = 0;
    if(access(argv, F_OK) != -1)
    {
        is_valid = true;
    }
    if(strstr(argv, "/") != NULL)   is_address = true;
    else{
        is_name = true;
    }
    if(is_name)
    {
        while(fgets(line, MAX_ADDRESS_SIZE, stage_list) != NULL)
        {
            size_t ln = strlen(line);
            line[ln-1] = '\0';
            sscanf(line, "%s  %s", address, name2);
            if(strcmp(name2, argv) == 0)
            {
                in_stage = true;
                break;
            }
        }
        rewind(stage_list);
        fclose(stage_list);
    }
    if(is_address)
    {
        while(fgets(line, MAX_ADDRESS_SIZE, stage_list) != NULL)
        {
            count++;
            size_t ln = strlen(line);
            line[ln-1] = '\0';
            realpath(argv, p);
            sscanf(line, "%s  %s", address, name2);
            if(strcmp(address, p) != 0)
            {
                in_stage = true;
                break;
            }
        }
        rewind(stage_list);
        fclose(stage_list);
    }
    sprintf(list_copy, "%s/list_copy.txt", repo_address);
    if(!in_stage)
    {
        fprintf(stderr, "%s has not been added\n", argv);
        return 1;
    }
    if(in_stage)
    {
        if((real_path = strstr(address, ".neogit")) == NULL)
        {
            fprintf(stderr, "Something went wrong\n");
            return 1;
        }
        real_path = real_path + strlen(".neogit");
        sprintf(path, "%s%s", stage_path, real_path);
        if(stat(path, &path_stat) == -1)
        {
            fprintf(stderr, "Error in stat\n");
            return 1;
        }
        if(S_ISDIR(path_stat.st_mode))  is_directory = true;
        else{is_file = true;};
        if(is_file)
        {
            int result = reset_file();
            if(result == 1)
            {
                fprintf(stderr, "Failed to reset the file\n");
                return 1;
            }
            list_c = fopen(list_copy, "w");
            if(list_c == NULL)
            {
                fprintf(stderr, "Error openning stage list copy\n");
                return 1;
            }
            int counter = 0;
            stage_list = fopen(list, "r");
            if(stage_list == NULL)
            {
                fprintf(stderr, "Error openning stage list in reset\n");
                return 1;
            }  
            counter = 0;     
            while(fgets(copy, MAX_ADDRESS_SIZE, stage_list)!= NULL)
            {
                size_t ln = strlen(copy);
                copy[ln-1] = '\0';
                if(strcmp(copy, line) == 0)
                {
                    continue;
                }
                fprintf(list_c, "%s\n", copy);
            }
            rewind(stage_list);
            fclose(stage_list);
            fclose(list_c);
            sprintf(command, "cp %s %s", list_copy, list);
            system(command);
            remove(list_copy);
            

        }
        if(is_directory)
        {
            dir = opendir(repo_address);
            if(dir == NULL)
            {
                fprintf(stderr, "Failed to open repository\n");
                return 1;
            }
            sprintf(delete, "%s/delete.txt", repo_address);
            deleted = fopen(delete, "w");
            if(deleted == NULL)
            {
                fprintf(stderr, "Failed to make delete file\n");
                return 1;
            }
            fclose(deleted); 
            char* dest = (char*)malloc(MAX_ADDRESS_SIZE);
            sprintf(dest, "%s/%s", repo_address, name2);
            list_c = fopen(list_copy, "w");
            if(list_c == NULL)
            {
                fprintf(stderr, "Error openning stage list copy\n");
                return 1;
            }
            stage_list = fopen(list, "r");
            if(stage_list == NULL)
            {
                fprintf(stderr, "Error openning stage list\n");
                return 1;
            }

            deleted = fopen(delete, "a");
            if(deleted == NULL)
            {
                fprintf(stderr, "Failed to open deleted file");
                return 1;
            }
            while(fgets(copy, MAX_ADDRESS_SIZE, stage_list) != NULL)
            {
                if(strstr(copy, name2) != NULL)
                {
                    fprintf(deleted, "%s", copy);
                }
            }
            rewind(stage_list);
            fclose(deleted);
            list_c = fopen(list_copy, "w");
            deleted = fopen(delete, "r");
            char* misery = (char*)malloc(MAX_ADDRESS_SIZE);
            while(fgets(copy, MAX_ADDRESS_SIZE, stage_list) != NULL)
            {
                bool ok = false;

                while(fgets(misery, MAX_ADDRESS_SIZE, deleted) != NULL)
                {
                    if(strcmp(misery, copy) == 0)
                    {
                        ok = true;
                    }
                }
                rewind(deleted);
                if(!ok)
                {
                    fprintf(list_c, "%s", copy);

                }
            }
            sprintf(command, "rm -r %s", path);
            system(command);
            fclose(list_c);
            fclose(deleted);
            fclose(stage_list);
            remove(delete);
            remove(list);
            sprintf(command, "cp %s %s", list_copy, list);
            system(command);
            remove(list_copy);
        }
    }
    // free(copy);
    // free(stage_path);
    // free(repo_address);
    // free(line);
    // free(list);
    // free(path);
    // free(name2);
    // free(real_path);
    // free(list_copy);
    return 0;
}