#include "../minishell.h"


char *strip_quotes(const char *val)
{
    size_t len = ft_strlen(val);
    if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"') ||
                     (val[0] == '\'' && val[len - 1] == '\'')))
    {
        return ft_strndup(val + 1, len - 2);
    }
    return ft_strdup(val);
}


void execute_export(t_token *token, t_env *env)
{
    if (!token)
    {
        // No args: print all exported env vars
        t_env *curr = env;
        while (curr)
        {
        if (curr->value)
            printf("declare -x %s=\"%s\"", curr->key, curr->value);
        else
        printf("declare -x %s", curr->key);
        }
        return;
    }
    print_tokens(token);
    while (token)
    {
        char *arg = token->value;
        char *equal_sign = strchr(arg, '=');

        if (equal_sign)
        {
            *equal_sign = '\0';
            char *key = arg;
            char *value = equal_sign + 1;
        
            // Strip quotes first
            char *cleaned = strip_quotes(value);
        
            // Expand cleaned value
            char *expanded = expand_all(cleaned, env);
            free(cleaned);
        
            // Set in environment
            set_env_val(env, key, expanded);
            free(expanded);
        
            *equal_sign = '=';
        }
    else
    {
            // No value: make sure it's in env with empty value if not set
        if (strcmp(arg, "") != 0 && !*get_env_val(env, arg))
                set_env_val(env, arg, "");
    }

         token = token->next;
    }
}