#include"minishell.h"

int check_cmd(t_token *cmd_token)
{
    // List of built-in commands
    char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
    int i = 0;

    // Check if the token is a command and compare it to the built-ins
    if (cmd_token && cmd_token->type == T_WORD)
    {
        for (i = 0; i < 7; i++)
        {
            if (strcmp(cmd_token->value, builtins[i]) == 0)
            {
                return 1; // It's a built-in command
            }
        }
    }

    return 0; // It's not a built-in command
}
void execute_builtin(char *cmd)
{
    if(check_cmd(cmd) == 1)
    {}
    if (strcmp(cmd, "pwd") == 0)
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("getcwd");
    }
}