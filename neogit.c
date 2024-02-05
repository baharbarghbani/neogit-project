#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include<time.h>
#include "add.c"
#include "reset.c"
char *username;
char *email;
FILE *repo_list;
int run_init(int argc, char* argv[]);
int config(int argc, char* argv[]);
int global_config(int argc, char* argv[]);
int add(char* argv);
int reset(char* argc);
// int alias(int argc, char const* argv[]);
// int global_alias(int argc, char const* argv[]);
// int username_alias(int argc, char const* argv[]);
// int email_alias(int argc, char const* argv[]);
// int username_global_alias(int argc, char const* argv[]);
// int email_global_alias(int argc,char const* argv[]);
int main(int argc, char* argv[])
{
    char* source = (char*)malloc(MAX_ADDRESS_SIZE);
    getcwd(source, MAX_ADDRESS_SIZE);
    if (argc < 2)
    {
        fprintf(stderr, "Invalid command: too few arguements\n");
        return 1;
    }
    // char* current = (char*)malloc(2000);
    // getcwd(current, 2000);
    // char* cwd = (char*)malloc(2000);
    // struct dirent* entry;
    // char* tmp_cwd = (char*)malloc(2000);
    // char* address = (char*)malloc(2000);
    // bool exists = false;
    // if(getcwd(cwd,2000) == NULL)
    // {
    //     fprintf(stderr, "Error opeening current directory\n");
    //     return 1;
    // }
    // do
    // {
    //     DIR *dir = opendir(".");
    //     if (dir == NULL)
    //     {
    //         fprintf(stderr, "Error occured\n");
    //         return 1;
    //     }
    //     while ((entry = readdir(dir)) != NULL)
    //     {
    //         if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
    //         {
    //             getcwd(address, 2000);
    //             exists = true;
    //             break;
    //         }
    //     }
    //     if (exists)
    //         break;

    //     if (getcwd(tmp_cwd, 2000) == NULL)
    //     {
    //         fprintf(stderr, "Error occured\n");
    //         return 1;
    //     }
    //     if (strcmp(tmp_cwd, "/") != 0)
    //     {
    //         if (chdir("..") != 0)
    //         {
    //             fprintf(stderr, "Error occured\n");
    //             return 1;
    //         }
    //     }
    // closedir(dir);
    // } while (strcmp(tmp_cwd, "/") != 0);
    // bool ok = false;
    // char* line = (char*)malloc(2000); 
    // if(exists && strcmp(argv[1], "init") == 0)
    // {
    //     fprintf(stderr, "Repository has been initialized\n");
    //     return 1;
    // }
    // else if(exists && strcmp(argv[1], "init") != 0)
    // {
    //     strcat(address, "/.neogit");
    //     chdir(address);
    //     DIR* dir = opendir(".");
    //     strcat(address, "/alias.txt");
    //     FILE* file = fopen(address, "r");
    //     char* command = (char*)malloc(2000);    //argv[1] might aliased command
    //     while(fgets(line, 2000, file) != NULL) //line is from alias txt
    //     {
    //         if(strstr(line, argv[1]) != NULL) //command has been aliased locally and is saved in line
    //         {
    //             line = line + strlen(argv[1]) +2;
    //             ok = true;
    //             break;
    //         }
    //     }
    //     if(!ok) //must search in globally alised commands
    //     {
    //         FILE* globe = fopen("/home/asus/Documents/neogit/alias_global.txt", "r");
    //         if(globe == NULL)
    //         {
    //             fprintf(stderr, "Error in openning alias_global.txt\n");
    //             return 1;
    //         }
    //         while(fgets(line, 2000, globe) != NULL)
    //         {
    //             if(strstr(line, argv[1]) != NULL)
    //             {
    //                 ok = true;
    //                 line = line + strlen(argv[1]) + 2;
    //                 break;
    //             }
    //         }
    //         fclose(globe);
    //     }
    //     if(!ok)
    //     {
    //         fprintf(stderr, "Invalid command\n");
    //         return 1;
    //     }
    //     fclose(file);
    //     closedir(dir);
    // }
    // else if(!exists && strcmp(argv[1], "init") == 0)
    // {
    //     return run_init(argc,argv);
    // }
    // else if(!exists && strcmp(argv[1],"init") != 0)
    // {
    //     FILE* file = fopen("/home/asus/Documents/neogit/alias_global.txt", "r");
    //     while(fgets(line, 2000, file)!= NULL)
    //     {
    //         if(strstr(line, argv[1]) != NULL)
    //         {
    //             line = line + strlen(argv[1]) + 2;
    //             ok = true;
    //             break;
    //         }
    //     }
    //     if(!ok)
    //     {
    //         fprintf(stderr, "Invalid command\n");
    //         return 1;
    //     }
    //     fclose(file);
    // }
    email = (char *)malloc(MAX_ADDRESS_SIZE);
    username = (char *)malloc(MAX_ADDRESS_SIZE);
    // chdir(current);
    // DIR* now = opendir(".");
    if (strcmp(argv[1], "init") == 0)
    {
        return run_init(argc, argv);
    }
    else if ((strcmp(argv[1], "config") == 0))
    {
        if(argc > 2)
        {
        // if(strstr(argv[2], "alias.") != NULL)
        // {
        //     return alias(argc,argv);
        // }
        
        if(strcmp(argv[2], "--global") == 0 )//&& strstr(argv[3], "alias.") == NULL)
        {
            return global_config(argc, argv);
        }
        }
        // if(strcmp(argv[2], "--global") == 0 && strstr(argv[3], "alias.") != NULL)
        // {
        //     return global_alias(argc, argv);
        // }
        return config(argc, argv);
    }
    else if((strcmp(argv[1], "add") == 0))
    {
        if(argc < 3)
        {
            fprintf(stderr, "Wrong number of arguements for add\n");
            return 1;
        }
        if(argc > 3)
        {
            
            int result;
            if(strcmp(argv[2], "-f") == 0)
            {
                for(int i = 0; i < argc-3; i++){
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command,"neogit add %s",argv[i+3]);
                    system(command);
                }
                return 0;
            }
            for(int i = 2; i < argc; i++)
            {
                chdir(source);
                char command[MAX_ADDRESS_SIZE];
                sprintf(command,"neogit add %s",argv[i]);
                system(command);
            }
            return 0;
            
        }
        if(argc == 3)
        {

          return add(argv[2]);
        }
    }
    else if(strcmp(argv[1], "reset") == 0)
    {
        if(argc > 3)
        {
            if(strcmp(argv[2], "-f") == 0)
            {
                for(int i = 3; i < argc; i++)
                {
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command, "neogit reset %s", argv[i]);
                    system(command);
                }
                return 0;
            }
            else{
                for(int i = 2; i < argc; i++)
                {
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command, "neogit reset %s", argv[i]);
                    system(command);
                }
                return 0;
            }
        }
        if(argc == 3)
        {
            return reset(argv[2]);
        }
        else{
            fprintf(stderr, "Wrong number of arguements\n");
            return 1;
        }
    }
    // else
    // {
    //     int ln = strlen(line);
    //     line[ln-1] = '\0';
    //     if(ok)
    //     {
    //     if(strcmp(line,"init")== 0) return run_init(argc,argv);
    //     else if(strcmp(line, "config user.name") == 0)
    //     {         
    //         return username_alias(argc,argv);
    //     }
    //     else if(strcmp(line, "config user.email") == 0) return email_alias(argc,argv);
    //     else if(strcmp(line, "config --global user.name") == 0) return username_global_alias(argc,argv);
    //     else if(strcmp(line, "config --global user.email") == 0) return email_global_alias(argc,argv);
    //     else if(strcmp(line, "config alias") == 0) return alias(argc,argv);
    //     else if(strcmp(line, "config --global alias") == 0) return global_alias(argc,argv);
    //     }
    //     fprintf(stderr, "Invalid command\n");
    // }
    // closedir(now);
    // free(source);
    // free(email);
    // free(username);
    return 0;
}
int run_init(int argc, char* argv[])
{
    char cwd[200];
    char tmp_cwd[200];
    bool exists = false;
    struct dirent *entry;
    if(argc != 2)
    {
        fprintf(stderr, "Wrong number of arguemetns for init command\n");
        return 1;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        fprintf(stderr, "No current working directory in init\n");
        return 1;
    }
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            fprintf(stderr, "Error openning directory in init\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
            {
                exists = true;
                break;
            }

            if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
                return 1;

            if (strcmp(tmp_cwd, "/") != 0)
            {
                if (chdir("..") != 0)
                    return 1;
            }
        }
        closedir(dir);

    } while (strcmp(tmp_cwd, "/") != 0);
    if (chdir(cwd) != 0)
        return 1;
    repo_list = fopen("/home/asus/Documents/neogit/repolist.txt", "a");
    if (!exists)
    {
        if (mkdir(".neogit", 0755) != 0)
        {
            return 1;
        }
        strcat(cwd, "/.neogit");
        fprintf(repo_list, "%s\n", cwd);
    }
    else
    {
        fprintf(stderr, "Repository has already been initialized\n");
        return 1;
    }
    fclose(repo_list);

    return 0;
}
int config(int argc, char* argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Wrong number of arguements\n");
        return 1;
    }
    if ((strcmp(argv[2], "user.name") != 0) && strcmp(argv[2], "user.email") != 0)
    {
        fprintf(stderr, "Invalid local config command\n");
        return 1;
    }
    char *cwd = (char *)malloc(MAX_ADDRESS_SIZE);
    char *tmp_cwd = (char *)malloc(MAX_ADDRESS_SIZE);
    char *address = (char *)malloc(MAX_ADDRESS_SIZE);
    struct dirent *entry;
    bool exists = 0;
    if (getcwd(cwd, MAX_ADDRESS_SIZE) == NULL)
    {
        fprintf(stderr, "Error in configuration\n");
        return 1;
    }
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            fprintf(stderr, "Error occured\n");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
            {
                getcwd(address, MAX_ADDRESS_SIZE);
                exists = true;
                break;
            }
        }
        if (exists)
            break;

        if (getcwd(tmp_cwd, MAX_ADDRESS_SIZE) == NULL)
        {
            fprintf(stderr, "Error occured\n");
            return 1;
        }
        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
            {
                fprintf(stderr, "Error occured\n");
                return 1;
            }
        }
    closedir(dir);
    } while (strcmp(tmp_cwd, "/") != 0);
    if (exists)
    {
        strcat(address, "/.neogit");
        chdir(address);
        DIR* dir = opendir(".");
        bool found = false;
        while((entry = readdir(dir)) != NULL)
        {
            if((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "config") == 0))
            {
                found = true;
                break;

            }
        }
        if(!found)
        {
            if (mkdir("config", 0755) != 0)
            {
                fprintf(stderr, "Error occured in config\n");
                return 1;
            }   
        }
        getcwd(address, MAX_ADDRESS_SIZE);
        strcat(address, "/config");
        chdir(address);
        DIR* current = opendir(".");
        if (strcmp(argv[2], "user.name") == 0)
        {
            FILE *user;
            strcat(address, "/username");
            user = fopen(address, "w");
            if (user == NULL)
            {
                fprintf(stderr, "Error occured\n");
                return 1;
            }
            strcpy(username, argv[3]);
            fprintf(user, "%s\n", username);
            fclose(user);
        }
        else if (strcmp(argv[2], "user.email") == 0)
        {
            FILE *user;
            strcat(address, "/useremail");
            user = fopen(address, "w");
            if (user == NULL)
            {
                fprintf(stderr, "Error occured\n");
                return 1;
            }
            strcpy(email, argv[3]);
            fprintf(user, "%s\n", email);
            fprintf(stdout, "Email has been successfuly registered locally\n");
            fclose(user);
        }
        else
        {
            fprintf(stderr, "Error in local config\n");
            return 1;
        }
        closedir(dir);
        closedir(current);
    }
    else
    {
        fprintf(stderr, "Repository hasn't been initialized\n");
        return 1;
    }
    // free(cwd);
    // free(tmp_cwd);
    // free(address);
    return 0;
}
int global_config(int argc, char* argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Wrong number of arguements\n");
        return 1;
    }
    if ((strcmp(argv[3], "user.name") != 0) && (strcmp(argv[3], "user.email") != 0))
    {
        fprintf(stderr, "Invalid global config command\n");
        return 1;
    }
    char *address = "/home/asus/Documents/neogit";
    DIR *dir = opendir(address);
    struct dirent *entry;
    bool exists = false;
    char *cwd = (char *)malloc(MAX_ADDRESS_SIZE);
    repo_list = fopen("/home/asus/Documents/neogit/repolist.txt", "r");
    char* repo = (char*)malloc(MAX_ADDRESS_SIZE);
    while ((entry = readdir(dir)) != NULL)
    {
        if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "global_config") == 0))
        {
            if (getcwd(cwd, 2000) == NULL)
            {
                fprintf(stderr, "Error in global configuration\n");
                return 1;
            }
            exists = true;
            break;
        }
    }
    if (!exists)
    {
        if (mkdir("global_config", 0755) != 0)
        {
            fprintf(stderr, "Error in global configuration\n");
            return 1;
        }
    }
    closedir(dir);
    strcat(cwd, "/global_config");
    dir = opendir(cwd);
    if (strcmp(argv[3], "user.name") == 0)
    {
        strcat(cwd, "/global_name");
        FILE *file = fopen(cwd, "w");
        strcpy(username, argv[4]);
        fprintf(file, "%s\n", username);
        fclose(file);
    }
    else if (strcmp(argv[3], "user.email") == 0)
    {
        strcat(cwd, "/global_email");
        FILE *file = fopen(cwd, "w");
        strcpy(email, argv[4]);
        fprintf(file, "%s\n", email);
        fclose(file);
    }
    else
    {
        fprintf(stderr, "Error in global configuration\n");
        return 1;
    }
    closedir(dir);
   while(fgets(repo, MAX_ADDRESS_SIZE, repo_list) != NULL)
   {
        int length = strlen(repo);
        repo[length - 1] = '\0';
        chdir(repo);
        DIR* repos = opendir(".");
        struct dirent *direct;
        bool found = false;
        while((direct = readdir(repos)) != NULL)
        {
            if(direct->d_type == DT_DIR && (strcmp(direct->d_name, "config") == 0))
            {
                found = true;
                break;
            }
        }
        if(!found)
        {
            if(mkdir("config", 0755) != 0)
            {
                fprintf(stderr, "Error in global config\n");
                return 1;
            }
        }
        strcat(repo,"/config");
        chdir(repo);
        DIR* dir = opendir(".");
        if(strcmp(argv[3], "user.name") == 0)
        {
            strcat(repo, "/username");
            FILE* file = fopen(repo, "w");
            strcpy(username, argv[4]);
            fprintf(file, "%s\n", username);
            fclose(file);
        }
        else if(strcmp(argv[3], "user.email") == 0)
        {
            strcat(repo, "/useremail");
            FILE* file = fopen(repo, "w");
            strcpy(email, argv[4]);
            fprintf(file, "%s\n", email);
            fclose(file);
        }
        else{
            fprintf(stderr, "Invalid configuration command\n");
            return 1;
        }
        closedir(dir);
   }
    // free(repo);
    // free(cwd);
    // fclose(repo_list);
    return 0;
}
// int alias(int argc, char const* argv[])
// {
//     if(argc != 4)
//     {
//         fprintf(stderr, "Wrong number of arguements\n");
//         return 1;
//     }
//     struct dirent* entry;
//     bool exists = false;
//     char* cwd = (char*)malloc(2000);
//     char* tmp_cwd = (char*)malloc(2000);
//     if(getcwd(cwd, 2000) == NULL)
//     {
//         fprintf(stderr, "Error occured in alias\n");
//         return 1;
//     }
//     char* add = (char*)malloc(2000);
//     getcwd(add, 2000);
//     do// int alias(int argc, char const* argv[])
// {
//     if(argc != 4)
//     {
//         fprintf(stderr, "Wrong number of arguements\n");
//         return 1;
//     }
//     struct dirent* entry;
//     bool exists = false;
//     char* cwd = (char*)malloc(2000);
//     char* tmp_cwd = (char*)malloc(2000);
//     if(getcwd(cwd, 2000) == NULL)
//     {
//         fprintf(stderr, "Error occured in alias\n");
//         return 1;
//     }
//     char* add = (char*)malloc(2000);
//     getcwd(add, 2000);
//     do
//     {
//         DIR *dir = opendir(".");
//         getcwd(add, 2000);
//         if (dir == NULL)
//         {
//             fprintf(stderr, "Error openning directory in init\n");
//             return 1;
//         }
//         while ((entry = readdir(dir)) != NULL)
//         {
//             if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
//             {
//                 exists = true;
//                 break;
//             }
//         if (getcwd(tmp_cwd, 2000) == NULL)
//                 return 1;

