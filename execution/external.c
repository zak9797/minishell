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
// void execute_external(t_token *token, t_env *env, int last_exit_status)
// {
// 	char **cmd = command_matrix(clean_command_tokens(token));
// 	char **exec_path = check_cmd_path(cmd, env, last_exit_status);
// 	char **arr = env_list_to_array(env);

// 	if (!cmd || !exec_path || !arr)
// 	{
// 		free_env_copy(cmd);
// 		free_env_copy(exec_path);
// 		free_env_copy(arr);
// 		exit(EXIT_FAILURE);
// 	}

// 	execve(exec_path[0], cmd, arr);
// 	perror("execve failed");

// 	free_env_copy(cmd);
// 	free_env_copy(exec_path);
// 	free_env_copy(arr);
// 	exit(EXIT_FAILURE);
// }

int handle_child(t_token *token, t_env *env, int prev_fd, int *pipefd, char **args, int last_exit_status)
{
    setup_child_io(prev_fd, pipefd, args);
    handle_redirections(token, env);
    
    if (check_cmd(token))
    {
        execute_builtin(token, env, last_exit_status);
        exit(last_exit_status);
    }
    else
    {
        char **cmd = command_matrix(clean_command_tokens(token));
        char **exec_path = check_cmd_path(cmd, env, last_exit_status);
        
        if (exec_path == NULL)
        {
            // Handle the case where the command was not found
            // fprintf(stderr, "%s: command not found\n", cmd[0]);
            exit(127);  // Exit with the last exit status (127 for not found)
        }
        
        char **arr = env_list_to_array(env);
        // fprintf(stderr, "Attempting to execute: %s\n", exec_path[0]);
        
        if (execve(exec_path[0], cmd, arr) == -1)
        {
            perror("execve");
            exit(127);  // Exit with 127 if execve fails
        }
    }
    return 0; 
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

int wait_for_children(pid_t *pids, int num_cmds)
{
	int status;
	int final_exit_status = 0;

	for (int j = 0; j < num_cmds; j++)
	{
		waitpid(pids[j], &status, 0);

		// Only capture the status of the last command
		if (j == num_cmds - 1)
		{
			if (WIFEXITED(status))
			{
				final_exit_status = WEXITSTATUS(status);
				// fprintf(stderr, "Exit status of last command: %d\n", final_exit_status);
			}
			else if (WIFSIGNALED(status))
			{
				final_exit_status = 128 + WTERMSIG(status);

				// Print signal message like bash
				if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				else if (WTERMSIG(status) == SIGQUIT)
					write(STDOUT_FILENO, "Quit: (core dumped)\n", 21);
			}
		}
	}

	init_signals();
	return final_exit_status;
}

int execute_complex(char **args, t_env *env, int last_exit_status)
{
	int i = 0;
	int prev_fd = -1;
	pid_t pids[256];
	pid_t pid;
	int num_cmds = 0;
	process_heredocs(args, env);

	while (args[i])
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		int pipefd[2];
		t_token *token = setup_token_and_pipe(args, i, pipefd);
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			int exit_status = handle_child(token, env, prev_fd, pipefd, args, last_exit_status);
			printf("%d", exit_status);
		}
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
	last_exit_status = wait_for_children(pids, num_cmds); 
	unlink(".heredoc_tmp");
	return last_exit_status;
}
