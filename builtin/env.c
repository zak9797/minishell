#include "minishell.h"

int	env_count(t_env *env)
{
	int		count;

	count = 0;
	while (env)
	{
		if (env->key && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

char	*build_env_line(t_env *e)
{
	char	*line;
	size_t	len;

	len = ft_strlen(e->key) + ft_strlen(e->value) + 2;
	line = malloc(len);
	if (!line)
		return (NULL);
	snprintf(line, len, "%s=%s", e->key, e->value);
	return (line);
}

char	**env_cpy(t_env *env)
{
	char	**arr;
	int		i;

	arr = malloc(sizeof(char *) * (env_count(env) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->key && env->value)
		{
			arr[i] = build_env_line(env);
			if (!arr[i++])
				return (free_env_copy(arr), NULL);
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

void	execute_env(t_env *env)
{
	char	**z;
	int		i;

	z = env_cpy(env);
	if (!z)
	{
		fprintf(stderr, "env: memory allocation error\n");
		return ;
	}
	i = 0;
	while (z[i])
		printf("%s\n", z[i++]);
	free_env_copy(z);
}
