#include "../minishell.h"

void execute_exit(t_env *env, t_token *arg_token)
{
    long long exit_code;
    int argc = 0;
    t_token *tmp = arg_token;

    write(2, "exit\n", 5); // Always print "exit" first

    // Count number of arguments
    while (tmp)
    {
        argc++;
        tmp = tmp->next;
    }

    if (argc == 0) // No arguments: exit with global status
        exit(g_exit_status);

    if (!is_numeric(arg_token->value)) // Non-numeric argument
    {
        dprintf(2, "bash: exit: %s: numeric argument required\n", arg_token->value);
        arg_token->exit_status = 2;
        exit(255);
    }

    if (argc > 1) // Too many arguments
    {
        dprintf(2, "bash: exit: too many arguments\n");
        arg_token->exit_status = 1;
        return; // do NOT exit
    }

    exit_code = ft_atoll(arg_token->value);
    exit(exit_code % 256); // Bash behavior: exit with (code % 256)
}