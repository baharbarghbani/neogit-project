#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include "add.c"
#include "reset.c"
char *username;
char *email;
char *address;
char *name_path;
char *stage_path;
char *email_path;
char *global_path;
char *current_branch;
char* branch_path;
FILE *log_file;
char *log_path;
FILE *repo_list;
FILE *branches;
FILE *commit_id;
FILE *info;
char *commit_path;
char *commit_info;
// char stage_path[MAX_ADDRESS_SIZE];
char *branch_list;
char *id_path;
int id;
char *repo;
int run_init(int argc, char *argv[]);
int config(int argc, char *argv[]);
int global_config(int argc, char *argv[]);
int add(char *argv);
int reset(char *argc);
int commit(char *argv, int argc);
int nlog();
int branch(char* argv[], int argc);
// int alias(int argc, char const* argv[]);
// int global_alias(int argc, char const* argv[]);
// int username_alias(int argc, char const* argv[]);
// int email_alias(int argc, char const* argv[]);
// int username_global_alias(int argc, char const* argv[]);
// int email_global_alias(int argc,char const* argv[]);
void repo_check2()
{
    struct dirent *entry;
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            fprintf(stderr, OPENNING_DIRECTORY_ERROR);
            return;
        }
        if (getcwd(address, MAX_ADDRESS_SIZE) == NULL)
        {
            fprintf(stderr, OPENNING_DIRECTORY_ERROR);
            return;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_DIR && (strcmp(entry->d_name, ".neogit") == 0))
            {
                getcwd(repo, MAX_ADDRESS_SIZE);
                exists = true;
                break;
            }
        }
        if (exists)
            break;
        if (getcwd(address, MAX_ADDRESS_SIZE) == NULL)
        {
            fprintf(stderr, OPENNING_DIRECTORY_ERROR);
            return;
        }
        if (strcmp(address, "/") != 0)
        {
            if (chdir("..") != 0)
            {
                fprintf(stderr, OPENNING_DIRECTORY_ERROR);
                return;
            }
        }
        closedir(dir);
    } while (strcmp(address, "/") != 0);
    return;
}
int main(int argc, char *argv[])
{
    char *source = (char *)malloc(MAX_ADDRESS_SIZE);
    getcwd(source, MAX_ADDRESS_SIZE);
    chdir(source);
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
    branch_path = (char*)malloc(MAX_ADDRESS_SIZE);
    stage_path = (char *)malloc(MAX_ADDRESS_SIZE);
    address = (char *)malloc(MAX_ADDRESS_SIZE);
    current_branch = (char *)malloc(MAX_ADDRESS_SIZE);
    global_path = (char *)malloc(MAX_ADDRESS_SIZE);
    email = (char *)malloc(MAX_ADDRESS_SIZE);
    username = (char *)malloc(MAX_ADDRESS_SIZE);
    repo = (char *)malloc(MAX_ADDRESS_SIZE);
    name_path = (char *)malloc(MAX_ADDRESS_SIZE);
    email_path = (char *)malloc(MAX_ADDRESS_SIZE);
    commit_path = (char *)malloc(MAX_ADDRESS_SIZE);
    commit_info = (char *)malloc(MAX_ADDRESS_SIZE);
    branch_list = (char *)malloc(MAX_ADDRESS_SIZE);
    id_path = (char *)malloc(MAX_ADDRESS_SIZE);
    log_path = (char*)malloc(MAX_ADDRESS_SIZE);
    // chdir(current);
    // DIR* now = opendir(".");
    if (strcmp(argv[1], "init") == 0)
    {
        return run_init(argc, argv);
    }
    else if ((strcmp(argv[1], "config") == 0))
    {
        if (argc > 2)
        {
            // if(strstr(argv[2], "alias.") != NULL)
            // {
            //     return alias(argc,argv);
            // }

            if (strcmp(argv[2], "--global") == 0) //&& strstr(argv[3], "alias.") == NULL)
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
    else if ((strcmp(argv[1], "add") == 0))
    {
        if (argc < 3)
        {
            fprintf(stderr, "Wrong number of arguements for add\n");
            return 1;
        }
        if (argc > 3)
        {

            int result;
            if (strcmp(argv[2], "-f") == 0)
            {
                for (int i = 0; i < argc - 3; i++)
                {
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command, "neogit add %s", argv[i + 3]);
                    system(command);
                }
                return 0;
            }
            for (int i = 2; i < argc; i++)
            {
                chdir(source);
                char command[MAX_ADDRESS_SIZE];
                sprintf(command, "neogit add %s", argv[i]);
                system(command);
            }
            return 0;
        }
        if (argc == 3)
        {

            return add(argv[2]);
        }
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        if (argc > 3)
        {
            if (strcmp(argv[2], "-f") == 0)
            {
                for (int i = 3; i < argc; i++)
                {
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command, "neogit reset %s", argv[i]);
                    system(command);
                }
                return 0;
            }
            else
            {
                for (int i = 2; i < argc; i++)
                {
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command, "neogit reset %s", argv[i]);
                    system(command);
                }
                return 0;
            }
        }
        if (argc == 3)
        {
            return reset(argv[2]);
        }
        else
        {
            fprintf(stderr, "Wrong number of arguements\n");
            return 1;
        }
    }
    else if (strcmp(argv[1], "commit") == 0 && strcmp(argv[2], "-m") == 0)
    {
        return commit(argv[3], argc);
    }
    else if(strcmp(argv[1], "log") == 0)
    {
        if(argc > 2)
        {
            fprintf(stderr, "Wrong number of arguements\n");
            return 1;
        }
        return nlog();
    }
    else if(strcmp(argv[1], "branch") == 0)
    {
        return branch(argv, argc);
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
int run_init(int argc, char *argv[])
{
    char cwd[200];
    char tmp_cwd[200];
    bool exists = false;
    struct dirent *entry;
    if (argc != 2)
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
        sprintf(repo, "%s/.neogit", cwd);
        fprintf(repo_list, "%s\n", repo);
        char command[2 * MAX_ADDRESS_SIZE];
        char master[1000];
        sprintf(master, "%s/master", repo);
        sprintf(command, "mkdir %s", master);
        system(command);
        sprintf(branch_list, "%s/.branches.txt", repo);
        branches = fopen(branch_list, "w");
        if (branches == NULL)
        {
            fprintf(stderr, "Error making branch list\n");
            return 1;
        }
        fprintf(branches, "master\n");
        fclose(branches);
        sprintf(current_branch, "%s/.current_branch.txt", repo);
        FILE *cb = fopen(current_branch, "w");
        if (cb == NULL)
        {
            fprintf(stderr, "Failed to open current branch file\n");
            return 1;
        }
        fprintf(cb, "master");
        fclose(cb);
        sprintf(id_path, "%s/.id.txt", repo);
        commit_id = fopen(id_path, "w");
        if (commit_id == NULL)
        {
            fprintf(stderr, "Error making commit id file\n");
            return 1;
        }
        id = 1;
        fprintf(commit_id, "%d", id);
        fclose(commit_id);
        sprintf(global_path, "/home/asus/Documents/neogit/global_config");
        DIR *dir = opendir("/home/asus/Documents/neogit");
        if (dir == NULL)
        {
            fprintf(stderr, "Error openning code file\n");
            return 1;
        }
        struct dirent *entry;
        bool config_done = false;
        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, "global_config") == 0)
            {
                config_done = true;
                break;
            }
        }
        closedir(dir);
        if (config_done)
        {
            dir = opendir(global_path);
            if (dir == NULL)
            {
                fprintf(stderr, "Failed to open global config folder\n");
                return 1;
            }
            bool email_ok = false;
            bool name_ok = false;
            while ((entry = readdir(dir)) != NULL)
            {
                if (strcmp(entry->d_name, "global_email") == 0)
                    email_ok = true;
                if (strcmp(entry->d_name, "global_name") == 0)
                    name_ok = true;
            }
            closedir(dir);
            dir = opendir(repo);
            if (dir == NULL)
            {
                fprintf(stderr, "Failed to open repository\n");
                return 1;
            }
            if (!email_ok)
            {
                fprintf(stdout, "Please configure your email\n");
            }
            if (!name_ok)
            {
                fprintf(stdout, "Please configure your user name\n");
            }
            if (name_ok || email_ok)
            {
                char *config_path = (char *)malloc(MAX_ADDRESS_SIZE);
                sprintf(config_path, "%s/config", repo);
                sprintf(command, "mkdir %s", config_path);
                system(command);
                closedir(dir);
                dir = opendir(config_path);
                char *name_config = (char *)malloc(MAX_ADDRESS_SIZE);
                char *email_config = (char *)malloc(MAX_ADDRESS_SIZE);
                sprintf(name_config, "%s/username", config_path);
                sprintf(email_config, "%s/useremail", config_path);
                if (name_ok)
                {
                    FILE *n = fopen(name_config, "w");
                    if (n == NULL)
                    {
                        fprintf(stderr, "Failed to build config name\n");
                        return 1;
                    }
                    char *gn = (char *)malloc(MAX_ADDRESS_SIZE);
                    sprintf(gn, "/home/asus/Documents/neogit/global_config/global_name");
                    FILE *fptr = fopen(gn, "r");
                    if (fptr == NULL)
                    {
                        fprintf(stderr, "Failed to open global config name\n");
                        return 1;
                    }
                    char un[100];
                    fgets(un, 100, fptr);
                    fprintf(n, "%s", un);
                    fclose(n);
                    fclose(fptr);
                }
                if (email_ok)
                {
                    FILE *e = fopen(email_config, "w");
                    if (e == NULL)
                    {
                        fprintf(stderr, "Failed to build email config\n");
                        return 1;
                    }
                    char *ge = (char *)malloc(MAX_ADDRESS_SIZE);
                    sprintf(ge, "/home/asus/Documents/neogit/global_config/global_email");
                    FILE *fptr2 = fopen(ge, "r");
                    if (fptr2 == NULL)
                    {
                        fprintf(stderr, "Failed to open global email file\n");
                        return 1;
                    }
                    char ue[100];
                    fgets(ue, 100, fptr2);
                    fprintf(e, "%s", ue);
                    fclose(fptr2);
                    fclose(e);
                }
            }
        }
        if (!config_done)
        {
            fprintf(stderr, "Please configure you email and username\n");
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Repository has already been initialized\n");
        return 1;
    }
    fclose(repo_list);

    return 0;
}
int config(int argc, char *argv[])
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
        DIR *dir = opendir(".");
        bool found = false;
        while ((entry = readdir(dir)) != NULL)
        {
            if ((entry->d_type == DT_DIR) && (strcmp(entry->d_name, "config") == 0))
            {
                found = true;
                break;
            }
        }
        if (!found)
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
        DIR *current = opendir(".");
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
int global_config(int argc, char *argv[])
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
    address = "/home/asus/Documents/neogit";
    DIR *dir = opendir(address);
    if (dir == NULL)
    {
        fprintf(stderr, "failed to open code folder\n");
        return 1;
    }
    struct dirent *entry;
    exists = false;
    char *cwd = (char *)malloc(MAX_ADDRESS_SIZE);
    repo_list = fopen("/home/asus/Documents/neogit/repolist.txt", "r");
    if (repo_list == NULL)
    {
        fprintf(stderr, "Failed to open repo list\n");
        return 1;
    }
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
    getcwd(cwd, MAX_ADDRESS_SIZE);
    strcat(cwd, "/global_config");
    dir = opendir(cwd);
    if (dir == NULL)
    {
        fprintf(stderr, "Failed to open global config folder\n");
        return 1;
    }
    if (strcmp(argv[3], "user.name") == 0)
    {
        strcat(cwd, "/global_name");
        FILE *file = fopen(cwd, "w");
        if (file == NULL)
        {
            fprintf(stderr, "Error openning global user name file\n");
            return 1;
        }
        strcpy(username, argv[4]);
        fprintf(file, "%s\n", username);
        fclose(file);
    }
    else if (strcmp(argv[3], "user.email") == 0)
    {
        strcat(cwd, "/global_email");
        FILE *file = fopen(cwd, "w");
        if (file == NULL)
        {
            fprintf(stderr, "Error openning global user email\n");
            return 1;
        }
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
    while (fgets(repo, MAX_ADDRESS_SIZE, repo_list) != NULL)
    {
        int length = strlen(repo);
        repo[length - 1] = '\0';
        chdir(repo);
        DIR *repos = opendir(".");
        struct dirent *direct;
        bool found = false;
        while ((direct = readdir(repos)) != NULL)
        {
            if (direct->d_type == DT_DIR && (strcmp(direct->d_name, "config") == 0))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (mkdir("config", 0755) != 0)
            {
                fprintf(stderr, "Error in global config\n");
                return 1;
            }
        }
        strcat(repo, "/config");
        chdir(repo);
        fprintf(stdout, "repo is %s\n", repo);
        DIR *dir = opendir(".");
        if (strcmp(argv[3], "user.name") == 0)
        {
            strcat(repo, "/username");
            FILE *file = fopen(repo, "w");
            if (file == NULL)
            {
                fprintf(stderr, "Failed to open %s\n", repo);
                return 1;
            }
            strcpy(username, argv[4]);
            fprintf(file, "%s\n", username);
            fclose(file);
        }
        else if (strcmp(argv[3], "user.email") == 0)
        {
            strcat(repo, "/useremail");
            FILE *file = fopen(repo, "w");
            if (file == NULL)
            {
                fprintf(stderr, "Failed to open %s\n", repo);
                return 1;
            }
            strcpy(email, argv[4]);
            fprintf(file, "%s\n", email);
            fclose(file);
        }
        else
        {
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
int branch(char* argv[], int argc)
{
    if(argc >3)
    {
        fprintf(stderr, "Wrong number of arguements for branch\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    char c[MAX_ADDRESS_SIZE];
    if(strcpy(c, repo) == NULL)
    {
        fprintf(stderr, "Failed to copy repo address to c\n");
        return 1;
    }
    strcat(repo, "/.neogit");
    sprintf(branch_list, "%s/.branches.txt", repo);
    branches = fopen(branch_list, "r");
    if(branches == NULL)
    {
        fprintf(stderr, "Failed to open branches file\n");
        return 1;
    }
    char r[MAX_ADDRESS_SIZE];
    char t[MAX_ADDRESS_SIZE];
    sprintf(current_branch, "%s/.current_branch.txt", repo);
    FILE* file = fopen(current_branch, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open current branch file\n");
        return 1;
    }
    char b[MAX_ADDRESS_SIZE];
    fgets(b, sizeof(b), file);
    char s[MAX_ADDRESS_SIZE];
    sscanf(b, "%s", s);
    if(argc == 2)
    {
        while(fgets(r, sizeof(r), branches)!= NULL)
        {
            sscanf(r, "%s", t);
            if(strcmp(s,t) == 0)
            {
                fprintf(stdout, "*%s  ", t);
            }
            else{
                fprintf(stdout, "%s  ", t);
            }
        }
        rewind(branches);
        return 0;
    }
    char* a = (char*)malloc(MAX_ADDRESS_SIZE);
    bool repeat = false;
    char l[MAX_ADDRESS_SIZE];
    while(fgets(l, sizeof(l), branches)!= NULL)
    {
        sscanf(l, "%s", a);
        if(strcmp(argv[2], a) == 0)
        {
            repeat = true;
        }
    }
    if(repeat)
    {
        fprintf(stderr, "Branch %s already exists\n",argv[2]);
        return 1;
    }
    fclose(file);
    fclose(branches);
    char* command = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(branch_path, "%s/%s", repo, argv[2]);
    sprintf(command, "mkdir -p %s", branch_path);
    system(command);
    branches = fopen(branch_list, "a");
    fprintf(branches, "%s  %s\n", argv[2], b);
    fclose(branches);
    return 0;
}
int commit(char *argv, int argc)
{
    char cwd[MAX_ADDRESS_SIZE];
    getcwd(cwd, sizeof(cwd));
    if (argc < 4)
    {
        fprintf(stderr, "Please enter a message to commit\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if (!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    if (strlen(argv) > 72)
    {
        fprintf(stderr, "Message is too long to be commited\n");
        return 1;
    }
    if (argc > 4)
    {
        fprintf(stderr, "Message must be in \" \"\n");
        return 1;
    }

    bool stage_exists = false;
    if (strstr(cwd, repo) == NULL)
    {
        fprintf(stderr, "Not in a neogit repository to commit\n");
        return 1;
    }
    strcat(repo, "/.neogit");
    sprintf(stage_path, "%s/stage", repo);
    struct dirent *entry;
    DIR *dir = opendir(repo);
    if (dir == NULL)
    {
        fprintf(stderr, "Failed to open repository\n");
        return 1;
    }
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, "stage") == 0)
        {
            stage_exists = true;
        }
    }
    if(!stage_exists)
    {
        fprintf(stderr, "Nothing has been added before to be commit\n");
        return 1;
    }
    closedir(dir);
    dir = opendir(stage_path);
    if(dir == NULL)
    {
        fprintf(stderr, "Failed tp open stage\n");
        return 1;
    }
    entry = readdir(dir);
    if(entry->d_name== NULL)
    {
        fprintf(stderr, "Nothing added to commit\n");
        return 1;
    }
    closedir(dir);
    dir = opendir(stage_path);
    if(dir == NULL)
    {
        fprintf(stderr, "Failed to open stage\n");
        return 1;
    }
    int file_count = 0;
    int dir_count = 0;
    while((entry = readdir(dir)) != NULL)
    {
        if(entry->d_type == DT_DIR)
        {
            dir_count++;
        }
        if(entry->d_type == DT_REG)
        {
            file_count++;
        }
    }
    sprintf(id_path, "%s/.id.txt", repo);
    commit_id = fopen(id_path, "r");
    if(commit_id == NULL)
    {
        fprintf(stderr, "Error openning id file\n");
        return 1;
    }
    fscanf(commit_id, "%d", &id);
    fclose(commit_id);
    commit_id = fopen(id_path, "w");
    fprintf(commit_id, "%d", (id+1));
    fclose(commit_id);
    sprintf(commit_path, "%s/#%d", repo, id);
    char* command = (char*)malloc(2*MAX_ADDRESS_SIZE);
    sprintf(command, "mkdir -p %s", commit_path);
    system(command);
    char* in_commit = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(in_commit, "%s/stage", commit_path);
    sprintf(command, "cp -r %s %s", stage_path,in_commit);
    system(command);
    char* stlist = (char*)malloc(MAX_ADDRESS_SIZE);;
    sprintf(stlist, "%s/stage_list.txt", repo);
    sprintf(in_commit, "%s/stage_list.txt", commit_path);
    sprintf(command, "cp %s %s", stlist, in_commit);
    system(command);
    char *new_branchlist = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(new_branchlist, "%s/.branches.txt", commit_path);
    sprintf(branch_list, "%s/.branches.txt", repo);
    sprintf(command, "cp %s %s", branch_list, new_branchlist);
    system(command);
    char* commit_cb = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(commit_cb, "%s/.current_branch.txt", commit_path);
    sprintf(current_branch, "%s/.current_branch.txt", repo);
    sprintf(command, "cp %s %s", current_branch, commit_cb);
    system(command);
    sprintf(commit_info, "%s/.info", commit_path);
    info = fopen(commit_info, "w");
    if(info == NULL)
    {
        fprintf(stderr, "Failed to open #%d commit info\n", id);
        return 1;
    }
    time_t rawtime;
    struct tm *timeinfo;
    char time_string[100];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_string, sizeof(time_string), "%c", timeinfo);
    fprintf(info, "commit id is : %d\n", id);
    fprintf(info, "commit message is : %s\n", argv);
    fprintf(info, "commit was done at %s", ctime(&rawtime));
    char* conname = (char*)malloc(MAX_ADDRESS_SIZE);
    char *conemail=(char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(conname, "%s/config/username", repo);
    sprintf(conemail, "%s/config/useremail", repo);
    FILE* e = fopen(conemail, "r");
    if(e == NULL)
    {
        fprintf(stderr, "Failed to open user email file\n");
        return 1;
    }
    FILE* n = fopen(conname, "r");
    if (n == NULL)
    {
        fprintf(stderr, "Failed to open user name file\n");
        return 1;
    }
    char un[100];
    char ue[100];
    fgets(un, sizeof(un), n);
    fgets(ue, sizeof(ue), e);
    fclose(e);
    fclose(n);
    fprintf(info, "user name is : %suser email is : %s", un, ue);
    FILE* b = fopen(current_branch,"r");
    fprintf(stdout, "%s", current_branch);
    if(b == NULL)
    {
        fprintf(stderr, "Failed to open current branch list\n");
        return 1;
    }
    char branch[MAX_ADDRESS_SIZE];
    char d[MAX_ADDRESS_SIZE];
    char* f = (char*)malloc(MAX_ADDRESS_SIZE);
    fscanf(b, "%s", branch);
    fprintf(info, "commit is done in branch \"%s\"\n", branch);
    sprintf(f, "%s/%s", commit_path, branch); //new
    sprintf(command, "mkdir -p %s",f);//new
    system(command);//new
    fclose(b);
    fprintf(info, "number of commited files are %d\nnumber of commited directories are %d\n\n", file_count, dir_count);
    fclose(info);
    sprintf(log_path, "%s/.log",repo);
    char* tmp_path =(char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(tmp_path, "%s/.tmp.txt", repo);
    FILE* tmp = fopen(tmp_path, "w");
    if(tmp == NULL)
    {
        fprintf(stderr,"Failed to open tmp file\n");
        return 1;
    }
    log_file = fopen(log_path, "a");
    if(log_file == NULL)
    {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }
    fclose(log_file);
    log_file = fopen(log_path, "r");
    if(log_file == NULL)
    {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    while(fgets(a, sizeof(a), log_file) != NULL)
    {
        fprintf(tmp, "%s", a);
    }
    fclose(log_file);
    log_file = fopen(log_path, "w");
    info = fopen(commit_info, "r");
    while(fgets(a, sizeof(a), info)!= NULL)
    {
        fprintf(log_file, "%s", a);
    }
    fclose(tmp);
    tmp = fopen(tmp_path, "r");
    fclose(log_file);
    log_file = fopen(log_path, "a");
    while(fgets(a, sizeof(a), tmp)!= NULL)
    {
        fprintf(log_file, "%s", a);
    }
    sprintf(command, "rm -r %s", stage_path); //new
    system(command); //new
    fclose(log_file);
    fclose(tmp);
    fclose(info);
    remove(tmp_path);
    sprintf(command, "mkdir -p %s/HEAD", repo);
    system(command);
    sprintf(tmp_path, "%s/HEAD/commit", repo);
    sprintf(command, "cp -r %s %s", commit_path, tmp_path);
    system(command);
    sprintf(tmp_path, "%s/%s/.lastcommit.txt",repo,branch);
    fprintf(stdout, "%s", tmp_path);
    FILE* fp = fopen(tmp_path, "w");
    if(fp == NULL)
    {
        fprintf(stderr, "Failed openning last commit file\n");
        return 1;
    }
    fprintf(fp, "#%d", id);
    fclose(fp);
    
    return 0;
}
int nlog()
{
    char cwd[MAX_ADDRESS_SIZE];
    getcwd(cwd, sizeof(cwd));
    exists = false;
    repo_check2();
    if (!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    if (strstr(cwd, repo) == NULL)
    {
        fprintf(stderr, "Not in a neogit repository to commit\n");
        return 1;
    }
    strcat(repo, "/.neogit");
    sprintf(log_path, "%s/.log",repo);
    log_file = fopen(log_path, "r");
    char a[MAX_ADDRESS_SIZE];    
    while(fgets(a,sizeof(a), log_file)!= NULL)
    {
        fprintf(stdout, "%s", a);
    }
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