//             if (strcmp(tmp_cwd, "/") != 0)
//             {
//                 if (chdir("..") != 0)
//                     return 1;
//             }
//         }
//         if(exists)
//         {
//             break;
//         }
//         closedir(dir);

//     } while (strcmp(tmp_cwd, "/") != 0);
//     if(exists)
//     {
//         strcat(add, "/.neogit");
//         chdir(add);
//     DIR* dir = opendir(".");
//     getcwd(add, sizeof(add));
//     if(dir == NULL)
//     {
//         fprintf(stderr, "Error occured in alias\n");
//         return 1;
//     }
//     strcat(add, "/alias.txt");
//     FILE *alias_file = fopen(add, "a");
//     if(alias_file == NULL)
//     
//         fprintf(stderr, "Error occured in openning alias.txt\n");
//         return 1;
//     }
//     char* name = (char*)malloc(1000);
//     sscanf(argv[2], "alias.%s", name);
//     // printf("name is %s\n", name);
//     char* command = (char*)malloc(2000);
//     strcpy(command, argv[3]);
//     command = command + strlen("neogit") + 1;
//     // printf("command is %s\n", command);
//     char* tmp = (char*)malloc(2000);
//     FILE* commands = fopen("/home/asus/Documents/neogit/commands.txt", "r");
//     if(commands == NULL)
//     {
//         fprintf(stderr, "Error occured in opeening commands file\n");
//         return 1;
//     }
//     bool ok= false;
//     while(fgets(tmp, 2000, commands) != NULL)
//     {
//         int ln = strlen(tmp);
//         tmp[ln -1] = '\0';
//         if(strcmp(command, tmp) == 0)
//         {
//             ok = true;
//             break;
//         }
//     }
//     if(ok)
//     {
//         fprintf(alias_file, "%s  %s\n", name, command);
//     }
//     else{
//         fprintf(stderr, "Given command isn't a neogit command\n");
//         return 1;
//     }
//     closedir(dir);
//     fclose(commands);
//     fclose(alias_file);
//     free(tmp);
//     free(name);
//     }
//     else{
//         fprintf(stderr, "The repository has not been initialized\n");
//         return 1;
//     }
//     free(cwd);
//     free(tmp_cwd);
//     return 0;
// }
//     {
//         DIR *dir = opendir(".");
//         getcwd(add, 2000);
//         if (dir == NULL)
//         {
//             fprintf(stderr, "Error openning directory in init\n");
//             return 1;
//         }
//         while ((entry = readdir(dir)) != NULL)
//         {
//             if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
//             {
//                 exists = true;
//                 break;
//             }
//         if (getcwd(tmp_cwd, 2000) == NULL)
//                 return 1;

