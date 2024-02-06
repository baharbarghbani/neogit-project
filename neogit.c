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
char* shortcut;
char *log_path;
FILE *repo_list;
FILE *branches;
FILE *commit_id;
FILE *info;
char *commit_path;
char *commit_info;
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
int set_s(char* argv1, char* argv2, int argc);
int replace_s(char* argv1, char* argv2, int argc);
int remove_s(char* argv, int argc);
int alias(int argc, char* argv1, char* argv2);
int global_alias(int argc, char* argv[]);
int looggn(char* argv, int argc);
int branch_log(char* argv, int argc);
int author_log(char* argv, int argc);
int word_log(char* argv, int argc);
int checkout_branch(char* argv);
int checkout_commit(char*argc);
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
int check_input(char* argv)
{
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    bool is_branch = false;
    strcat(repo,"/.neogit");
    sprintf(branch_list, "%s/.branches.txt", repo);
    branches = fopen(branch_list, "r");
    if(branches == NULL)
    {
        fprintf(stderr, "Failed to open branch list\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    char b[MAX_ADDRESS_SIZE];
    while(fgets(a, sizeof(a), branches) != NULL)
    {
        sscanf(a, "%s", b);
        size_t ln = strlen(b);
        if(b[ln-1] == '\n')
        {
            b[ln-1] = '\0';
        }
        if(strcmp(b, argv) == 0)
        {
            is_branch;
            break;
        }
    }
    int r;
    if(is_branch)
    {
        r = checkout_branch(argv);
    }
    else{
        r = checkout_commit(argv);
    }
    return r;
}
int find_shortcut(char* argv, int argc)
{
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    sprintf(shortcut, "%s/.neogit/.shortcut.txt", repo);
    FILE* fptr = fopen(shortcut, "r");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut file\n");
        return 1;
    }
    char a[200];
    bool s_exist = false;
    char c[200];
    char d[200];
    while(fgets(a, sizeof(a), fptr) != NULL)
    {
        sscanf(a, "%s  %s\n", c, d);
        if(strcmp(c, argv) == 0)
        {
            s_exist = true;
            break;
        }
    }
    if(!s_exist)
    {
        fprintf(stderr, "This shortcut has not been set\n");
        return 1;
    }
    fclose(fptr);
    int result = commit(d, argc);
    return result;
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
    shortcut = (char*)malloc(MAX_ADDRESS_SIZE);
    exists = false;
    repo_check2();
    char* gla = "/home/asus/Documents/neogit/alias_global.txt";
    char* ccc = "/home/asus/Documents/neogit";
    char* alias_path = (char*)malloc(MAX_ADDRESS_SIZE);
    struct dirent* entry;
    bool valid_name = false;
    found = false;
    char a[100];
    char n[100];
    char* c = (char*)malloc(MAX_ADDRESS_SIZE);
    DIR* dir = opendir(ccc);
    if(dir == NULL)
    {
        fprintf(stderr, "Error openning project file\n");
        return 1;
    }
    bool glo_exist = false;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, "alias_global.txt")==0)
        {
            glo_exist = true;
            break;
        }
    }
    closedir(dir);
    if(exists)
    {
        bool alias_exist = false;
        strcat(repo, "/.neogit");
        dir = opendir(repo);
        while((entry = readdir(dir)) != NULL)
        {
            if(strcmp(entry->d_name, ".alias.txt") == 0)
            {
                alias_exist = true;
            }
        }
        if(alias_exist)
        {
            sprintf(alias_path, "%s/.alias.txt", repo);
            FILE* alias_file = fopen(alias_path, "r");
            if(alias_file == NULL)
            {
                fprintf(stderr, "Error openning alias file in main\n");
                return 1;
            }
            while(fgets(a, 100, alias_file) != NULL)
            {
                sscanf(a, "%s", n);
                c = a + strlen(n) + 2;
                if(strcmp(n, argv[1])==0)
                {
                    valid_name = true;
                    found = true;
                    break;
                } 
            }
        }
    }
    if(!exists && glo_exist)
    {
        FILE* fptr = fopen(gla, "r");
        if(fptr == NULL)
        {
            fprintf(stderr, "Error openning alias global file\n");
            return 1;
        }
        while(fgets(a, 100, fptr) != NULL)
        {
            sscanf(a, "%s", n);
            c = a + strlen(n) + 2;
            if(strcmp(n, argv[1]) == 0)
            {
                valid_name = true;
                found = true;
                break;
            }
        }
    }
    if(!exists && !glo_exist)
    {
        fprintf(stderr, "Not a aliased commnad\n");
        return 1;
    }
    if(found)
    {
        if(strcmp(c, "neogit init") == 0)
        {
            sprintf(c, "neogit init");
            system(c);
            return 0;
        }
        else if(strstr(c, "neogit config --global") != NULL)
        {
            sprintf(address, "%s %s", c, argv[1]);
            system(address);
            return 0;
        }
        else if(strstr(c, "neogit config") != NULL)
        {
            sprintf(address, "%s %s", c, argv[1]);
            system(address);
            return 0;
        }
        else if(strstr(c, "neogit add") != NULL)
        {
            sprintf(address, "%s %s", c, argv[1]);
            system(address);
            return 0;
        }
        else if(strstr(c, "neogit reset") != NULL)
        {
            sprintf(address, "%s %s", c, argv[1]);
            system(address);
            return 0;
        }
         else if(strstr(c, "neogit branch") != NULL)
        {
            sprintf(address, "%s %s", c, argv[1]);
            system(address);
            return 0;
        }
         else if(strstr(c, "neogit commit") != NULL)
        {
            sprintf(address, "%s %s", c, argv[1]);
            system(address);
            return 0;
        }
        
    }



    if (strcmp(argv[1], "init") == 0)
    {
        return run_init(argc, argv);
    }
    else if ((strcmp(argv[1], "config") == 0))
    {
        if(strcmp(argv[2], "--global")==0)
        {
            if(strstr(argv[3], "alias.") != NULL)
            {
                return global_alias(argc, argv);
            }
            else{
                return global_config(argc, argv);
            }
        }
        else if(strstr(argv[2], "alias.") != NULL)
        {
            return alias(argc, argv[2], argv[3]);
        }
        else{
            return config(argc, argv);
        }
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
    else if (strcmp(argv[1], "commit") == 0)
    {
        if(argc == 4)
        {
            if(strcmp(argv[2], "-m") == 0)   return commit(argv[3], argc);
            if(strcmp(argv[2], "-s") == 0) 
            {
                int r = find_shortcut(argv[3], argc);
                if(r == 1)
                {
                    return 1;
                }
            } 
            else{
                fprintf(stderr, "Invalid config command\n");
                return 1;
            }
        }
        else{
            fprintf(stderr, "Wrong number of arguements for commit %s", argv[3]);
            return 1;
        }
    }
    else if(strcmp(argv[1], "log") == 0)
    {
        if(argc == 4)
        {
            if(strcmp(argv[2], "-n") == 0)
            {
                return looggn(argv[3], argc);
            }
            else if(strcmp(argv[2], "-branch") == 0)
            {
                return branch_log(argv[3], argc);
            }
            else if(strcmp(argv[2], "-author") == 0)
            {
                return author_log(argv[3], argc);
            }
            else if(strcmp(argv[2], "-search") == 0)
            {
                return word_log(argv[3], argc);
            }
            
        }
        else if(argc > 4)
        {
            if(strcmp(argv[2], "-search") == 0)
            {
                for(int i = 3; i < argc; i++)
                {
                    chdir(source);
                    char command[MAX_ADDRESS_SIZE];
                    sprintf(command, "neogit log -search %s", argv[i]);
                    system(command);
                }
                return 0;
            }
        }
        else{
            return nlog();
        }
    }
    else if(strcmp(argv[1], "branch") == 0)
    {
        return branch(argv, argc);
    }
    else if(strcmp(argv[1], "set") == 0 && strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
    {
        return set_s(argv[3], argv[5], argc);
    }
    else if(strcmp(argv[1], "replace") == 0 && strcmp(argv[2], "-m") == 0 && strcmp(argv[4], "-s") == 0)
    {
        return replace_s(argv[5], argv[3], argc);
    }
    else if(strcmp(argv[1], "remove") == 0)
    {
        if(argc == 4)
        {
            if(strcmp(argv[2], "-s") == 0)
            {
                return remove_s(argv[3], argc);
            }
            else{
                fprintf(stderr, "Invalid command for commit\n");
                return 1;
            }
        }
        else if(argc >4)
        {
            fprintf(stderr, "Too many arguements for remove shortcut\n");
            return 1;
        }
        else{
            fprintf(stderr, "few number of arguements for remove shortcut\n");
            return 1;
        }

    }
    else if(strcmp(argv[1], "checkout") == 0)
    {
        if(argc > 3)
        {
            fprintf(stderr, "Too many arguements in neogit checkout\n");
            return 1;
        }
        if (argc < 3)
        {
            fprintf(stderr, "few arguements in neogit checkout\n");
            return 1;
        }
        else{
            return check_input(argv[2]);
        }


    }
    else{
        fprintf(stderr, "Invalid command\n");
        return 1;
    }

    return 0;
}
int run_init(int argc, char *argv[])
{
    char cwd[2000];
    char tmp_cwd[2000];
    exists = false;
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
    chdir(cwd);
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
            {
                fprintf(stderr, "Error getting tmp_cwd\n");
                return 1;

            }
    
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
        fprintf(stdout, "\n");
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
    char* shortc = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(shortc, "%s/.shortcut.txt", repo);
    sprintf(in_commit, "%s/.shortcut.txt", commit_path);
    sprintf(command, "cp %s %s", shortc, in_commit);
    char* stlist = (char*)malloc(MAX_ADDRESS_SIZE);
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
    char* alis = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(alis, "%s/.alias.txt", repo);
    sprintf(in_commit, "%s/.alias.txt", commit_path);
    sprintf(command, "cp %s %s", repo,in_commit);
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
int set_s(char* argv1, char* argv2, int argc)
{
    if(argc > 6)
    {
        fprintf(stderr, "Too many arguements\n");
        return 1;
    }
    if(argc < 6)
    {
        fprintf(stderr, "few arguements\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    strcat(repo, "/.neogit");
    sprintf(shortcut, "%s/.shortcut.txt", repo);
    FILE* fptr = fopen(shortcut, "a");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut file\n");
        return 1;
    }
    fprintf(fptr, "%s  %s\n", argv2, argv1);
    fclose(fptr);
return 0;
}
int replace_s(char* argv1, char* argv2, int argc)
{
    if(argc > 6)
    {
        fprintf(stderr, "Too many arguements in replacing shortcut\n");
        return 1;
    }
    if(argc < 6)
    {
        fprintf(stderr, "few arguements in replacing shortcut\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    strcat(repo, "/.neogit");
    sprintf(shortcut, "%s/.shortcut.txt", repo);
    FILE* fptr = fopen(shortcut, "r");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut file\n");
        return 1;
    }
    char* tmp = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(tmp, "%s/.tmp.txt", repo);
    FILE* tmp_file = fopen(tmp, "w");
    if(tmp_file == NULL)
    {
        fprintf(stderr, "Failed to open tmp file in replacing shortcut\n");
        return 1;
    }
    char a[300];
    char s[300];
    char c[300];
    char co[300];
    bool shortcut_e = false;
    while(fgets(a, sizeof(a), fptr) != NULL)
    {
        sscanf(a, "%s  %s\n", s, c);
        if(strcmp(s, argv1) == 0)
        {
            if(strcpy(co, c) == NULL)
            {
                fprintf(stderr, "Failed to copy c to co in replace\n");
                return 1;
            }
            continue;
        }
        else{
            fprintf(tmp_file, "%s", a);
        }
    }
    rewind(fptr);
    fclose(fptr);
    fclose(tmp_file);
    tmp_file = fopen(tmp, "r");
    if(tmp_file == NULL)
    {
        fprintf(stderr, "Failed to open tmp file in replacing shortcut\n");
        return 1;
    }
    fptr = fopen(shortcut, "w");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut file\n");
        return 1;
    }
    while(fgets(a, sizeof(a), tmp_file) != NULL)
    {
        fprintf(fptr, "%s", a);
    }
    fclose(fptr);
    fclose(tmp_file);
    fptr = fopen(shortcut, "a");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut list\n");
        return 1;
    }
    fprintf(fptr, "%s  %s\n", argv1, argv2);
    fclose(fptr);
    remove(tmp);
return 0;
}
int remove_s(char* argv, int argc)
{
    if(argc > 4)
    {
        fprintf(stderr, "Too many arguements in replacing shortcut\n");
        return 1;
    }
    if(argc < 4)
    {
        fprintf(stderr, "few arguements in replacing shortcut\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    strcat(repo, "/.neogit");
    sprintf(shortcut, "%s/.shortcut.txt", repo);
    FILE* fptr = fopen(shortcut, "r");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut file\n");
        return 1;
    }
    char* tmp = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(tmp, "%s/.tmp.txt", repo);
    FILE* tmp_file = fopen(tmp, "w");
    if(tmp_file == NULL)
    {
        fprintf(stderr, "Failed to open tmp file in replacing shortcut\n");
        return 1;
    }
    char a[300];
    char b[300];
    char c[300];
    while(fgets(a, sizeof(a), fptr) != NULL)
    {
        sscanf(a, "%s  %s\n",b, c);
        if(strcmp(b, argv) == 0)
        {
            continue;
        }
        else{
            fprintf(tmp_file, "%s", a);
        }
    }
    rewind(fptr);
    fclose(fptr);
    fclose(tmp_file);
    tmp_file = fopen(tmp, "r");
    if(tmp_file == NULL)
    {
        fprintf(stderr, "Failed to open tmp file in replacing shortcut\n");
        return 1;
    }
    fptr = fopen(shortcut, "w");
    if(fptr == NULL)
    {
        fprintf(stderr, "Failed to open shortcut file\n");
        return 1;
    }
    while(fgets(a, sizeof(a), tmp_file) != NULL)
    {
        fprintf(fptr, "%s", a);
    }
    fclose(fptr);
    fclose(tmp_file);
    remove(tmp);
    return 0;
}
int alias(int argc, char* argv1, char* argv2)
{
    if(argc> 4)
    {
        fprintf(stderr, "Too many arguements in alias\n");
        return 1;
    }
    if(argc < 4)
    {
        fprintf(stderr, "feww arguements in alias\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    if(strcat(repo, "/.neogit") == NULL)
    {
        fprintf(stderr, "Failed to strcat .neogit to repo in alias\n");
        return 1;
    }
    char* alias_path = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(alias_path, "%s/.alias.txt", repo);
    FILE* alias_file = fopen(alias_path, "a");
    if(alias_file == NULL)
    {
        fprintf(stderr, "Failed to open alias file\n");
        return 1;
    }
    char* commands = (char*)malloc(200);
    sprintf(commands, "/home/asus/Documents/neogit/commands.txt");
    FILE* file = fopen(commands, "r");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open command\n");
        return 1;
    }
    char a[100];
    bool command_valid = false;
    while(fgets(a, sizeof(a), file)!= NULL)
    {
        size_t ln = strlen(a);
        a[ln-1] = '\0';
        if(strstr(argv2, a) != NULL)
        {
            command_valid = true;
            break;
        }
    }
    fclose(file);
    if(!command_valid)
    {
        fprintf(stderr, "Not a valid command to be aliased\n");
        return 1;
    }
    char* str = (char*)malloc(100);
    if((str = strstr(argv1, "alias.")) == NULL)
    {
        fprintf(stderr, "Invalid alias command\n");
        return 1;
    }
    str = str + strlen("alias.");
    fprintf(alias_file, "%s  %s\n", str, argv2);
    fclose(alias_file);
    FILE* file2 = fopen("/home/asus/Documents/neogit/.alias_path.txt", "a");
    if(file2 == NULL)
    {
        fprintf(stderr, "Error openning alias list\n");
        return 1;
    }
    fprintf(file2, "%s", alias_path);
    fclose(file2);

    return 0;
}
int global_alias(int argc, char* argv[])
{
    if(argc != 5)
    {
        fprintf(stderr, "Wrong number of arguements in alias global\n");
        return 0;
    }
    FILE *global = fopen("/home/asus/Documents/neogit/alias_global.txt", "a");
    if(global == NULL)
    {
        fprintf(stderr, "Error in global alias\n");
        return 1;
    }
    char* lines = (char*)malloc(2000);
    FILE* commands = fopen("/home/asus/Documents/neogit/commands.txt", "r");
    if(commands == NULL)
    {
        fprintf(stderr, "Error in global alias\n");
        return 1;
    }
    char* name = (char*)malloc(2000);
    char* command = (char*)malloc(2000);
    sscanf(argv[3], "alias.%s", name);
    strcpy(command, argv[4]);
    bool exist = false;
    while(fgets(lines, 1000, commands) != NULL)
    {
        int ln = strlen(lines);
        lines[ln - 1] = '\0';
        if(strstr(command, lines) == NULL)
        {
            exist = true;
            break;
        }
    }
    char* address = (char*)malloc(2000);
    if(exist)
    {
        fprintf(global, "%s  %s\n", name, command);
    }
    else{
        fprintf(stderr, "The given command in not a neogit command\n");
        return 1;
    }
    FILE* file = fopen("/home/asus/Documents/neogit/.alias_path.txt", "a");
    fclose(file);
    file = fopen("/home/asus/Documents/neogit/.alias_path.txt", "r");
    if(file == NULL)
    {
        fprintf(stderr, "Failed to open the file\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    while(fgets(a, MAX_ADDRESS_SIZE, file) != NULL)
    {
        size_t ln = strlen(a);
        a[ln-1] = '\0';
        FILE* tmp = fopen(a,"a");
        fprintf(tmp, "%s  %s\n", name, command);
        fclose(tmp);

    }
    fclose(file);
    fclose(global);
    fclose(commands);
    free(lines);
    return 0;
}
int looggn(char* argv, int argc)
{
    if(argc > 4)
    {
        fprintf(stderr, "Too many arguements for log -n\n");
        return 1;
    }
    if(argc < 4)
    {
        fprintf(stderr, "few arguements for log -n\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    strcat(repo, "/.neogit");
    sprintf(log_path, "%s/.log", repo);
    FILE* log_file = fopen(log_path, "r");
    if(log_file == NULL)
    {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }
    if(!isdigit(argv))
    {
        fprintf(stderr, "Invalid command\n");
        return 1;
    }
    int ln = strlen(argv);
    int value = 1;
    int num = 0;
    for(int i = 0; i < ln; i++)
    {
        value = value * 10;
    }
    value /= 10;
    for(int i = 0; i < ln; i++)
    {
        num += value* (argv[i] - 48);
        value /= 10;
    }
    char s[MAX_ADDRESS_SIZE];
    int count = 0;
    while(fgets(s, sizeof(s), log_file) != NULL)
    {
        count++;
    }
    if(count > num * 9)
    {
        fprintf(stderr, "Number of commits are less than the requeated number\n");
        return 1;
    }
    rewind(log_file);
    for(int i = 0; i < 9 * num; i++)
    {
        fgets(s, sizeof(s), log_file);
        fprintf(stdout, "%s", s);
    }
    fclose(log_file);
    return 0;
}
int branch_log(char* argv, int argc)
{
    if(argc > 4)
    {
        fprintf(stderr, "Too many arguements for log -n\n");
        return 1;
    }
    if(argc < 4)
    {
        fprintf(stderr, "few arguements for log -n\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    strcat(repo, "/.neogit");
    sprintf(log_path, "%s/.log", repo);
    char* log_path_copy = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(log_path_copy, "%s/log.copy.txt", repo);
    command = (char*)malloc(MAX_ADDRESS_SIZE);
    FILE* log_copy = fopen(log_path_copy, "w");
    if(log_copy == NULL)
    {
        fpritnf(stderr, "Failed to open log copy\n");
        return 1;
    }
    FILE* log_file = fopen(log_path, "r");
    if(log_file == NULL)
    {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    while(fgets(a,sizeof(a), log_file) != NULL)
    {
        fprintf(log_copy, "%s", a);
    }
    rewind(log_file);
    fclose(log_copy);
    log_copy = fopen(log_path_copy, "r");
    int count = 0;
    while(fgets(a, sizeof(a), log_file) != NULL)
    {
        count++;
        if((count+3) % 9 == 0)
        {
            if(strstr(a, argv) != NULL)
            {
                for(int i = 0; i < count - 6; i++)
                {
                    fgets(a, sizeof(a), log_copy);
                }
                for(int i = 0; i < 9; i++)
                {
                    fgets(a, sizeof(a), log_copy);
                    fprintf(stdout, "%s", a);
                }
            }
        }
        rewind(log_copy);
    }
fclose(log_file);
remove(log_path_copy);
return 0;
}
int author_log(char* argv, int argc)
{
    if(argc > 4)
    {
        fprintf(stderr, "Too many arguements for log -n\n");
        return 1;
    }
    if(argc < 4)
    {
        fprintf(stderr, "few arguements for log -n\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    strcat(repo, "/.neogit");
    sprintf(log_path, "%s/.log", repo);
    char* log_path_copy = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(log_path_copy, "%s/log.copy.txt", repo);
    command = (char*)malloc(MAX_ADDRESS_SIZE);
    FILE* log_copy = fopen(log_path_copy, "w");
    if(log_copy == NULL)
    {
        fpritnf(stderr, "Failed to open log copy\n");
        return 1;
    }
    FILE* log_file = fopen(log_path, "r");
    if(log_file == NULL)
    {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    while(fgets(a,sizeof(a), log_file) != NULL)
    {
        fprintf(log_copy, "%s", a);
    }
    rewind(log_file);
    fclose(log_copy);
    log_copy = fopen(log_path_copy, "r");
    int count = 0;
    while(fgets(a, sizeof(a), log_file) != NULL)
    {
        count++;
        if((count+5) % 9 == 0)
        {
            if(strstr(a, argv) != NULL)
            {
                for(int i = 0; i < count - 6; i++)
                {
                    fgets(a, sizeof(a), log_copy);
                }
                for(int i = 0; i < 9; i++)
                {
                    fgets(a, sizeof(a), log_copy);
                    fprintf(stdout, "%s", a);
                }
            }
        }
        rewind(log_copy);
    }
fclose(log_file);
remove(log_path_copy);
return 0;
}
int word_log(char* argv, int argc)
{
    if(argc > 4)
    {
        fprintf(stderr, "Too many arguements for log -n\n");
        return 1;
    }
    if(argc < 4)
    {
        fprintf(stderr, "few arguements for log -n\n");
        return 1;
    }
    exists = false;
    repo_check2();
    if(!exists)
    {
        fprintf(stderr, "Repository has not been initialized\n");
        return 1;
    }
    char a[MAX_ADDRESS_SIZE];
    strcat(repo, "/.neogit");
    sprintf(log_path, "%s/.log", repo);
    char* log_path_copy = (char*)malloc(MAX_ADDRESS_SIZE);
    sprintf(log_path_copy, "%s/log.copy.txt", repo);
    command = (char*)malloc(MAX_ADDRESS_SIZE);
    FILE* log_copy = fopen(log_path_copy, "w");
    if(log_copy == NULL)
    {
        fpritnf(stderr, "Failed to open log copy\n");
        return 1;
    }
    FILE* log_file = fopen(log_path, "r");
    if(log_file == NULL)
    {
        fprintf(stderr, "Failed to open log file\n");
        return 1;
    }
    char* a = (char*)malloc(MAX_ADDRESS_SIZE);
    while(fgets(a, MAX_ADDRESS_SIZE, log_file) != NULL)
    {
        fprintf(log_copy, "%s", a);
    }
    rewind(log_file);
    fclose(log_copy);
    log_copy = fopen(log_path_copy, "r");
    int count = 0;
    if(log_copy == NULL)
    {
        fpritnf(stderr, "Failed to open log copy\n");
        return 1;
    }
    char* b = (char*)malloc(MAX_ADDRESS_SIZE);
    while(fgets(a, MAX_ADDRESS_SIZE ,log_file) != NULL)
    {
        count++;
        if((count+3) % 9 == 0)
        {
            strcpy(b,a);
            b = b + strlen("commit message is : ");
            if(strstr(b, argv) != NULL)
            {
                for(int i = 0; i < count - 6; i++)
                {
                    fgets(a, MAX_ADDRESS_SIZE, log_copy);
                }
                for(int i = 0; i < 9; i++)
                {
                    fgets(a, MAX_ADDRESS_SIZE, log_copy);
                    fprintf(stdout, "%s", a);
                }
            }
        }
        rewind(log_copy);
    }
fclose(log_file);
remove(log_path_copy);
return 0;
}
int checkout_branch(char* argv)
{
    DIR* dir = opendir(repo);
    if(dir == NULL)
    {
        fprintf(stderr, "Failed to open .neogit\n");
        return 1;
    }
    struct dirent* entry;
    stage_exists = false;
    while((entry = readdir(dir)) != NULL)
    {
        if(strcmp(entry->d_name, "stage") == 0)
        {
            stage_exists = true;
            break;
        }
    }
    if(stage_exists)
    {
        fprintf(stderr, "Staging areas has some uncommited changes\n");
        return 1;
    }
    sprintf(commit_info, "%s/%s/.lastcommit.txt", repo, argv);
    FILE* last = fopen(commit_info, "r");
    if(last == NULL)
    {
        fprintf(stderr, "Failed to open last commit in branch %s\n", argv);
        return 1;
    }
    char* name2 = (char*)malloc(100);
    fscanf(last, "%s", name2);
    sprintf(commit_path, "%s/%s", repo, name2);
    sprintf(current_branch, "%s/.branches.txt", repo);
    FILE* b = fopen(current_branch, "r");
    if(b == NULL)
    {
        fprintf(stderr, "Failed to open current branch list\n");
        return 1;
    }
    char c[200];
    fgets(c, sizeof(c), b);
    fclose(b);
    b = fopen(current_branch, "w");
    if(b == NULL)
    {
        fprintf(stderr, "Failed to open current branch list\n");
        return 1;
    }
    fprintf(b, "%s/%s", argv, c);
    fclose(b);


    return 0;
}
int checkout_commit(char* argv)
{
    DIR* dir = opendir(repo);
    if(dir == NULL)
    {
        fprintf(stderr, "Failed to open .neogit\n");
        return 1;
    }
    
}