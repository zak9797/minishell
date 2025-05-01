#include "minishell.h"

void freee_tokens(t_token *head) {
    t_token *tmp;
    while (head) {
        tmp = head->next;
        if (head->value)
            free(head->value);
        free(head);
        head = tmp;
    }
}
bool is_double_redirection(char *input, int i)
{
	return (input[i] == '>' && input[i + 1] && input[i + 1] == '>') ||
	       (input[i] == '<' && input[i + 1] && input[i + 1] == '<');
}

t_token *process_input_part1(char *input, char **current_word, int *i, t_token **tokens)
{
	while (input[*i])
	{
		if (isspace(input[*i]))
		{
			handle_whitespace(current_word, tokens, i, input);
			continue;
		}
		if (input[*i] == '\'' || input[*i] == '"')
		{
			handle_quotes(current_word, i, input);
			continue;
		}
		if (is_double_redirection(input, *i))
		{
			handle_double_redirection(current_word, tokens, i, input);
			continue;
		}
		if (input[*i] == '>' || input[*i] == '<' || input[*i] == '|')
		{
			handle_single_operator(current_word, tokens, i, input);
			continue;
		}
		break;
	}
	return NULL;
}
t_token *process_input_loop(char *input, char **current_word)
{
	t_token *tokens = NULL;
	int i = 0;

	while (input[i])
	{
		if (process_input_part1(input, current_word, &i, &tokens) != NULL)
			continue;
	
		if (input[i] == '\0')  // ← Prevent overread
			break;
	
		char tmp[2] = { input[i], '\0' };
		char *old = *current_word;
		*current_word = str_append1(*current_word, tmp);
		free(old);
		i++;
	}
	return tokens;
}

t_token *tokenize_input(char *input)
{
	if (input == NULL)
		return NULL;

	char *current_word = strdup("");
	t_token *tokens = process_input_loop(input, &current_word);

	if (*current_word)
		add_token(&tokens, create_token(T_WORD, current_word, NO_QUOTE));

	free(current_word);
	return tokens;
}