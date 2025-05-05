#include "minishell.h"

int	handle_output_redirect(char *file)
{
	int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (perror("open output"), -1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_append_redirect(char *file)
{
	int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (perror("open append"), -1);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_input_redirect(char *file)
{
	int fd = open(file, O_RDONLY);
	if (fd < 0)
		return (perror("open input"), -1);
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

void	read_heredoc_input(int write_fd, const char *delim, t_env *env)
{
	char *line;

	while (1)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		line = expand_all(readline("> "),env, NULL);
		if (!line || strcmp(line, delim) == 0)
			break;
		write(write_fd, line, strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	// init_signals();
	free(line);
	close(write_fd);
	exit(0);
}