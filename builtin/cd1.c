#include "../minishell.h"

char *expand_home_path(char *path, t_env *env)
{
	char *home;
	char *full;

	home = get_env_val(env, "HOME");
	if (!home)
	{
		fprintf(stderr, "cd: HOME not set\n");
		return (NULL);
	}
	full = malloc(strlen(home) + strlen(path));
	if (!full)
		return (NULL);
	sprintf(full, "%s%s", home, path + 1);
	return (full);
}

char *resolve_cd_path(t_token *arg, t_env *env)
{
	char *path;

	if (!arg || !arg->value || ft_strlen(arg->value) == 0)
		return (strdup(get_env_val(env, "HOME")));
	path = arg->value;
	if (strcmp(path, "-") == 0)
	{
		path = get_env_val(env, "OLDPWD");
		if (!path)
			fprintf(stderr, "cd: OLDPWD not set\n");
		else
			printf("%s\n", path);
		return (strdup(path));
	}
	if (path[0] == '~')
		return (expand_home_path(path, env));
	return (strdup(path));
}

void execute_cd(t_token *arg, t_env *env)
{
	char *path;
	char *oldpwd;

	path = resolve_cd_path(arg, env);
	if (!path)
		return;
	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
	{
		perror("getcwd before chdir");
		free(path);
		return;
	}
	if (chdir(path) == -1)
	{
		perror("cd");
		free(path);
		free(oldpwd);
		return;
	}
	update_pwd_env(env, oldpwd);
	free(path);
	free(oldpwd);
}
