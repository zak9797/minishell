#include "minishell.h"

void signal_handler(int sig)
{
    if (sig == SIGINT) {
        // Handle SIGINT (Ctrl+C)
        write(STDOUT_FILENO, "\nMinishell: Interrupted by SIGINT\n", 32);
        // You can add other actions here (like resetting the prompt, etc.)
    }
    else if (sig == SIGQUIT) {
        // Handle SIGQUIT (Ctrl+\)
        write(STDOUT_FILENO, "Minishell: Quit signal received\n", 32);
        // You can also handle this signal in a similar manner
    }
}

// Handle redirections for built-ins
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

void	read_heredoc_input(int write_fd, const char *delim)
{
	char *line;

	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delim) == 0)
			break;
		write(write_fd, line, strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	free(line);
	close(write_fd);
	exit(0);
}