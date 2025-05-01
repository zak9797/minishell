#include "minishell.h"

void	free_env_copy(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
		free(env[i++]);
	free(env);
}

t_env	*new_env_node(char *key, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*current;
	t_env	*node;
	char	*sep;

	head = NULL;
	current = NULL;
	while (*envp)
	{
		sep = ft_strchr(*envp, '=');
		if (!sep++)
			return (envp++, NULL);
		node = new_env_node(ft_strndup(*envp, sep - *envp - 1), ft_strdup(sep));
		if (!node || !node->key || !node->value)
			return (free_env(head), NULL);
		if (!head)
			head = node;
		else
			current->next = node;
		current = node;
		envp++;
	}
	return (head);
}

void	free_env(t_env *env)
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
