#include "../minishell.h"

// char *expand_vars(const char *input)
// {
// 	char *result = malloc(...); // dynamically build the result
// 	size_t i = 0, j = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == '$' && input[i + 1] == '?')
// 		{
// 			char *code = ft_itoa(last_exit_status);
// 			strcpy(&result[j], code);
// 			j += strlen(code);
// 			i += 2;
// 			free(code);
// 		}
// 		else
// 		{
// 			result[j++] = input[i++];
// 		}
// 	}
// 	result[j] = '\0';
// 	return result;
// }
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
	 if(arg->value && ft_strcmp(arg->value, "$?") == 0)
	{
		check_signal();
		if (g_sig_int)
		{
			last_exit_status = 130;
			g_sig_int = 0;
		}
		printf("%d", last_exit_status);
		last_exit_status=0;
	}
	 else if (arg->value && ft_strcmp(arg->value, " ") != 0
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