//             if (strcmp(tmp_cwd, "/") != 0)
//             {
//                 if (chdir("..") != 0)
//                     return 1;
//             }
//         }
//         if(exists)
//         {
//             break;
//         }
//         closedir(dir);

//     } while (strcmp(tmp_cwd, "/") != 0);
//     if(exists)
//     {
//         strcat(add, "/.neogit");
//         chdir(add);
//     DIR* dir = opendir(".");
//     getcwd(add, sizeof(add));
//     if(dir == NULL)
//     {
//         fprintf(stderr, "Error occured in alias\n");
//         return 1;
//     }
//     strcat(add, "/alias.txt");
//     FILE *alias_file = fopen(add, "a");
//     if(alias_file == NULL)
//     
//         fprintf(stderr, "Error occured in openning alias.txt\n");
//         return 1;
//     }
//     char* name = (char*)malloc(1000);
//     sscanf(argv[2], "alias.%s", name);
//     // printf("name is %s\n", name);
//     char* command = (char*)malloc(2000);
//     strcpy(command, argv[3]);
//     command = command + strlen("neogit") + 1;
//     // printf("command is %s\n", command);
//     char* tmp = (char*)malloc(2000);
//     FILE* commands = fopen("/home/asus/Documents/neogit/commands.txt", "r");
//     if(commands == NULL)
//     {
//         fprintf(stderr, "Error occured in opeening commands file\n");
//         return 1;
//     }
//     bool ok= false;
//     while(fgets(tmp, 2000, commands) != NULL)
//     {
//         int ln = strlen(tmp);
//         tmp[ln -1] = '\0';
//         if(strcmp(command, tmp) == 0)
//         {
//             ok = true;
//             break;
//         }
//     }
//     if(ok)
//     {
//         fprintf(alias_file, "%s  %s\n", name, command);
//     }
//     else{
//         fprintf(stderr, "Given command isn't a neogit command\n");
//         return 1;
//     }
//     closedir(dir);
//     fclose(commands);
//     fclose(alias_file);
//     free(tmp);
//     free(name);
//     }
//     else{
//         fprintf(stderr, "The repository has not been initialized\n");
//         return 1;
//     }
//     free(cwd);
//     free(tmp_cwd);
//     return 0;
// }
// int global_alias(int argc, char const* argv[])
// {
//     if(argc != 5)
//     {
//         fprintf(stderr, "Wrong number of arguements in alias global\n");
//         return 0;
//     }
//     FILE *global = fopen("/home/asus/Documents/neogit/alias_global.txt", "a");
//     if(global == NULL)
//     {
//         fprintf(stderr, "Error in global alias\n");
//         return 1;
//     }
//     char* lines = (char*)malloc(2000);
//     FILE* commands = fopen("/home/asus/Documents/neogit/commands.txt", "r");
//     if(commands == NULL)
//     {
//         fprintf(stderr, "Error in global alias\n");
//         return 1;
//     }
//     char* name = (char*)malloc(2000);
//     char* command = (char*)malloc(2000);
//     sscanf(argv[3], "alias.%s", name);
//     strcpy(command, argv[4]);
//     command = command + strlen("neogit") + 1;
//     bool exist = false;
//     while(fgets(lines, 1000, commands) != NULL)
//     {
//         int ln = strlen(lines);
//         lines[ln - 1] = '\0';
//         if(strcmp(command, lines) == 0)
//         {
//             exist = true;
//             break;
//         }
//     }
//     char* address = (char*)malloc(2000);
//     if(exist)
//     {
//         fprintf(global, "%s  %s\n", name, command);
//     }
//     else{
//         fprintf(stderr, "The given command in not a neogit command\n");
//         return 1;
//     }

