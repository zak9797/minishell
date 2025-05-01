#include "../minishell.h"

void	unset_env_var(t_env *env, t_token *token)
{
	t_env	*curr;
	t_env	*prev;
	char	*key;

	if (!env || !token || !token->value)
		return ;
	key = token->value;
	curr = env;
	prev = NULL;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			if (prev)
				prev->next = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
