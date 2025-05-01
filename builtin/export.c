#include "minishell.h"

char	*strip_quotes(const char *val)
{
	size_t	len;

	if (!val)
		return (NULL);
	len = ft_strlen(val);
	if (len >= 2 && ((val[0] == '"' && val[len - 1] == '"')
			|| (val[0] == '\'' && val[len - 1] == '\'')))
		return (ft_strndup(val + 1, len - 2));
	return (ft_strdup(val));
}

int	is_valid_export_arg(char *arg)
{
	int	i;

	if (!arg || (!ft_isalpha(arg[0]) && arg[0] != '_'))
		return (0);
	i = 0;
	while (arg[i] && arg[i] != '=')
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	print_export_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key);
		env = env->next;
	}
}
void	handle_assign(char *arg, t_env *env)
{
	char	*equal;
	char	*key;
	char	*value;
	char	*expanded;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return ;
	*equal = '\0';
	key = arg;
	if (!key || !*key)
		return ((void)(*equal = '='));
	value = equal + 1;
	expanded = expand_all(value, env);
	if (!expanded)
		return ((void)(*equal = '='));
	set_env_val(env, key, expanded);
	free(expanded);
	*equal = '=';
}

void	execute_export(t_token *token, t_env *env)
{
	char	*arg;

	if (!token)
		return (print_export_env(env));
	while (token)
	{
		arg = token->value;
		if (ft_strchr(arg, '='))
			handle_assign(arg, env);
		else if (*arg && !get_env_val(env, arg))
			set_env_val(env, arg, "");
		token = token->next;
	}
}
