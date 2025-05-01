#include "minishell.h"

void fork_and_execute(t_token *cleaned, t_env *env, char **arr, int redir_fd)
{

	pid_t id = fork();
	if (id == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (id == 0) // CHILD
	{
		char **cmd = command_matrix(cleaned);
		char **args = check_cmd_path(cmd, env);
		if (args && args[0] && access(args[0], X_OK) == 0)
			execve(args[0], cmd, arr);
		perror("execve failed");
		// free_env_copy(cmd);
		// free_env_copy(args);
		exit(EXIT_FAILURE); // Prevent leak in child if execve fails
	}
	else // PARENT
	{
		int status;
		waitpid(id, &status, 0);
		restore_stdio(redir_fd);
		free_env_copy(arr);
		freee_tokens(cleaned);
		// free_split(args);
	}
}

void execute_simple(char **arg, t_env *env)
{
	char **arr = env_list_to_array(env);
	t_token *tokens = tokenize_input(arg[0]);
	int redir_fd = redirect_for_builtin(tokens);
	t_token *cleaned = clean_command_tokens(tokens);

	if (check_cmd(tokens))
	{
		execute_builtin(cleaned, env);
		restore_stdio(redir_fd);
		free_env_copy(arr);
		freee_tokens(cleaned);
	}
	else
	{
		fork_and_execute(cleaned, env, arr, redir_fd);
	}
}

char **env_list_to_array(t_env *env)
{
	int		i = 0;
	int		count = 0;
	t_env	*tmp = env;
	char	**envp;
	char	*tmp_str;

	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		tmp_str = ft_strjoin(tmp->key, "=");
		envp[i] = ft_strjoin(tmp_str, tmp->value);
		free(tmp_str);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}
t_token *setup_token_and_pipe(char **args, int i, int *pipefd)
{
	t_token *token = tokenize_input(args[i]);
	if (args[i + 1] != NULL && pipe(pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	return token;
}