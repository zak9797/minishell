#include "minishell.h"

void handle_double_redirection(char **current_word, t_token **tokens, int *i, char *input)
{
	if (**current_word)
	{
		add_token(tokens, create_token(T_WORD, *current_word, NO_QUOTE));
		free(*current_word);
		*current_word = strdup("");
	}
	char symbol[3] = { input[*i], input[*i + 1], '\0' };
	add_token(tokens, create_token(get_token_type(input, *i), symbol, NO_QUOTE));
	(*i) += 2;
}

void handle_single_operator(char **current_word, t_token **tokens, int *i, char *input)
{
	if (**current_word)
	{
		t_token *word_token = create_token(T_WORD, *current_word, NO_QUOTE);
		if (!word_token)
			return; // handle error if needed
		add_token(tokens, word_token);
		free(*current_word);
		*current_word = strdup(""); // also check strdup if needed
	}

	char symbol[2] = { input[*i], '\0' };
	t_token *op_token = create_token(get_token_type(input, *i), symbol, NO_QUOTE);
	if (!op_token)
		return; // handle error
	add_token(tokens, op_token);
	(*i)++;
}

void free_split(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}