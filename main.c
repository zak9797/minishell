#include "minishell.h"

volatile sig_atomic_t	g_sig_int = 0;

char	**read_and_expand_input(t_env *env, char *argv, int *last_exit_status)
{
	char	*input;
	char	*expanding;
	char	**args;

	input = readline("minishell$ ");
	if (!input || input[0] == '\0')
	{
		free(input);
		return (NULL);  // caller checks for NULL and skips
	}
	add_history(input);
	expanding = expand_all(input, env, argv, last_exit_status);
	free(input);
	if (!expanding)
		return ((char **)1);
	args = ft_split(expanding, '|');
	free(expanding);
	return (args);
}

int	handle_signals_and_input(char ***args, t_env *env, char *argv, int *last_exit_status)
{
	
	if (g_sig_int)
	{
		*last_exit_status = 130;
		g_sig_int = 0;
		return (0);
	}
	*args = read_and_expand_input(env, argv, last_exit_status);
	if (*args == NULL && !g_sig_int)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		return (-1);
	}
	return (1);
}

static void	execute_input(char **args, t_env *env, int *last_exit_status)
{
	if (args == (char **)1 || !args || !args[0])
	{
		if (args != (char **)1)
			free_split(args);
		return ;
	}
	if (args[1] == NULL)
		*last_exit_status = execute_simple(args, env, *last_exit_status);
	else
		*last_exit_status = execute_complex(args, env, *last_exit_status);
	free_split(args);
}

void	execution_loop(t_env *env, char *argv)
{
	int		last_exit_status;
	char	**args;
	int		status;

	last_exit_status = 0;
	init_signals();
	while (1)
	{
		check_signal();
		status = handle_signals_and_input(&args, env,argv, &last_exit_status);
		if (status == -1)
			break ;
		if (status == 0)
			continue ;
		execute_input(args, env, &last_exit_status);
	}
}
void	add_env_node(t_env **env, char *key, char *value)
{
	t_env	*new;
	t_env	*tmp;

	new = new_env_node(key, value);
	if (!new)
		return ;
	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
void	update_shlvl(t_env *env)
{
	t_env *tmp = env;
	int lvl = 1;

	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0)
		{
			if (tmp->value)
				lvl = ft_atoi(tmp->value) + 1;
			free(tmp->value);
			tmp->value = ft_itoa(lvl);
			return;
		}
		tmp = tmp->next;
	}

	// If SHLVL was not found, add it with value 1
	add_env_node(&env, ft_strdup("SHLVL"), ft_strdup("1"));
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	env = init_env(envp);
	update_shlvl(env);
	execution_loop(env, argv[0]);
	free_env_list(env);
	return (0);
}
