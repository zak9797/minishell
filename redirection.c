#include "minishell.h"

void restore_std_fds(int stdin_backup, int stdout_backup)
{
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
}

int handle_redirections(t_token *tokens, t_env *env)
{
    t_token *curr = tokens;
    t_token *prev = NULL;
    int redirection_fd = -1; // -1 means no redirection to STDOUT

    while (curr)
    {
        // Check if the token is a redirection and NOT inside quotes
        if ((curr->type == T_OUTPUT_REDIRECT || curr->type == T_APPEND_OUTPUT ||
             curr->type == T_INPUT_REDIRECT || curr->type == T_HEREDOC) &&
            curr->next)
        {
            // Apply the redirection
            if (curr->type == T_OUTPUT_REDIRECT)
            {
                char *file = curr->next->value;
                redirection_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (redirection_fd < 0) { perror("open output"); exit(EXIT_FAILURE); }
                dup2(redirection_fd, STDOUT_FILENO);
                close(redirection_fd);
            }
            else if (curr->type == T_APPEND_OUTPUT)
            {
                char *file = curr->next->value;
                redirection_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (redirection_fd < 0) { perror("open append"); exit(EXIT_FAILURE); }
                dup2(redirection_fd, STDOUT_FILENO);
                close(redirection_fd);
            }
            else if (curr->type == T_INPUT_REDIRECT)
            {
                char *file = curr->next->value;
                redirection_fd = open(file, O_RDONLY);
                if (redirection_fd < 0) { perror("open input"); exit(EXIT_FAILURE); }
                dup2(redirection_fd, STDIN_FILENO);
                close(redirection_fd);
            }
            else if (curr->type == T_HEREDOC)
            {
                char *last_heredoc_delim = NULL;
                t_token *tmp = curr;
                while (tmp)
                {
                    if (tmp->type == T_HEREDOC && tmp->next)
                    {
                        last_heredoc_delim = tmp->next->value;
                    }
                    tmp = tmp->next;
                }

                char *file = curr->next->value;
                int pipefd[2];
                if (pipe(pipefd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
                pid_t h_pid = fork();
                if (h_pid == 0) {
                    close(pipefd[0]);
                    char *line = NULL;
                    while (1) {
                        line = expand_all(readline("> "), env);
                        if (strcmp(line, file) == 0) break;
                        write(pipefd[1], line, strlen(line));
                        write(pipefd[1], "\n", 1);
                        free(line);
                    }
                    close(pipefd[1]);
                    exit(0);
                } else 
                {
                    wait(NULL);
                    close(pipefd[1]);
                    if (strcmp(file, last_heredoc_delim) == 0)
                        dup2(pipefd[0], STDIN_FILENO);
                    close(pipefd[0]);
                }
            }

            // Remove both current and next token (redirection + file)
            t_token *to_free = curr;
            t_token *file_token = curr->next;
            curr = file_token->next;

            if (prev)
                prev->next = curr;
            else
                tokens = curr;

            free(file_token);
            free(to_free);
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
    return redirection_fd;
}
int	redirect_for_builtin(t_token *tokens)
{
	int	original_stdin = dup(STDIN_FILENO);
	int	original_stdout = dup(STDOUT_FILENO);
	t_token *curr = tokens;

	while (curr)
	{
		if ((curr->type == T_OUTPUT_REDIRECT || curr->type == T_APPEND_OUTPUT ||
			 curr->type == T_INPUT_REDIRECT || curr->type == T_HEREDOC) &&
			curr->next)
		{
			char *file = curr->next->value;
			int fd;

			if (curr->type == T_OUTPUT_REDIRECT)
            {
                 fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open output"); exit(EXIT_FAILURE); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (curr->type == T_APPEND_OUTPUT)
            {
                 fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) { perror("open append"); exit(EXIT_FAILURE); }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (curr->type == T_INPUT_REDIRECT)
            {
           
                 fd = open(file, O_RDONLY);
                if (fd < 0) { perror("open input"); exit(EXIT_FAILURE); }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
			else if (curr->type == T_HEREDOC)
			{
				char *last_heredoc_delim = NULL;
				t_token *tmp = curr;
				while (tmp)
				{
					if (tmp->type == T_HEREDOC && tmp->next)
					{
						last_heredoc_delim = tmp->next->value;
					}
						tmp = tmp->next;
				}

				int pipefd[2];
				if (pipe(pipefd) == -1) return (-1);
				pid_t pid = fork();
				if (pid == 0)
				{
					close(pipefd[0]);
					char *line = NULL;
					while (1)
					{
						line = readline("> ");
						if (strcmp(line, file) == 0)
							break;
						write(pipefd[1], line, strlen(line));
						write(pipefd[1], "\n", 1);
						free(line);
					}
					close(pipefd[1]);
					exit(0);
				}
				else
				{
					wait(NULL);
					close(pipefd[1]);
					if (strcmp(file, last_heredoc_delim) == 0)
					dup2(pipefd[0], STDIN_FILENO);
					close(pipefd[0]);
					curr = curr->next; // skip next token
					continue;
				}
			}
			else
				return (-1);

			if (fd < 0)
			{
				perror("open");
				return (-1);
			}
			if (curr->type == T_INPUT_REDIRECT)
				dup2(fd, STDIN_FILENO);
			else
			{
				dup2(fd, STDOUT_FILENO);
			}
				close(fd);
				curr = curr->next; // skip file token
		}
		curr = curr->next;
	}
	return (original_stdin << 16 | original_stdout); // pack into single int
}
void	restore_stdio(int packed_fd)
{
	int stdin_fd = (packed_fd >> 16) & 0xFFFF;
	int stdout_fd = packed_fd & 0xFFFF;

	dup2(stdin_fd, STDIN_FILENO);
	dup2(stdout_fd, STDOUT_FILENO);
	close(stdin_fd);
	close(stdout_fd);
}

t_token *clean_command_tokens(t_token *tokens)
{
	t_token *curr = tokens;
	t_token *prev = NULL;

	while (curr)
	{
		if ((curr->type == T_OUTPUT_REDIRECT || curr->type == T_APPEND_OUTPUT ||
			 curr->type == T_INPUT_REDIRECT || curr->type == T_HEREDOC) &&
			curr->next)
		{
			t_token *to_delete = curr;
			t_token *file_token = curr->next;

			curr = file_token->next;

			if (prev)
				prev->next = curr;
			else
				tokens = curr;

			free(to_delete);
			free(file_token);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	return tokens;
}


char **handle_redirectionss(char **args, t_env *env, int *saved_stdin, int *saved_stdout)
{
    int i = 0;

    *saved_stdin = dup(STDIN_FILENO);  // Save the current stdin
    *saved_stdout = dup(STDOUT_FILENO); // Save the current stdout

    if (*saved_stdin == -1 || *saved_stdout == -1) {
        perror("dup");
        exit(EXIT_FAILURE);
    }

    while (args[i])
    {
        if ((strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0 ||
             strcmp(args[i], "<") == 0 || strcmp(args[i], "<<") == 0) &&
            args[i + 1])
        {
            char *op = args[i];
            char *target = args[i + 1];

            if (strcmp(op, ">") == 0)
            {
                int fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) { perror("open >"); exit(EXIT_FAILURE); }
                dup2(fd, STDOUT_FILENO); // Redirect stdout
                close(fd);
            }
            else if (strcmp(op, ">>") == 0)
            {
                int fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fd < 0) { perror("open >>"); exit(EXIT_FAILURE); }
                dup2(fd, STDOUT_FILENO); // Redirect stdout
                close(fd);
            }
            else if (strcmp(op, "<") == 0)
            {
                int fd = open(target, O_RDONLY);
                if (fd < 0) { perror("open <"); exit(EXIT_FAILURE); }
                dup2(fd, STDIN_FILENO); // Redirect stdin
                close(fd);
            }
            else if (strcmp(op, "<<") == 0)
            {
                int pipefd[2];
                if (pipe(pipefd) == -1) { perror("pipe"); exit(EXIT_FAILURE); }

                pid_t pid = fork();
                if (pid == 0)
                {
                    close(pipefd[0]);
                    char *line;
                    while (1)
                    {
                        line = expand_all(readline("> "), env);
                        if (strcmp(line, target) == 0) break;
                        write(pipefd[1], line, strlen(line));
                        write(pipefd[1], "\n", 1);
                        free(line);
                    }
                    close(pipefd[1]);
                    exit(0);
                }
                else
                {
                    wait(NULL);
                    close(pipefd[1]);
                    dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe
                    close(pipefd[0]);
                }
            }
            // Shift arguments left to remove op and target
            i++; // Skip the next argument since it's the target of the redirection
        }
        i++;
    }

    return args;
}