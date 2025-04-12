#include"minishell.h"

int is_n_flag(const char *str)
{
    if (str[0] != '-' || str[1] == '\0')
        return 0;
    for (int i = 1; str[i]; i++)
    {
        if (str[i] != 'n')
            return 0;
    }
    return 1;
}

void execute_echo(t_token *arg, t_env *env)
{
    int newline = 1;

    (void)env;

    // Handle multiple -n flags
    while (arg && is_n_flag(arg->value))
    {
        newline = 0;
        arg = arg->next;
    }
    while (arg)
    {
        printf("%s", arg->value);
        if (arg->next)
            printf(" ");
        arg = arg->next;
    }

    if (newline)
        printf("\n");
}