#include "minishell.h"

void free_tokens(t_token *tokens)
{
    t_token *current = tokens;
    t_token *next;

    while (current)
    {
        next = current->next;
        if (current->value)
            free(current->value);
        free(current);
        current = next;
    }
}

// Function to restore standard file descriptors
void restore_std_fds(int stdin_backup, int stdout_backup)
{
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
}

void handlee_heredoc(char *delim, t_env *env)
{
	int fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (perror("open heredoc temp file"));
	char *line;
	while (1)
	{
		line = expand_all(readline("> "),env, NULL);
		if (!line || strcmp(line, delim) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
}

void process_heredocs(char **args, t_env *env)
{
	int i = 0;
	while (args[i])
	{
		t_token *tokens = tokenize_input(args[i]);
		t_token *curr = tokens;
		while (curr)
		{
			if (curr->type == T_HEREDOC && curr->next)
				handlee_heredoc((curr->next->value),env);
			curr = curr->next;
		}
		free_tokens(tokens);
		i++;
	}
}