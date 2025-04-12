#include "../minishell.h"

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
            if (ft_strcmp(cmd_token->value, builtins[i]) == 0)
            {
                return 1; // It's a built-in command
            }
        }
    }

    return 0; // It's not a built-in command
}

void execute_builtin(t_token *tokens, t_env *env)
{
    t_token *cmd_token = tokens;

    if (cmd_token && check_cmd(cmd_token))  // If it's a built-in command
    {
        if (ft_strcmp(cmd_token->value, "pwd") == 0)
        {
            execute_pwd(env);  // Use the environment copy here
        }
        else if (ft_strcmp(cmd_token->value, "env") == 0)
        {
            execute_env(env);
        }
        else if (ft_strcmp(cmd_token->value, "cd") == 0)
        {
            t_token *next_token = cmd_token->next;
            execute_cd(next_token, env);  // Pass envp to update PWD
        }
        else if (ft_strcmp(cmd_token->value, "echo") == 0)
        {
            t_token *next_token = cmd_token->next;
            execute_echo(next_token, env);
        }
    }
}        /*else if (ft_strcmp(cmd_token->value, "cd") == 0)
        {
            // Get the next token (the directory path)
            t_token *cd_token = cmd_token->next;
            if (cd_token)
            {
                execute_cd(cd_token->value);  // Handle cd with the path argument
            }
            else
            {
                // Handle the case where no path is provided
                execute_cd(NULL);  // Default to HOME if no argument is passed
            }
        }*/

        // Continue processing other tokens after built-in commands

