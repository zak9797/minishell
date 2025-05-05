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

int print_arg_value(t_token *arg, int last_exit_status)
{
	//  if(arg->value && ft_strcmp(arg->value, "$?") == 0)
	if (arg->value && ft_strcmp(arg->value, " ") != 0
		&& ft_strlen(arg->value) > 0)
	{
		printf("%s", arg->value);
		if (arg->next && ft_strcmp(arg->next->value, " ") != 0)
			printf(" ");
	}
	return last_exit_status;
}


int execute_echo(t_token *arg, t_env *env, int last_exit_status)
{
	int w=0;
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
		w = print_arg_value(arg, last_exit_status);
		arg = arg->next;
	}
	if (newline)
		printf("\n");

	return w;
}