//     fclose(global);
//     fclose(commands);
//     free(lines);
//     return 0;
// }
// int username_alias(int argc, char const* argv[])
// {
//     char *cwd = (char *)malloc(2000);
//     char *tmp_cwd = (char *)malloc(2000);
//     char *address = (char *)malloc(2000);
//     struct dirent *entry;
//     bool exists = 0;
//     if (getcwd(cwd, 2000) == NULL)
//     {
//         fprintf(stderr, "Error in configuration\n");
//         return 1;
//     }
//     do
//     {
//         DIR *dir = opendir(".");
//         if (dir == NULL)
//         {
//             fprintf(stderr, "Error occured\n");
//             return 1;
//         }
//         while ((entry = readdir(dir)) != NULL)
//         {
//             if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
//             {
//                 getcwd(address, 2000);
//                 exists = true;
//                 break;
//             }
//         }
//         if (exists)
//             break;

//         if (getcwd(tmp_cwd, 2000) == NULL)
//         {
//             fprintf(stderr, "Error occured\n");
//             return 1;
//         }
//         if (strcmp(tmp_cwd, "/") != 0)
//         {
//             if (chdir("..") != 0)
//             {
//                 fprintf(stderr, "Error occured\n");
//                 return 1;
//             }
//         }
//     closedir(dir);
//     } while (strcmp(tmp_cwd, "/") != 0);
//     if (exists)
//     {
//         strcat(address, "/.neogit");
//         chdir(address);
//         DIR* dir = opendir(".");
//         bool found = false;
//         while((entry = readdir(dir)) != NULL)
//         {
//             if((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "config") == 0))
//             {
//                 found = true;
//                 break;
//             }
//         }
//         if(!found)
//         {
//             if (mkdir("config", 0755) != 0)
//             {
//                 fprintf(stderr, "Error occured in config\n");
//                 return 1;
//             }   
//         }
//         strcat(address, "/config");
//         chdir(address);
//         closedir(dir);
//         dir = opendir(".");
//         strcat(address, "/username");
//         FILE* file = fopen(address, "w");
//         fprintf(file, "%s", argv[2]);
//         fclose(file);
//         closedir(dir);
//     }
//     return 0;
// }
// int email_alias(int argc, char const* argv[])
// {
//     char *cwd = (char *)malloc(2000);
//     char *tmp_cwd = (char *)malloc(2000);
//     char *address = (char *)malloc(2000);
//     struct dirent *entry;
//     bool exists = 0;
//     if (getcwd(cwd, 2000) == NULL)
//     {
//         fprintf(stderr, "Error in configuration\n");
//         return 1;
//     }
//     do
//     {
//         DIR *dir = opendir(".");
//         if (dir == NULL)
//         {
//             fprintf(stderr, "Error occured\n");
//             return 1;
//         }
//         while ((entry = readdir(dir)) != NULL)
//         {
//             if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, ".neogit") == 0))
//             {
//                 getcwd(address, 2000);
//                 exists = true;
//                 break;
//             }
//         }
//         if (exists)
//             break;

