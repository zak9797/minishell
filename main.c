#include "minishell.h"

volatile sig_atomic_t	g_sig_int = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig_int = 1;
	write(STDOUT_FILENO, "\n", 1); // move to new line
	rl_replace_line("", 0);        // clear current line buffer
	rl_on_new_line();              // tell readline we're on a new line
	rl_redisplay();                // redraw the prompt
}

int check_signal()
{
	if (g_sig_int)
	{
		dup2(STDOUT_FILENO, STDIN_FILENO);
		// g_sig_int = 0;
		return (1);
	}
	return (0);
}

void	init_signals(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
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

void execution_loop(t_env *env)
{
	int last_exit_status =0;
    int i;
    char **args;
    init_signals();

    while (1)
    {
		check_signal();
		if (g_sig_int)
		{
			last_exit_status = 130;
			g_sig_int = 0;
			continue;
		}
		
        i = 0;
        args = read_and_expand_input(env);
        if (args == NULL  && !g_sig_int)
		{
			write(STDOUT_FILENO, "exit\n", 5);
            break;
		}
        if (args == (char **)1 || !args || !args[0])
        {
            if (args != (char **)1)
                free_split(args);
            continue;
        }
        if (args[1] == NULL)
		{
			last_exit_status = execute_simple(args, env, last_exit_status);
		}
        else
			last_exit_status = execute_complex(args, env, last_exit_status);
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
