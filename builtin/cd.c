#include"minishell.h"

char *get_env_val(t_env *env, const char *key)
{
    while (env)
    {
        if (strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

void set_env_val(t_env *env, const char *key, const char *val)
{
    t_env *head = env;

    while (env)
    {
        if (strcmp(env->key, key) == 0)
        {
            free(env->value);
            env->value = strdup(val);
            return;
        }
        env = env->next;
    }

    t_env *new_node = malloc(sizeof(t_env));
    new_node->key = strdup(key);
    new_node->value = strdup(val);
    new_node->next = NULL;

    t_env *tmp = head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}

void update_pwd_env(t_env *env, char *oldpwd)
{
    char cwd[1024];
    if (!getcwd(cwd, sizeof(cwd)))
    {
        perror("getcwd");
        return;
    }

    if (oldpwd)
        set_env_val(env, "OLDPWD", oldpwd);

    set_env_val(env, "PWD", cwd);
}

void execute_cd(t_token *arg, t_env *env)
{
    char *path = NULL;
    char *to_free = NULL;

    if (arg && arg->value)
    {
        path = arg->value;

        // Handle cd -
        if (strcmp(path, "-") == 0)
        {
            path = get_env_val(env, "OLDPWD");
            if (!path)
            {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return;
            }
            ft_printf("%s\n", path); // mimic bash output
        }

        // Handle ~ and ~/folder
        else if (path[0] == '~')
        {
            char *home = get_env_val(env, "HOME");
            if (!home)
            {
                fprintf(stderr, "cd: HOME not set\n");
                return;
            }

            // Join home with the rest of the path (after ~)
            to_free = malloc(strlen(home) + strlen(path)); // +1 is already inside
            if (!to_free)
                return; // handle malloc failure
            sprintf(to_free, "%s%s", home, path + 1); // skip the '~'
            path = to_free;
        }
    }
    else
    {
        // Default case: cd with no args
        path = get_env_val(env, "HOME");
        if (!path)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    }

    // Save old PWD
    char *oldpwd = getcwd(NULL, 0);
    if (!oldpwd)
    {
        perror("getcwd before chdir");
        if (to_free) free(to_free);
        return;
    }

    // Try to change directory
    if (chdir(path) == -1)
    {
        perror("cd");
        free(oldpwd);
        if (to_free) free(to_free);
        return;
    }

    // Update PWD and OLDPWD
    update_pwd_env(env, oldpwd);

    free(oldpwd);
    if (to_free) free(to_free);
}