#include "../minishell.h"

int	is_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] == '\0')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

 void	print_arg_value(t_token *arg)
{
	if (arg->value && ft_strcmp(arg->value, "$?") == 0)
		printf("%d", g_shell.last_exit_status);
	else if (arg->value && ft_strcmp(arg->value, " ") != 0
		&& ft_strlen(arg->value) > 0)
	{
		printf("%s", arg->value);
		if (arg->next && ft_strcmp(arg->next->value, " ") != 0)
			printf(" ");
	}
}

void	execute_echo(t_token *arg, t_env *env)
{
	int	newline;

	(void)env;
	newline = 1;
	while (arg && is_n_flag(arg->value))
	{
		newline = 0;
		arg = arg->next;
	}
	while (arg)
	{
		print_arg_value(arg);
		arg = arg->next;
	}
	if (newline)
		printf("\n");
}
