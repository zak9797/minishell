#include "minishell.h"

t_token *create_token(t_token_type type, char *value, t_quote_type quote_type)
{
    t_token *new = malloc(sizeof(t_token));
    if (!new)
        return NULL;
    new->type = type;
    new->value = strdup(value);
    new->quote = quote_type;  // Assign the quote type
    new->next = NULL;
    new->exit_status = 0;
    return new;
}

// Token type determination
t_token_type get_token_type(char *input, int i)
{
    if (input[i] == '|')
        return T_PIPE;
    else if (input[i] == '<' && input[i + 1] == '<')
        return T_HEREDOC;  // '<<'
    else if (input[i] == '>' && input[i + 1] == '>')
        return T_APPEND_OUTPUT;  // '>>'
    else if (input[i] == '>')
        return T_OUTPUT_REDIRECT;  // '>'
    else if (input[i] == '<')
        return T_INPUT_REDIRECT;   // '<'
    return T_WORD;
}

// Append two strings
char *str_append1(char *s1, char *s2)
{
    if (!s1 && !s2)
        return NULL;
    if (!s1)
        return strdup(s2);
    if (!s2)
        return strdup(s1);

    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (!result)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
void handle_whitespace(char **current_word, t_token **tokens, int *i, char *input)
{
    (void)input;
	if (**current_word)
	{
		add_token(tokens, create_token(T_WORD, *current_word, NO_QUOTE));
		free(*current_word);
		*current_word = strdup("");
	}
	(*i)++;
}

void handle_quotes(char **current_word, int *i, char *input)
{
	char quote = input[(*i)++];
	int start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	char *quoted = strndup(&input[start], *i - start);
	char *tmp = *current_word;
	*current_word = str_append1(*current_word, quoted);
	free(tmp);
	free(quoted);
	if (input[*i] == quote)
		(*i)++;
}