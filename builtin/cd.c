#include "../minishell.h"

char	*get_env_val(t_env *env, char *key)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

void update_existing_key(t_env *env, char *key, char *val)
{
	while (env)
	{
		if (strcmp(env->key, key) == 0)
		{
			free(env->value);
			env->value = strdup(val);
			return;
		}
		env = env->next;
	}
}

void append_new_key(t_env *head, char *key, char *val)
{
	t_env *new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		return;
	}
	new_node->key = strdup(key);
	new_node->value = strdup(val);
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		fprintf(stderr, "Error: memory allocation failed\n");
		return;
	}
	new_node->next = NULL;
	while (head->next)
		head = head->next;
	head->next = new_node;
}

void	set_env_val(t_env *env, char *key, char *val)
{
	if (!key)
		key = "default_key";
	if (!val)
		val = " ";
	update_existing_key(env, key, val);
	append_new_key(env, key, val);
}

void	update_pwd_env(t_env *env, char *oldpwd)
{
	char cwd[1024];
	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("getcwd");
		return;
	}
	if (oldpwd)
		set_env_val(env, "OLDPWD", oldpwd);
	set_env_val(env, "PWD", cwd);
}
