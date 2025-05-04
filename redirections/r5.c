
#include "minishell.h"

t_token	*clean_command_tokens(t_token *tokens)
{
	t_token *curr = tokens;
	t_token *prev = NULL;

	while (curr)
	{
		if ((curr->type == T_OUTPUT_REDIRECT || curr->type == T_APPEND_OUTPUT ||
			 curr->type == T_INPUT_REDIRECT || curr->type == T_HEREDOC) &&
			curr->next)
		{
			skip_redirection_pair(&curr, &prev, &tokens);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
	return tokens;
}