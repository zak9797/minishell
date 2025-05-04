#include "minishell.h"

int	handle_heredoc(t_token *curr, t_env *env)
{
	char	*delim = NULL;
	t_token	*tmp = curr;
	int		pipefd[2];

	while (tmp)
	{
		if (tmp->type == T_HEREDOC && tmp->next)
			delim = tmp->next->value;
		tmp = tmp->next;
	}
	if (!delim || pipe(pipefd) == -1)
		return (-1);
	 if (fork() == 0)
	{
		close(pipefd[0]);
		read_heredoc_input(pipefd[1], delim, env);
	}
	wait(NULL);
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

int	process_redirect_token(t_token *curr, t_env *env)
{
	char *file = curr->next->value;

	if (curr->type == T_OUTPUT_REDIRECT)
		if (handle_output_redirect(file) < 0) return (-1);
	if (curr->type == T_APPEND_OUTPUT)
		if (handle_append_redirect(file) < 0) return (-1);
	if (curr->type == T_INPUT_REDIRECT)
		if (handle_input_redirect(file) < 0) return (-1);
	if (curr->type == T_HEREDOC)
	{
		if (handle_heredoc((curr),env) < 0) return (-1);
	}
	return (0);
}

int	redirect_for_builtin(t_token *tokens,t_env *env)
{
	int original_stdin = dup(STDIN_FILENO);
	int original_stdout = dup(STDOUT_FILENO);
	t_token *curr = tokens;

	while (curr)
	{
		if ((curr->type == T_OUTPUT_REDIRECT || curr->type == T_APPEND_OUTPUT ||
			curr->type == T_INPUT_REDIRECT || curr->type == T_HEREDOC) && curr->next)
		{
			if (process_redirect_token((curr), env) < 0)
				return (-1);
			if (curr->type == T_HEREDOC)
				curr = curr->next;
			curr = curr->next;
			continue;
		}
		curr = curr->next;
	}
	return (original_stdin << 16 | original_stdout);
}

// Restore stdio after redirection
void restore_stdio(int packed_fd)
{
    int stdin_fd = (packed_fd >> 16) & 0xFFFF;
    int stdout_fd = packed_fd & 0xFFFF;

    dup2(stdin_fd, STDIN_FILENO);
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdin_fd);
    close(stdout_fd);
}

// Clean up tokens by removing redirection tokens
void skip_redirection_pair(t_token **curr, t_token **prev, t_token **tokens)
{
	t_token *to_delete = *curr;
	t_token *file_token = (*curr)->next;

	*curr = file_token->next;
	if (*prev)
		(*prev)->next = *curr;
	else
		*tokens = *curr;

	// Free token values first
	if (to_delete->value)
		free(to_delete->value);
	if (file_token->value)
		free(file_token->value);

	// Then free the tokens themselves
	free(to_delete);
	free(file_token);
}
