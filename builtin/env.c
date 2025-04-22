#include"minishell.h"

char **env_cpy(t_env *env)
{
    int count = 0;
    t_env *tmp = env;

    // Count how many variables
    while (tmp)
    {
        if (tmp->key && tmp->value)
            count++;
        tmp = tmp->next;
    }

    char **env_arr = malloc(sizeof(char *) * (count + 1));
    if (!env_arr)
        return NULL;

    tmp = env;
    int i = 0;
    while (tmp)
    {
        if (tmp->key && tmp->value)
        {
            size_t len = strlen(tmp->key) + strlen(tmp->value) + 2;
            env_arr[i] = malloc(len);
            if (env_arr[i])
                snprintf(env_arr[i], len, "%s=%s", tmp->key, tmp->value);
            i++;
        }
        tmp = tmp->next;
    }
    env_arr[i] = NULL;

    return env_arr;
}
void execute_env(t_env *env)
{
    char **z = env_cpy(env);

    if (z == NULL) {
        fprintf(stderr, "Failed to copy environment variables.\n");
        return;
    }

    for (int i = 0; z[i]; i++)
        printf("%s\n", z[i]);

    free_env_copy(z);
}
void free_env_copy(char **env)
{
    if (!env)
        return;

    for (int i = 0; env[i]; i++)
        free(env[i]);
    free(env);
}

t_env *init_env(char **envp)
{
    t_env *head = NULL;
    t_env *current = NULL;

    for (int i = 0; envp[i]; i++) {
        char *sep = strchr(envp[i], '=');
        if (!sep)
            continue;

        t_env *new_node = malloc(sizeof(t_env));
        new_node->key = strndup(envp[i], sep - envp[i]);
        new_node->value = strdup(sep + 1);
        new_node->next = NULL;

        if (!head)
            head = new_node;
        else
            current->next = new_node;

        current = new_node;
    }
    return head;
}
void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->key);      // Free the key (env variable name)
        free(env->value);    // Free the value (env variable value)
        free(env);           // Free the node itself
        env = tmp;           // Move to the next node
    }
}
