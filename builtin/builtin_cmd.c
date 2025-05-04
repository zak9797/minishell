#include "../minishell.h"

int	is_builtin(const char *cmd)
{
	char	*builtins[] = {
		"echo", "cd", "pwd", "export", "unset", "env", "exit"
	};
	int		i;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	check_cmd(t_token *cmd_token)
{
	if (!cmd_token || cmd_token->type != T_WORD || !cmd_token->value)
		return (0);
	return (is_builtin(cmd_token->value));
}

int  dispatch_builtin(t_token *cmd_token, t_env *env, int last_exit_status)
{
	int q =0;
	t_token	*next;

	next = cmd_token->next;
	if (ft_strcmp(cmd_token->value, "pwd") == 0)
		execute_pwd(env);
	else if (ft_strcmp(cmd_token->value, "env") == 0)
		execute_env(env);
	else if (ft_strcmp(cmd_token->value, "cd") == 0)
		execute_cd(next, env);
	else if (ft_strcmp(cmd_token->value, "echo") == 0)
		q = execute_echo(next, env, last_exit_status);
	else if (ft_strcmp(cmd_token->value, "export") == 0)
		execute_export(next, env);
	else if (ft_strcmp(cmd_token->value, "unset") == 0)
		unset_env_var(env, next);
	else if (ft_strcmp(cmd_token->value, "exit") == 0)
		execute_exit(cmd_token);

	return q;
	
}

int execute_builtin(t_token *tokens, t_env *env , int last_exit_status)
{
	int i= 0;
	// if (!tokens || !check_cmd(tokens))
	// 	return ;
	i = dispatch_builtin(tokens, env, last_exit_status);
	return i;
}