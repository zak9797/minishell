#include "minishell.h"

t_shell_state g_shell = {
    .last_exit_status = 0,
    .signal_flag = 0
};

// Check if a signal was caught (Ctrl+C)
int check_signal(t_token *token)
{
    if (g_shell.signal_flag)
    {
        // This handles Ctrl+C before command execution
        g_shell.signal_flag = 0;
        g_shell.last_exit_status = 130;
        if (token)
            token->exit_status = 130;
        return 1;
    }
    return 0;
}

// Main function to handle the shell
void	handle_signal_flag(void)
{
	if (g_shell.signal_flag)
	{
		g_shell.signal_flag = 0;
		g_shell.last_exit_status = 130;
	}
}

char	**read_and_expand_input(t_env *env)
{
	char	*input = readline("minishell$ ");
	char	*expanding;
	char	**args;

	if (!input)
		return (NULL);
	if (input[0] == '\0')
	{
		free(input);
		return ((char **)1); // Return dummy non-null pointer to skip
	}
	add_history(input);
	expanding = expand_all(input, env);
	free(input);
	if (!expanding)
		return ((char **)1);
	args = ft_split(expanding, '|');
	free(expanding);
	return (args);
}

void	execution_loop(t_env *env)
{
	int i;
	char	**args;

	
	while (1)
	{
		i= 0;
		handle_signal_flag();
		args = read_and_expand_input(env);
		if (args == NULL)
			break;
		if (args == (char **)1 || !args || !args[0])
		{
			if (args != (char **)1)
				free_split(args);
			continue;
		}
		if (args[1] == NULL)
			execute_simple(args, env);
		else
			execute_complex(args, env);

			
		while(args[i])
		{
			t_token *tokens = tokenize_input(args[i]);
			freee_tokens(tokens);
			i++;
		}
		free_split(args);
	}
}
void free_env_list(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

int	main(int argc, char **argv, char **envp)
{
	t_env *env;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	execution_loop(env);
	free_env_list(env);

	return (0);
}
