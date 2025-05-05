#include "minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	g_sig_int = 1;
	write(STDOUT_FILENO, "\n", 1);       // Move first
	rl_on_new_line();                   // Set readline to new line
	rl_replace_line("", 0);            // Clear current line
	rl_redisplay();                    // Redisplay the prompt
}

int	check_signal(void)
{
	if (g_sig_int)
	{
		g_sig_int = 0;
		// dup2(STDOUT_FILENO, STDIN_FILENO);
		return (1);
	}
	return (0);
}

void	init_signals(void)
{
	struct sigaction	sa;

	rl_catch_signals = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}