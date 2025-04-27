#include "minishell.h"

// Function to create a new token

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

 t_token *create_token(t_token_type type, char *value)
{
	t_token *new = malloc(sizeof(t_token));
	if (!new)
		return NULL;
	new->type = type;
	new->value = strdup(value);
	//new->quote = quote_type;
	new->next = NULL;
	return new;
}

 void	add_token(t_token **head, t_token *new)
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
    //tokens->value = NULL;
    //tokens->next = NULL;
    //tokens->quote = NULL;
    //tokens->type = NULL; 
    // Skip spaces only if NOT inside quotes
    if (isspace(input[i]))
    {
        i++;
    }
		if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
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
			while (input[i] && input[i] != '|' && input[i] != '<' && input[i] != '>')
				i++;
			char *word = strndup(&input[start], i - start);
			add_token(&tokens, create_token(T_WORD, word));
			free(word);
		}
	
	return tokens;
}

void print_tokens(t_token *head)
{
    int expect_command = 1; // At start, expect a command

    while (head)
    {
        const char *type_str;

        switch (head->type)
        {
            case T_PIPE:
                type_str = "PIPE";
                expect_command = 1; // After a pipe, expect a new command
                break;
            case T_OUTPUT_REDIRECT:
                type_str = "OUTPUT REDIRECT";
                break;
            case T_APPEND_OUTPUT:
                type_str = "APPEND OUTPUT";
                break;
            case T_INPUT_REDIRECT:
                type_str = "INPUT REDIRECT";
                break;
            case T_HEREDOC:
                type_str = "HEREDOC";
                break;
            case T_WORD:
                if (expect_command)
                {
                    if (check_cmd(head))
                        type_str = "BUILT-IN COMMAND";
                    else
                        type_str = "COMMAND";
                    expect_command = 0;
                }
                else
                {
                    type_str = "ARG";
                }
                break;
            default:
                type_str = "UNKNOWN";
        }

        printf("Type: %-18s | Value: '%s'\n", type_str, head->value);
        head = head->next;
    }
}
char *replace_escapes(const char *input)
{
    char *result = malloc(strlen(input) + 1); // safe size for now
    int j = 0;
    for (int i = 0; input[i]; i++)
    {
        if (input[i] == '\\' && input[i + 1])
        {
            i++;
            if (input[i] == 'n') result[j++] = '\n';
            else if (input[i] == 't') result[j++] = '\t';
            else if (input[i] == ' ') result[j++] = ' ';
            else result[j++] = input[i]; // just copy e.g. '\\'
        }
        else
        {
            result[j++] = input[i];
        }
    }
    result[j] = '\0';
    return result;
}

int skip_quots(char *str, int i)
{
    char    c;

    if (str[i] == '"' || str[i] == '\'')
        c = str[i];
    else
        return (i);
    i++;
    while (str[i] != c)
        i++;
    return (i);
}

void    remove_char(char **str, int i)
{
    while((*str)[i + 1])
    {
        (*str)[i] = (*str)[i + 1];
        i++;
    }
    (*str)[i] = '\0';
}

void    remove_quots(char **str)
{
    int     i;
    char    c;

    i = 0;
    while ((*str)[i])
    {
        if ((*str)[i] == '"' || (*str)[i] == '\'')
        {
            c = (*str)[i];
            remove_char(str, i);
            while ((*str)[i] != c)
                i++;
            remove_char(str, i);
        }
        i++;
    }
}

t_token *tokenizee_input(char *input)
{
    t_token *tokens = NULL;
    int i = 0;

    while (input[i])
    {
        // Skip whitespace
        if (isspace(input[i]))
        {
            i++;
        }
        // Handle single or double quoted strings
        if (input[i] == '\'' || input[i] == '"')
        {
            char quote = input[i++];
            int start = i;

            while (input[i] && input[i] != quote)
                i++;

            char *quoted = ft_strndup(&input[start], i - start);
            add_token(&tokens, create_token(T_WORD, quoted));
            free(quoted);

            if (input[i] == quote)
                i++; // skip the closing quote

            continue;
        }

        // Handle >> or <<
        if ((input[i] == '>' && input[i + 1] == '>') || (input[i] == '<' && input[i + 1] == '<'))
        {
            char symbol[3] = { input[i], input[i + 1], '\0' };
            add_token(&tokens, create_token(get_token_type(input, i), symbol));
            i += 2;
            continue;
        }

        // Handle > < |
        if (input[i] == '>' || input[i] == '<' || input[i] == '|')
        {
            char symbol[2] = { input[i], '\0' };
            add_token(&tokens, create_token(get_token_type(input, i), symbol));
            i++;
            continue;
        }

        // Handle normal unquoted words
        int start = i;
        while (input[i] &&
               !isspace(input[i]) &&
               input[i] != '|' &&
               input[i] != '<' &&
               input[i] != '>' &&
               input[i] != '"' &&
               input[i] != '\'')
        {
            // i = skip_quots(input, i);
            i++;
        }
        // printf("hello %d\n", i);
        char *word = ft_strndup(&input[start], i - start);
        // remove_quots(&word);
        // printf("hello %d\n", i);
        add_token(&tokens, create_token(T_WORD, word));
        free(word);
    }
    // printf("out\n");
    return tokens;
}
void free_split(char **arr)
{
    if (!arr)
        return;
    for (int i = 0; arr[i]; i++)
        free(arr[i]);
    free(arr);
}

int main(int argc, char **argv, char **envp)
{
    t_env *env;
    (void)argc;
    (void)argv;
    char *input;
    char **args;

    env = init_env(envp);
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)
            break;
        if (input[0] == '\0') // If user pressed only Enter
        {
            free(input);
            continue;
        }
        add_history(input);

        char *expanding = expand_all(input, env);
        free(input); // free input after expanding

        if (!expanding) // very important
            continue;

        args = ft_split(expanding, '|');
        free(expanding); // free expanded input after split

        if (!args || !args[0]) // safety check
        {
            free_split(args); // custom function to free char **
            continue;
        }

        if (args[1] == NULL)
            execute_simple(args, env);
        else
        {
            execute_complex(args, env);
        }
        
        free_split(args); // custom function to free char **
    }
    return 0;
}
