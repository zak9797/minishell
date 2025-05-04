#include "minishell.h"

int fork_and_execute(t_token *cleaned, t_env *env, char **arr, int redir_fd, int last_exit_status)
{
	pid_t id = fork();
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (id == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (id == 0) // CHILD
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		char **cmd = command_matrix(cleaned);
		char **args = check_cmd_path(cmd, env, last_exit_status);
		if (args == NULL)
        {
            exit(127); 
        }
		if (args && args[0] && access(args[0], X_OK) == 0)
			execve(args[0], cmd, arr);
		else
		{
		perror("execve");
		exit(127);
		}
	}
	else // PARENT
	{
		int exit_status = 0;
		int status;
		waitpid(id, &status, 0);
		restore_stdio(redir_fd);
		if (WIFSIGNALED(status))
		{
			int sig = WTERMSIG(status);
			if (sig == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (sig == SIGQUIT)
				write(STDOUT_FILENO, "Quit: (core dumped)\n", 21);
		
			exit_status = 128 + sig;  // ✅ Correct exit code for signal-terminated process
		}
		else if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
		}
		free_env_copy(arr);
		freee_tokens(cleaned);
		init_signals();
		return exit_status;
		// free_split(args); <-- only needed if you malloc'd args
	}
	return last_exit_status;
}

int execute_simple(char **arg, t_env *env, int last_exit_status)
{
	// process_heredocs(arg);
	char **arr = env_list_to_array(env);
	t_token *tokens = tokenize_input(arg[0]);
	int redir_fd = redirect_for_builtin(tokens, env);
	t_token *cleaned = clean_command_tokens(tokens);

	if (check_cmd(tokens))
	{
		last_exit_status = execute_builtin(cleaned, env, last_exit_status);
		restore_stdio(redir_fd);
		free_env_copy(arr);
		freee_tokens(cleaned);
		return last_exit_status;;
	}
	else
	{
		last_exit_status = fork_and_execute(cleaned, env, arr, redir_fd, last_exit_status);
	}
	return last_exit_status;
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