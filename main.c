#include"minishell.h"

// Function to create a new token
int check_cmd(t_token *cmd_token)
{
    // List of built-in commands
    char *builtins[] = {"echo", "cd", "pwd", "export", "unset", "env", "exit"};
    int i = 0;

    // Check if the token is a command and compare it to the built-ins
    if (cmd_token && cmd_token->type == T_WORD)
    {
        for (i = 0; i < 7; i++)
        {
            if (strcmp(cmd_token->value, builtins[i]) == 0)
            {
                return 1; // It's a built-in command
            }
        }
    }

    return 0; // It's not a built-in command
}

static t_token_type get_token_type(char *input, int i)
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

static t_token *create_token(t_token_type type, char *value)
{
	t_token *new = malloc(sizeof(t_token));
	if (!new)
		return NULL;
	new->type = type;
	new->value = strdup(value);
	new->next = NULL;
	return new;
}

static void	add_token(t_token **head, t_token *new)
{
	if (!*head)
		*head = new;
	else
	{
		t_token *temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

t_token *tokenize_input(char *input)
{
	t_token *tokens = NULL;
	int i = 0;

	while (input[i])
	{
		if (isspace(input[i]))
		{
			i++;
			continue;
		}
		else if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
		{
			char symbol[3] = { input[i], input[i + 1], '\0' };
			add_token(&tokens, create_token(get_token_type(input, i), symbol));
			i += 2;  // Skip the next character as it's part of the operator
		}
		else if (input[i] == '>' || input[i] == '<' || input[i] == '|')
		{
			char symbol[2] = { input[i], '\0' };
			add_token(&tokens, create_token(get_token_type(input, i), symbol));
			i++;
		}
		else
		{
			int start = i;
			while (input[i] && !isspace(input[i]) && input[i] != '|' && input[i] != '<' && input[i] != '>')
				i++;
			char *word = strndup(&input[start], i - start);
			add_token(&tokens, create_token(T_WORD, word));
			free(word);
		}
	}
	return tokens;
}
void print_tokens(t_token *head)
{
    while (head)
    {
        const char *type_str;
        if (head->type == T_WORD)
        {
            if (check_cmd(head))
                type_str = "BUILT-IN COMMAND";  // If it's a built-in command
            else
                type_str = "COMMAND";  // If it's just a regular command
        }
        else if (head->type == T_PIPE)
            type_str = "PIPE";
        else if (head->type == T_OUTPUT_REDIRECT)
            type_str = "OUTPUT REDIRECT";
        else if (head->type == T_APPEND_OUTPUT)
            type_str = "APPEND OUTPUT";
        else if (head->type == T_INPUT_REDIRECT)
            type_str = "INPUT REDIRECT";
        else if (head->type == T_HEREDOC)
            type_str = "HEREDOC";
        else
            type_str = "UNKNOWN";

        printf("Type: %-15s | Value: '%s'\n", type_str, head->value);
        head = head->next;
    }
}

void free_tokens(t_token *head)
{
	while (head)
	{
		t_token *temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}
int main(void)
{
	char *input;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break;

		t_token *tokens = tokenize_input(input);
		print_tokens(tokens);
		free_tokens(tokens);
		free(input);
	}

	return 0;
}