//         if (getcwd(tmp_cwd, 2000) == NULL)
//         {
//             fprintf(stderr, "Error occured\n");
//             return 1;
//         }
//         if (strcmp(tmp_cwd, "/") != 0)
//         {
//             if (chdir("..") != 0)
//             {
//                 printf("4");
//                 fprintf(stderr, "Error occured\n");
//                 return 1;
//             }
//         }
//     closedir(dir);
//     } while (strcmp(tmp_cwd, "/") != 0);

//     if (exists)
//     {
//         strcat(address, "/.neogit");
//         chdir(address);
//         DIR* dir = opendir(".");
//         bool found = false;
//         while((entry = readdir(dir)) != NULL)
//         {
//             if((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "config") == 0))
//             {
//                 found = true;
//                 break;

//             }
//         }
//         if(!found)
//         {
//             if (mkdir("config", 0755) != 0)
//             {
//                 fprintf(stderr, "Error occured in config\n");
//                 return 1;
//             }   
//         }
//         strcat(address, "/config");
//         chdir(address);
//         closedir(dir);
//         dir = opendir(".");
//         strcat(address, "/useremail");
//         FILE* file = fopen(address, "w");
//         fprintf(file, "%s", argv[2]);
//         fclose(file);
//         closedir(dir);
//     }
//     return 0;
//}
// int username_global_alias(int argc, char const* argv[])
// {
//     char *address = "/home/asus/Documents/neogit";
//     chdir(address);
//     DIR *dir = opendir(".");
//     if(dir == NULL)
//     {
//         fprintf(stderr, "Error in openning neogit directory\n");
//         return 1;
//     }
//     struct dirent *entry;
//     bool exists = false;
//     char *cwd = (char *)malloc(2000);
//     repo_list = fopen("/home/asus/Documents/neogit/repolist.txt", "r");
//     char* repo = (char*)malloc(2000);
//     while ((entry = readdir(dir)) != NULL)
//     {
//         if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "global_config") == 0))
//         {
//             if (getcwd(cwd, 2000) == NULL)
//             {
//                 fprintf(stderr, "Error in global configuration\n");
//                 return 1;
//             }
//             exists = true;
//             break;
//         }
//     }
//     if (!exists)
//     {
//         if (mkdir("global_config", 0755) != 0)
//         {
//             fprintf(stderr, "Error in global configuration\n");
//             return 1;
//         }
//     }
//     strcat(cwd, "/global_config");
//     chdir(cwd);
//     dir = opendir(".");
//         strcat(cwd, "/global_name");
//         FILE *file = fopen(cwd, "w");
//         fprintf(file, "%s\n", argv[2]);
//         fclose(file);

