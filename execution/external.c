#include "minishell.h"

void setup_child_io(int prev_fd, int *pipefd, char **args)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (args[1] != NULL)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

void handle_child(t_token *token, t_env *env, int prev_fd, int *pipefd, char **args)
{
	setup_child_io(prev_fd, pipefd, args);
	handle_redirections(token, env);
	if (check_cmd(token) && !args[1])
	{
		execute_builtin(token, env);
		exit(0);
	}
	else
	{
		char **cmd = command_matrix(clean_command_tokens(token));
		char **exec_path = check_cmd_path(cmd, env);
		char **arr = env_list_to_array(env);
		execve(exec_path[0], cmd, arr);
		exit(EXIT_FAILURE);
	}
}

void handle_parent(pid_t pid, pid_t *pids, int *num_cmds, int *prev_fd, int *pipefd, char **args, int i)
{
	pids[(*num_cmds)++] = pid;
	if (*prev_fd != -1)
		close(*prev_fd);
	if (args[i + 1] != NULL)
	{
		close(pipefd[1]);
		*prev_fd = pipefd[0];
	}
}

void wait_for_children(pid_t *pids, int num_cmds)
{
	for (int j = 0; j < num_cmds; j++)
	{
		int status;
		waitpid(pids[j], &status, 0);
	}
}

void execute_complex(char **args, t_env *env)
{
	int i = 0;
	int prev_fd = -1;
	pid_t pids[256];
	pid_t pid;
	int num_cmds = 0;

	process_heredocs(args);
	while (args[i])
	{
		int pipefd[2];
		t_token *token = setup_token_and_pipe(args, i, pipefd);
		pid = fork();
		if (pid == 0)
			handle_child(token, env, prev_fd, pipefd, args);
		else if (pid > 0)
		{
			handle_parent(pid, pids, &num_cmds, &prev_fd, pipefd, args, i);
			freee_tokens(token);
		}
		else
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	wait_for_children(pids, num_cmds);
	unlink(".heredoc_tmp");
}