//     closedir(dir);
//    while(fgets(repo, 1000, repo_list) != NULL)
//    {
//         int length = strlen(repo);
//         repo[length - 1] = '\0';
//         chdir(repo);
//         DIR* repos = opendir(".");
//         struct dirent *direct;
//         bool found = false;
//         while((direct = readdir(repos)) != NULL)
//         {
//             if(direct->d_type == DT_DIR && (strcmp(direct->d_name, "config") == 0))
//             {
//                 found = true;
//                 break;
//             }
//         }
//         if(!found)
//         {
//             if(mkdir("config", 0755) != 0)
//             {
//                 fprintf(stderr, "Error in global config\n");
//                 return 1;
//             }
//         }
//         strcat(repo,"/config");
//         chdir(repo);
//         DIR* dir = opendir(".");
//             strcat(repo, "/username");
//             FILE* file = fopen(repo, "w");
//             fprintf(file, "%s\n", argv[2]);
//             fclose(file);   
//         closedir(dir);
//    }
//     free(repo);
//     fclose(repo_list);
//     return 0;
// }
// int email_global_alias(int argc, char const* argv[])
// {
//     char *address = "/home/asus/Documents/neogit";
//     chdir(address);
//     DIR *dir = opendir(".");
//     struct dirent *entry;
//     bool exists = false;
//     char *cwd = (char *)malloc(2000);
//     repo_list = fopen("/home/asus/Documents/neogit/repolist.txt", "r");
//     char* repo = (char*)malloc(2000);
//     while ((entry = readdir(dir)) != NULL)
//     {
//         if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "global_config") == 0))
//         {
//             if (getcwd(cwd, 2000) == NULL)
//             {
//                 fprintf(stderr, "Error in global configuration\n");
//                 return 1;
//             }
//             exists = true;
//             break;
//         }
//     }
//     if (!exists)
//     {
//         if (mkdir("global_config", 0755) != 0)
//         {
//             fprintf(stderr, "Error in global configuration\n");
//             return 1;
//         }
//     }
//     closedir(dir);
//     strcat(cwd, "/global_config");
//     chdir(cwd);
//     dir = opendir(".");
//         strcat(cwd, "/global_email");
//         FILE *file = fopen(cwd, "w");
//         fprintf(file, "%s\n", argv[2]);
//         fclose(file);

//     closedir(dir);
//    while(fgets(repo, 1000, repo_list) != NULL)
//    {
//         int length = strlen(repo);
//         repo[length - 1] = '\0';
//         chdir(repo);
//         DIR* repos = opendir(".");
//         struct dirent *direct;
//         bool found = false;
//         while((direct = readdir(repos)) != NULL)
//         {
//             if(direct->d_type == DT_DIR && (strcmp(direct->d_name, "config") == 0))
//             {
//                 found = true;
//                 break;
//             }
//         }
//         if(!found)
//         {
//             if(mkdir("config", 0755) != 0)
//             {
//                 fprintf(stderr, "Error in global config\n");
//                 return 1;
//             }
//         }
//         strcat(repo,"/config");
//         chdir(repo);
//         DIR* dir = opendir(".");
//             strcat(repo, "/useremail");
//             FILE* file = fopen(repo, "w");
//             fprintf(file, "%s\n", argv[2]);
//             fclose(file);   
//         closedir(dir);
//    }
//     free(repo);
//     fclose(repo_list);
//     return 0;
// }
                